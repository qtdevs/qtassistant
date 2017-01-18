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
            kickGroupMember(member.first, member.second, false);
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

// class QtAssistantPrivate

QtAssistantPrivate::QtAssistantPrivate()
    : levels(Q_NULLPTR)
    , welcome(Q_NULLPTR)
    , blacklist(Q_NULLPTR)
    , deathHouse(Q_NULLPTR)
    , htmlFeedback(Q_NULLPTR)
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

void QtAssistantPrivate::permissionDenied(qint64 gid, qint64 uid, MasterLevel level, const QString &reason)
{
    Q_UNUSED(uid);

    Q_Q(QtAssistant);

    QString content = reason.isEmpty() ? q->tr("As %1, you have no rights.").arg(MasterLevels::levelName(level)) : reason;
    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(q->tr("Permission Denied"), content);

    QPixmap feedback = htmlFeedback->drawDanger(html, 400);
    QString fileName = q->saveImage(feedback);
    q->sendGroupMessage(gid, q->cqImage(fileName));
}

void QtAssistantPrivate::showWarning(qint64 gid, const QString &title, const QString &content)
{
    Q_Q(QtAssistant);

    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(title, content);

    QPixmap feedback = htmlFeedback->drawWarning(html, 400);
    QString fileName = q->saveImage(feedback);
    q->sendGroupMessage(gid, q->cqImage(fileName));
}

void QtAssistantPrivate::showPrompt(qint64 gid, const QString &title, const QString &content)
{
    Q_Q(QtAssistant);

    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(title, content);

    QPixmap feedback = htmlFeedback->drawPrompt(html, 400);
    QString fileName = q->saveImage(feedback);
    q->sendGroupMessage(gid, q->cqImage(fileName));
}

void QtAssistantPrivate::showSuccess(qint64 gid, const QString &title, const QString &content)
{
    Q_Q(QtAssistant);

    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(title, content);

    QPixmap feedback = htmlFeedback->drawSuccess(html, 400);
    QString fileName = q->saveImage(feedback);
    q->sendGroupMessage(gid, q->cqImage(fileName));
}

void QtAssistantPrivate::showPrimaryList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Primary);
}

void QtAssistantPrivate::showDangerList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Danger);
}

void QtAssistantPrivate::showWarningList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Warning);
}

void QtAssistantPrivate::showPromptList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Prompt);
}

void QtAssistantPrivate::showSuccessList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Success);
}

void QtAssistantPrivate::feedbackList(qint64 gid, const QString &title, const LevelInfoList &members, bool level, HtmlFeedback::Style style)
{
    Q_Q(QtAssistant);

    for (int i = 0, part = 0; i < members.count();) {
        QString html;
        do {
            int cc = i + 4;
            QTextStream ds(&html);
            ds << "<html><body><span class=\"t\">" << title;
            if (members.count() > 5) {
                ds << q->tr("(Part %1)").arg(++part);
            }
            ds << "</span><div>";
            for (; i < members.count(); ++i) {
                const LevelInfo &li = members.at(i);
                CqMemberInfo mi = q->memberInfo(gid, li.uid);
                ds << "<p class=\"c\">";
                if (level) {
                    ds << MasterLevels::levelName(li.level) << q->tr(": ");
                }
                if (!mi.nameCard().isEmpty()) {
                    ds << mi.nameCard() << "</p>";
                } else {
                    ds << mi.nickName() << "</p>";
                }
                if (i == cc) {
                    ++i;
                    break;
                }
            }
            ds << "</div></body></html>";
        } while (false);

        QPixmap feedback = htmlFeedback->draw(html, 400, style);
        QString fileName = q->saveImage(feedback);
        q->sendGroupMessage(gid, q->cqImage(fileName));
    }
}
