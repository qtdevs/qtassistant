#include "ManagementModule.h"
#include "ManagementModule_p.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPixmap>
#include <QStringBuilder>
#include <QTextStream>
#include <QTranslator>
#include <QUuid>
#include <QtDebug>
#include <QMetaEnum>

#include "sqldatas/MasterLevels.h"
#include "sqldatas/MemberBlacklist.h"
#include "sqldatas/MemberWatchlist.h"

#include "HtmlDraw/HtmlDraw.h"
#include "ManagementFilters.h"

// class ManagementModule

ManagementModule::ManagementModule(CoolQ::ServiceEngine *engine)
    : CoolQ::ServiceModule(*new ManagementModulePrivate(), engine)
{
    Q_ASSERT(ManagementModulePrivate::instance == nullptr);
    ManagementModulePrivate::instance = this;

    Q_D(ManagementModule);

    QTranslator *translator = new QTranslator(qApp);
    if (translator->load("qtassistant_zh.qm", ":/translations")) {
        qApp->installTranslator(translator);
    }

    d->levels = new MasterLevels(this);
    d->blacklist = new MemberBlacklist(this);
    d->watchlist = new MemberWatchlist(this);

    new HtmlDraw(usrFilePath("Materials"), this);

    d->checkTimerId = startTimer(10000);

    QFile file(usrFilePath("Management.json"));
    if (file.open(QFile::ReadOnly)) {
        auto doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isEmpty() && doc.isObject()) {
            d->init(doc.object());
        }
    }

    d->levels->init(d->superUsers);

    QFileInfo rootInfo(imgFilePath("Welcomes"));
    if (rootInfo.isDir()) {
        QDir root(rootInfo.absoluteFilePath());
        for (auto fileInfo : root.entryInfoList(QDir::Files, QDir::Name)) {
            if ((fileInfo.suffix().toLower() == "png") || (fileInfo.suffix().toLower() == "jpg")) {
                auto fileName = fileInfo.absoluteFilePath();
                fileName = fileName.mid(d->imagePath.count() + 1);
                d->welcomeImages << QDir::toNativeSeparators(fileName);
            }
        }
    }

    // Private Commands

    new PrivateRestartComputer(this);

    new PrivateCreateStartupShortcut(this);
    new PrivateDeleteStartupShortcut(this);

    // Group Commands

    new GroupBanHongbaoAction(d->banHongbaoGroups, this);

    new GroupCommandsAction(this);

    new GroupBanMemberAction(this);
    new GroupKickMemberAction(this);
    new GroupRaiseMemberAction(this);

    new GroupUnbanMemberAction(this);
    new GroupLowerMemberAction(this);

    new GroupWatchlistAction(this);
    new GroupBlacklistAction(this);

    // Group Help Commands

    new GroupBanMemberHelpAction(this);
    new GroupKickMemberHelpAction(this);
    new GroupRaiseMemberHelpAction(this);

    new GroupUnbanMemberHelpAction(this);
    new GroupLowerMemberHelpAction(this);

    new GroupWatchlistHelpAction(this);
    new GroupBlacklistHelpAction(this);
}

ManagementModule::~ManagementModule()
{
    Q_D(ManagementModule);

    ManagementModulePrivate::instance = nullptr;

    killTimer(d->checkTimerId);
}

ManagementModule *ManagementModule::instance()
{
    return ManagementModulePrivate::instance;
}

MasterLevel ManagementModule::level(qint64 gid, qint64 uid) const
{
    Q_D(const ManagementModule);

    return d->levels->level(gid, uid);
}

void ManagementModule::timerEvent(QTimerEvent *)
{
    Q_D(ManagementModule);

    // 检查新手名单。
    do {
        CoolQ::MemberList members;
        d->watchlist->expiredMembers(members);
        for (const auto &member : members) {
            CoolQ::MemberInfo mi = memberInfo(member.first, member.second, false);
            if (mi.isValid() && mi.lastSent().isNull()) {
                kickGroupMember(member.first, member.second, false);
            }
        }
    } while (false);
}

void ManagementModule::permissionDenied(qint64 gid, qint64 uid, MasterLevel level, const QString &reason)
{
    Q_UNUSED(uid);

    QString content = reason.isEmpty() ? tr("作为一个%1，你没有权限执行此操作。").arg(MasterLevels::levelName(level)) : reason;
    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(tr("没有相关权限"), content);

    QPixmap feedback = HtmlDraw::drawDangerText(html, 400, gid);
    QString fileName = saveImage(feedback);
    sendGroupMessage(gid, cqImage(fileName));
}

