#include "managemodule.h"
#include "managemodule_p.h"

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

#include "sqldatas/masterlevels.h"
#include "sqldatas/memberwelcome.h"
#include "sqldatas/memberblacklist.h"
#include "sqldatas/memberdeathhouse.h"

#include "htmlfeedback/htmlfeedback.h"
#include "ManagementFilters.h"

#include "searchmodule.h"
#include "donatemodule.h"

// class ManageModule

ManageModule::ManageModule(CoolQ::ServicePortal *engine)
    : CoolQ::ServiceModule(*new ManageModulePrivate(), engine)
{
    Q_ASSERT(ManageModulePrivate::instance == nullptr);
    ManageModulePrivate::instance = this;

    Q_D(ManageModule);

    QTranslator *translator = new QTranslator(qApp);
    if (translator->load("qtassistant_zh.qm", ":/translations")) {
        qApp->installTranslator(translator);
    }

    d->levels = new MasterLevels(this);
    d->welcome = new MemberWelcome(this);
    d->blacklist = new MemberBlacklist(this);
    d->deathHouse = new MemberDeathHouse(this);

    d->htmlFeedback = new HtmlFeedback(this);

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

    new RestartComputer(this);

    new CreateStartupShortcut(this);
    new DeleteStartupShortcut(this);
}

ManageModule::~ManageModule()
{
    Q_D(ManageModule);

    ManageModulePrivate::instance = nullptr;

    killTimer(d->checkTimerId);
}

ManageModule *ManageModule::instance()
{
    return ManageModulePrivate::instance;
}

MasterLevel ManageModule::level(qint64 gid, qint64 uid) const
{
    Q_D(const ManageModule);

    return d->levels->level(gid, uid);
}

void ManageModule::timerEvent(QTimerEvent *)
{
    Q_D(ManageModule);

    // 检查新手名单。
    do {
        CoolQ::MemberList members;
        d->welcome->expiredMembers(members);
        for (const auto &member : members) {
            CoolQ::MemberInfo mi = memberInfo(member.first, member.second, false);
            if (mi.isValid() && mi.lastSent().isNull()) {
                kickGroupMember(member.first, member.second, false);
            }
        }
    } while (false);

    // 检查踢出名单。
    do {
        CoolQ::MemberList members;
        d->deathHouse->expiredMembers(members);
        for (const auto &member : members) {
            kickGroupMember(member.first, member.second, false);
        }
    } while (false);
}

void ManageModule::permissionDenied(qint64 gid, qint64 uid, MasterLevel level, const QString &reason)
{
    Q_UNUSED(uid);

    Q_D(ManageModule);

    QString content = reason.isEmpty() ? tr("作为一个%1，你没有权限执行此操作。").arg(MasterLevels::levelName(level)) : reason;
    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(tr("没有相关权限"), content);

    QPixmap feedback = d->htmlFeedback->drawDanger(html, 400);
    QString fileName = saveImage(feedback);
    sendGroupMessage(gid, cqImage(fileName));
}

void ManageModule::showPrimary(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlFeedback::Primary);
}

void ManageModule::showDanger(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlFeedback::Danger);
}

void ManageModule::showWarning(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlFeedback::Warning);
}

void ManageModule::showPrompt(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlFeedback::Prompt);
}

void ManageModule::showSuccess(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlFeedback::Success);
}

void ManageModule::feedback(qint64 gid, const QString &title, const QString &content, HtmlFeedback::Style style)
{
    Q_D(ManageModule);

    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(title, content);

    QPixmap feedback = d->htmlFeedback->draw(html, 400, style);
    QString fileName = saveImage(feedback);
    sendGroupMessage(gid, cqImage(fileName));
}

void ManageModule::showPrimaryList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Primary);
}

void ManageModule::showDangerList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Danger);
}

void ManageModule::showWarningList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Warning);
}

void ManageModule::showPromptList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Prompt);
}

void ManageModule::showSuccessList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Success);
}

void ManageModule::feedbackList(qint64 gid, const QString &title, const LevelInfoList &members, bool level, HtmlFeedback::Style style)
{
    Q_D(ManageModule);

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

        QPixmap feedback = d->htmlFeedback->draw(html, 400, style);
        QString fileName = saveImage(feedback);
        sendGroupMessage(gid, cqImage(fileName));
    }
}

void ManageModule::showWelcomes(qint64 gid, qint64 uid)
{
    Q_D(ManageModule);

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

void ManageModule::saveWelcomes(qint64 gid, qint64 uid)
{
    Q_D(ManageModule);

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
                        auto styleEnum = QMetaEnum::fromType<HtmlFeedback::Style>();
                        auto style = styleEnum.keysToValue(nameParts.at(1).toLatin1());
                        auto image = d->htmlFeedback->draw(html, 400, (HtmlFeedback::Style)style);
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

// class ManageModulePrivate

ManageModule *ManageModulePrivate::instance = nullptr;

ManageModulePrivate::ManageModulePrivate()
    : levels(Q_NULLPTR)
    , welcome(Q_NULLPTR)
    , blacklist(Q_NULLPTR)
    , deathHouse(Q_NULLPTR)
    , htmlFeedback(Q_NULLPTR)
    , checkTimerId(-1)
{
}

ManageModulePrivate::~ManageModulePrivate()
{
}

LevelInfoList ManageModulePrivate::findUsers(const QStringList &args)
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

void ManageModulePrivate::safetyNameCard(QString &nameCard)
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

void ManageModulePrivate::formatNameCard(QString &nameCard)
{
    // 在这里，我们对新名片做规范化处理。
    nameCard.remove(' '); // 消除空格，不允许有空格。
    nameCard.replace("【", "["); // 替换全角方括号，用半角方括号替代。
    nameCard.replace("】", "]"); // 替换全角方括号，用半角方括号替代。
}

void ManageModulePrivate::init(const QJsonObject &o)
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

void ManageModulePrivate::saveWelcomes(const QString &id, HtmlFeedback::Style style)
{
    Q_Q(ManageModule);

    QFile file(QString(":/Welcomes/%1.utf8.txt").arg(id));
    if (file.open(QFile::ReadOnly)) {
        auto htmlText = QString::fromUtf8(file.readAll());

        QString path = q->imgFilePath("Welcomes");
        QPixmap image = htmlFeedback->draw(htmlText, 400, style);
        if (image.save(q->imgFilePath(QString("Welcomes/%1.png").arg(id)), "PNG")) {
            qInfo() << "Output 1";
        } else {
            qInfo() << "Output 2";
        }
    }

    qInfo() << "failed";
}
