#include "cqassistant.h"
#include "cqassistant_p.h"

#include <QDateTime>
#include <QDir>
#include <QThread>
#include <QPixmap>
#include <QStringBuilder>
#include <QTextStream>
#include <QUuid>
#include <QCoreApplication>
#include <QTranslator>

#include "datas/masterlevels.h"
#include "datas/memberwelcome.h"
#include "datas/memberblacklist.h"
#include "datas/memberdeathhouse.h"

#include "htmlfeedback/htmlfeedback.h"

CqPortal *_q_CreateInstance(QObject *parent)
{
    return new CqAssistant(parent);
}

// class CqAssistant

CqAssistant::CqAssistant(QObject *parent)
    : CqPortal(*new CqAssistantPrivate(), parent)
{
    Q_D(CqAssistant);

    QTranslator *translator = new QTranslator(qApp);
    if (translator->load("cqassistant_zh.qm", ":/translations")) {
        qApp->installTranslator(translator);
    }

    d->levels = new MasterLevels(d);
    d->welcome = new MemberWelcome(d);
    d->blacklist = new MemberBlacklist(d);
    d->deathHouse = new MemberDeathHouse(d);

    d->htmlFeedback = new HtmlFeedback(d);

    d->timerId = d->startTimer(10000);
}

CqAssistant::~CqAssistant()
{
    Q_D(CqAssistant);

    d->killTimer(d->timerId);
}

// class CqAssistantPrivate

CqAssistantPrivate::CqAssistantPrivate()
    : levels(Q_NULLPTR)
    , welcome(Q_NULLPTR)
    , blacklist(Q_NULLPTR)
    , deathHouse(Q_NULLPTR)
    , htmlFeedback(Q_NULLPTR)
{
}

CqAssistantPrivate::~CqAssistantPrivate()
{
}

void CqAssistantPrivate::timerEvent(QTimerEvent *)
{
    Q_Q(CqAssistant);

    auto welcome = this->welcome->welcome();
    if (!welcome.isEmpty()) {
        qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
        QHashIterator<Member, qint64> i(welcome);
        while (i.hasNext()) {
            i.next();
            if ((i.value() + 1800000) < now) {
                this->welcome->removeMember(i.key().first, i.key().second);
                q->kickGroupMember(i.key().first, i.key().second, false);
                QString msg = "Killed: " + QString::number(i.key().second);
                q->sendGroupMessage(i.key().first, msg);
            }
        }
    }

    auto deaths = deathHouse->deathHouse();
    if (!deaths.isEmpty()) {
        qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
        QHashIterator<Member, qint64> i(deaths);
        while (i.hasNext()) {
            i.next();
            if ((i.value() + 360000) < now) {
                deathHouse->removeMember(i.key().first, i.key().second);
                q->kickGroupMember(i.key().first, i.key().second, false);
                QString msg = "Killed: " + QString::number(i.key().second);
                q->sendGroupMessage(i.key().first, msg);
            }
        }
    }
}

LevelInfoList CqAssistantPrivate::findUsers(const QStringList &args) const
{
    QListIterator<QString> i(args);
    LevelInfoList liList;
    i.toBack();
    while (i.hasPrevious()) {
        const QString &arg = i.previous();
        if (arg.startsWith(QStringLiteral("[CQ:at"))) {
            qint64 uid = arg.mid(10, arg.count() - 11).toLongLong();
            if (0 == uid) {
                break;
            }

            bool noFound = true;
            for (const LevelInfo &li : liList) {
                if (li.uid == uid) {
                    noFound = false;
                    break;
                }
            }
            if (noFound) {
                liList << LevelInfo(uid, MasterLevel::Unknown);
            }
        } else {
            qint64 uid = arg.toLongLong();
            if (0 == uid) {
                break;
            }

            bool noFound = true;
            for (const LevelInfo &li : liList) {
                if (li.uid == uid) {
                    noFound = false;
                    break;
                }
            }
            if (noFound) {
                liList << LevelInfo(uid, MasterLevel::Unknown);
            }
        }
    }

    return liList;
}

void CqAssistantPrivate::permissionDenied(qint64 gid, qint64 uid, MasterLevel level, const QString &reason)
{
    Q_UNUSED(uid);

    Q_Q(CqAssistant);

    QString content = reason.isEmpty() ? tr("As %1, you have no rights.").arg(MasterLevels::levelName(level)) : reason;
    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(tr("Permission Denied"), content);

    QPixmap feedback = htmlFeedback->drawDanger(html, 400);
    QString fileName = q->saveImage(feedback);
    q->sendGroupMessage(gid, q->cqImage(fileName));
}

void CqAssistantPrivate::showPrompt(qint64 gid, const QString &title, const QString &content)
{
    Q_Q(CqAssistant);

    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(title, content);

    QPixmap feedback = htmlFeedback->drawPrompt(html, 400);
    QString fileName = q->saveImage(feedback);
    q->sendGroupMessage(gid, q->cqImage(fileName));
}

void CqAssistantPrivate::showSuccess(qint64 gid, const QString &title, const QString &content)
{
    Q_Q(CqAssistant);

    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(title, content);

    QPixmap feedback = htmlFeedback->drawSuccess(html, 400);
    QString fileName = q->saveImage(feedback);
    q->sendGroupMessage(gid, q->cqImage(fileName));
}

void CqAssistantPrivate::showPrimaryList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Primary);
}

void CqAssistantPrivate::showDangerList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Danger);
}

void CqAssistantPrivate::showWarningList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Warning);
}

void CqAssistantPrivate::showPromptList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Prompt);
}

void CqAssistantPrivate::showSuccessList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Success);
}

void CqAssistantPrivate::feedbackList(qint64 gid, const QString &title, const LevelInfoList &members, bool level, HtmlFeedback::Style style)
{
    Q_Q(CqAssistant);

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
                CqMemberInfo mi = q->memberInfo(gid, li.uid);
                ds << "<p class=\"c\">";
                if (level) {
                    ds << MasterLevels::levelName(li.level) << tr(": ");
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