void ManagementModule::showPrimary(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlDraw::Primary);
}

void ManagementModule::showDanger(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlDraw::Danger);
}

void ManagementModule::showWarning(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlDraw::Warning);
}

void ManagementModule::showPrompt(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlDraw::Prompt);
}

void ManagementModule::showSuccess(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlDraw::Success);
}

void ManagementModule::feedback(qint64 gid, const QString &title, const QString &content, HtmlDraw::Style style)
{
    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(title, content);

    QPixmap feedback = HtmlDraw::drawText(html, style, 400, gid);
    QString fileName = saveImage(feedback);
    sendGroupMessage(gid, cqImage(fileName));
}

void ManagementModule::showPrimaryList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlDraw::Primary);
}

void ManagementModule::showDangerList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlDraw::Danger);
}

void ManagementModule::showWarningList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlDraw::Warning);
}

void ManagementModule::showPromptList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlDraw::Prompt);
}

void ManagementModule::showSuccessList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlDraw::Success);
}

void ManagementModule::feedbackList(qint64 gid, const QString &title, const LevelInfoList &members, bool level, HtmlDraw::Style style)
{
    for (int i = 0, part = 0; i < members.count();) {
        QString html;
        do {
            int cc = i + 4;
            QTextStream ds(&html);
            ds << "<html><body><span class=\"t\">" << title;
            if (members.count() > 5) {
                ds << tr("(Part %1)").arg(++part);
            }
            ds << "</span><div>";
            for (; i < members.count(); ++i) {
                const LevelInfo &li = members.at(i);
                ds << "<p class=\"c\">";
                if (level) {
                    ds << MasterLevels::levelName(li.level) << tr(": ");
                }
                CoolQ::MemberInfo mi = memberInfo(gid, li.uid);
                if (mi.isValid()) {
                    if (!mi.nameCard().isEmpty()) {
                        ds << mi.nameCard();
                    } else {
                        ds << mi.nickName();
                    }
                } else {
                    CoolQ::PersonInfo pi = personInfo(li.uid);
                    if (pi.isValid()) {
                        ds << pi.nickName();
                    } else {
                        ds << QString::number(li.uid);
                    }
                    ds << "<span>(" << tr("不在本群") << ")</span>";
                }
                ds << "</p>";

                if (i == cc) {
                    ++i;
                    break;
                }
            }
            ds << "</div></body></html>";
        } while (false);

        QPixmap feedback = HtmlDraw::drawText(html, style, 400, gid);
        QString fileName = saveImage(feedback);
        sendGroupMessage(gid, cqImage(fileName));
    }
}

void ManagementModule::showWelcomes(qint64 gid, qint64 uid)
{
    Q_D(ManagementModule);

    MasterLevel level = d->levels->level(gid, uid);
    if (MasterLevel::ATField < level) {
        return;
    }

    QString msg;
    QTextStream ts(&msg);

    for (auto fileName : d->welcomeImages)
        ts << cqImage(fileName) << "\n";

    auto keys = QUuid::createUuid().toRfc4122().toHex();
    ts << QString::fromLatin1(keys).left(8) << " - " << at(uid);

    ts.flush();

    sendGroupMessage(gid, msg);
}

void ManagementModule::saveWelcomes(qint64 gid, qint64 uid)
{
    Q_D(ManagementModule);

    MasterLevel level = d->levels->level(gid, uid);
    if (MasterLevel::ATField < level) {
        return;
    }

    QFileInfo rootInfo(imgFilePath("Welcomes"));
    if (rootInfo.isDir()) {
        QDir root(rootInfo.absoluteFilePath());
        for (auto fileInfo : root.entryInfoList(QDir::Files, QDir::Name)) {
            if (fileInfo.suffix() == "txt") {
                QStringList nameParts = fileInfo.fileName().split('.');
                if (nameParts.count() == 3) {
                    QFile file(fileInfo.absoluteFilePath());
                    if (file.open(QFile::ReadOnly)) {
                        auto html = QString::fromUtf8(file.readAll());
                        auto styleEnum = QMetaEnum::fromType<HtmlDraw::Style>();
                        auto style = styleEnum.keysToValue(nameParts.at(1).toLatin1());
                        auto image = HtmlDraw::drawText(html, (HtmlDraw::Style)style, 400, gid);
                        auto fileName = imgFilePath(QString("Welcomes/%1.PNG").arg(nameParts.at(0)));
                        if (image.save(fileName, "PNG")) {
                            qInfo() << "Save Welcomes:" << fileName;
                        } else {
                            qInfo() << "Save Welcomes failed:" << fileName;
                        }
                    }
                }
            }
        }
    }

    sendGroupMessage(gid, tr("Save Welcomes finished"));
}

