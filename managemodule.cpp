#include "managemodule.h"
#include "managemodule_p.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QPixmap>
#include <QStringBuilder>
#include <QTextStream>
#include <QTranslator>
#include <QUuid>

#include "sqldatas/masterlevels.h"
#include "sqldatas/memberwelcome.h"
#include "sqldatas/memberblacklist.h"
#include "sqldatas/memberdeathhouse.h"

#include "htmlfeedback/htmlfeedback.h"

#include "searchmodule.h"
#include "donatemodule.h"

// class ManageModule

ManageModule::ManageModule(CqEngine *engine)
    : CqModule(engine)
    , d_ptr(new ManageModulePrivate())
{
    d_ptr->q_ptr = this;

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

//    connect(SearchModule::instance(), &SearchModule::groupSearchResult,
//            this, &ManageModule::groupSearchResult);
}

ManageModule::~ManageModule()
{
    Q_D(ManageModule);

    killTimer(d->checkTimerId);
}

void ManageModule::timerEvent(QTimerEvent *)
{
    Q_D(ManageModule);

    // 检查新手名单。
    do {
        MemberList members;
        d->welcome->expiredMembers(members);
        for (const auto &member : members) {
            CqMemberInfo mi = memberInfo(member.first, member.second, false);
            if (mi.isValid() && mi.lastSent().isNull()) {
                kickGroupMember(member.first, member.second, false);
            }
        }
    } while (false);

    // 检查踢出名单。
    do {
        MemberList members;
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
                CqMemberInfo mi = memberInfo(gid, li.uid);
                if (mi.isValid()) {
                    if (!mi.nameCard().isEmpty()) {
                        ds << mi.nameCard();
                    } else {
                        ds << mi.nickName();
                    }
                } else {
                    CqPersonInfo pi = personInfo(li.uid);
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

void ManageModule::groupSearchResult(qint64 gid, qint64 uid, const QString &key, const QJsonObject &result)
{
    QString msg;
    QTextStream ts(&msg);

    ts << at(uid) << endl;

    if (result.contains("error_type")) {
        ;
    } else {
        QJsonArray topics = result.value("topics").toArray();
        if (topics.count() == 0) {
            ts << QString::fromUtf8("很抱歉，没有找到与\"") << key << QString::fromUtf8("\"相关的主题。");
        } else {
            ts << QString::fromUtf8("关键词: ") << key << endl;
            ts << endl;

            for (int i = 0; i < topics.count() && i < 5; ++i) {
                if (i != 0) { ts << endl; }
                QJsonObject topic = topics.at(i).toObject();
                ts << QString::fromUtf8("主题: ") << topic.value("fancy_title").toString() << endl;
                auto lastPostedAt = QDateTime::fromString(topic.value("last_posted_at").toString(), Qt::ISODate);
                int year = lastPostedAt.date().year();
                int month = lastPostedAt.date().month();
                int day = lastPostedAt.date().day();
                ts << QString::fromUtf8("%1年%2月%3日 ").arg(year).arg(month, 2, 10, QLatin1Char('0')).arg(day, 2, 10, QLatin1Char('0'));
                ts << "https://qtdevs.org/t/topic/" << topic.value("id").toInt();
            }
        }
    }

    ts.flush();

    sendGroupMessage(gid, msg);
}

void ManageModule::qtdevsSearch(const MessageEvent &ev, const QStringList &args)
{
    Q_UNUSED(ev);
    Q_UNUSED(args);

    return;
//    if (args.isEmpty()) {
//    } else {
//        SearchModule::instance()->groupSearch(ev.from, ev.sender, args.join(" "));
//    }
}

// class ManageModulePrivate

ManageModulePrivate::ManageModulePrivate()
    : q_ptr(nullptr)
    , levels(Q_NULLPTR)
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
