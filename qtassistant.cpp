#include "qtassistant.h"
#include "qtassistant_p.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
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

// class QtAssistant

QtAssistant::QtAssistant(QObject *parent)
    : CqPortal(*new QtAssistantPrivate(), parent)
{
    Q_D(QtAssistant);

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
}

QtAssistant::~QtAssistant()
{
    Q_D(QtAssistant);

    killTimer(d->checkTimerId);
}

void QtAssistant::timerEvent(QTimerEvent *)
{
    Q_D(QtAssistant);

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

void QtAssistant::permissionDenied(qint64 gid, qint64 uid, MasterLevel level, const QString &reason)
{
    Q_UNUSED(uid);

    Q_D(QtAssistant);

    QString content = reason.isEmpty() ? tr("作为一个%1，你没有权限执行此操作。").arg(MasterLevels::levelName(level)) : reason;
    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(tr("没有相关权限"), content);

    QPixmap feedback = d->htmlFeedback->drawDanger(html, 400);
    QString fileName = saveImage(feedback);
    sendGroupMessage(gid, cqImage(fileName));
}

void QtAssistant::showPrimary(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlFeedback::Primary);
}

void QtAssistant::showDanger(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlFeedback::Danger);
}

void QtAssistant::showWarning(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlFeedback::Warning);
}

void QtAssistant::showPrompt(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlFeedback::Prompt);
}

void QtAssistant::showSuccess(qint64 gid, const QString &title, const QString &content)
{
    feedback(gid, title, content, HtmlFeedback::Success);
}

void QtAssistant::feedback(qint64 gid, const QString &title, const QString &content, HtmlFeedback::Style style)
{
    Q_D(QtAssistant);

    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(title, content);

    QPixmap feedback = d->htmlFeedback->draw(html, 400, style);
    QString fileName = saveImage(feedback);
    sendGroupMessage(gid, cqImage(fileName));
}

void QtAssistant::showPrimaryList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Primary);
}

void QtAssistant::showDangerList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Danger);
}

void QtAssistant::showWarningList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Warning);
}

void QtAssistant::showPromptList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Prompt);
}

void QtAssistant::showSuccessList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Success);
}

void QtAssistant::feedbackList(qint64 gid, const QString &title, const LevelInfoList &members, bool level, HtmlFeedback::Style style)
{
    Q_D(QtAssistant);

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

// class QtAssistantPrivate

QtAssistantPrivate::QtAssistantPrivate()
    : levels(Q_NULLPTR)
    , welcome(Q_NULLPTR)
    , blacklist(Q_NULLPTR)
    , deathHouse(Q_NULLPTR)
    , htmlFeedback(Q_NULLPTR)
    , checkTimerId(-1)
{
}

QtAssistantPrivate::~QtAssistantPrivate()
{
}

LevelInfoList QtAssistantPrivate::findUsers(const QStringList &args)
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

void QtAssistantPrivate::safetyNameCard(QString &nameCard)
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

void QtAssistantPrivate::formatNameCard(QString &nameCard)
{
    // 在这里，我们对新名片做规范化处理。
    nameCard.remove(' '); // 消除空格，不允许有空格。
    nameCard.replace("【", "["); // 替换全角方括号，用半角方括号替代。
    nameCard.replace("】", "]"); // 替换全角方括号，用半角方括号替代。

}