// class ManagementModulePrivate

ManagementModule *ManagementModulePrivate::instance = nullptr;

ManagementModulePrivate::ManagementModulePrivate()
    : levels(Q_NULLPTR)
    , watchlist(Q_NULLPTR)
    , blacklist(Q_NULLPTR)
    , htmlDraw(Q_NULLPTR)
    , checkTimerId(-1)
{
}

ManagementModulePrivate::~ManagementModulePrivate()
{
}

LevelInfoList ManagementModulePrivate::findUsers(const QStringList &args)
{
    QListIterator<QString> i(args);
    LevelInfoList levels;
    i.toBack();

    while (i.hasPrevious()) {
        const QString &arg = i.previous();
        if (arg.startsWith(QStringLiteral("[CQ:at"))) {
            qint64 uid = arg.mid(10, arg.count() - 11).toLongLong();
            if (100000 > uid) {
                break;
            }

            bool noFound = true;
            for (const LevelInfo &li : levels) {
                if (li.uid == uid) {
                    noFound = false;
                    break;
                }
            }
            if (noFound) {
                levels.prepend(LevelInfo(uid, MasterLevel::Unknown));
            }
        } else {
            qint64 uid = arg.toLongLong();
            if (100000 > uid) {
                break;
            }

            bool noFound = true;
            for (const LevelInfo &li : levels) {
                if (li.uid == uid) {
                    noFound = false;
                    break;
                }
            }
            if (noFound) {
                levels.prepend(LevelInfo(uid, MasterLevel::Unknown));
            }
        }
    }

    return levels;
}

void ManagementModulePrivate::safetyNameCard(QString &nameCard)
{
    // 屏蔽 emoji 表情
    do {
        int s = nameCard.indexOf("[CQ:emoji");
        if (s < 0) {
            break;
        }
        int e = nameCard.indexOf(']', s);
        if (e < 0) {
            break;
        }

        nameCard.replace(s, e - s + 1, "{?}");
    } while (true);
}

void ManagementModulePrivate::formatNameCard(QString &nameCard)
{
    // 在这里，我们对新名片做规范化处理。
    nameCard.remove(' '); // 消除空格，不允许有空格。
    nameCard.replace("【", "["); // 替换全角方括号，用半角方括号替代。
    nameCard.replace("】", "]"); // 替换全角方括号，用半角方括号替代。
}

void ManagementModulePrivate::init(const QJsonObject &o)
{
    QJsonArray superUsers = o.value("superUsers").toArray();
    for (int i = 0; i < superUsers.count(); ++i)
        this->superUsers.insert(superUsers.at(i).toString().toLongLong());

    QJsonArray managedGroups = o.value("managedGroups").toArray();
    for (int i = 0; i < managedGroups.count(); ++i)
        this->managedGroups.insert(managedGroups.at(i).toString().toLongLong());

    QJsonArray banHongbaoGroups = o.value("banHongbaoGroups").toArray();
    for (int i = 0; i < banHongbaoGroups.count(); ++i)
        this->banHongbaoGroups.insert(banHongbaoGroups.at(i).toString().toLongLong());

    qInfo() << "superUsers" << this->superUsers;
    qInfo() << "managedGroups" << this->managedGroups;
    qInfo() << "banHongbaoGroups" << this->banHongbaoGroups;
}

void ManagementModulePrivate::saveWelcomes(const QString &id, HtmlDraw::Style style)
{
    Q_Q(ManagementModule);

    QFile file(QString(":/Welcomes/%1.utf8.txt").arg(id));
    if (file.open(QFile::ReadOnly)) {
        auto htmlText = QString::fromUtf8(file.readAll());

        QPixmap image = htmlDraw->drawText(htmlText, style, 400, 0);
        if (image.save(q->imgFilePath(QString("Welcomes/%1.png").arg(id)), "PNG")) {
            qInfo() << "Output 1";
        } else {
            qInfo() << "Output 2";
        }
    }

    qInfo() << "failed";
}
