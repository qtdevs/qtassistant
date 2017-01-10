#include "cqassistant.h"
#include "cqassistant_p.h"

#include <QDir>
#include <QThread>
#include <QStringBuilder>

#include "cqapi/cqapi.h"

#include "datas/masterlevels.h"
#include "datas/memberwelcome.h"
#include "datas/memberblacklist.h"
#include "datas/memberdeathhouse.h"

CqCode cqCode(qint32 code)
{
    switch (code) {
    case 1:
        break;
    default:
        return CqCode::Unknown;
    }

    return CqCode::Unknown;
}

// class CqAssistant

CqAssistant::CqAssistant(qint32 token, QObject *parent)
    : QObject(parent)
    , d_ptr(new CqAssistantPrivate())
{
    d_ptr->q_ptr = this;
    d_ptr->token = token;
    d_ptr->initThread();

    d_ptr->currentId = CQ_getLoginQQ(token);
    QString path = conv(CQ_getAppDirectory(token));
    d_ptr->basePath = QDir::cleanPath(path);
}

CqAssistant::~CqAssistant()
{
    Q_D(CqAssistant);

    if (d->agent->isRunning()) {
        d->agent->quit();
        d->agent->wait();
    }
}

QByteArray CqAssistant::conv(const QString &str)
{
    return str.toLocal8Bit();
}

QString CqAssistant::conv(const char *gbkStr)
{
    return QString::fromLocal8Bit(gbkStr);
}

QString CqAssistant::conv(const QByteArray &str)
{
    return QString::fromLocal8Bit(str);
}

qint64 CqAssistant::currentId() const
{
    return d_ptr->currentId;
}

QString CqAssistant::basePath() const
{
    return d_ptr->basePath;
}

QString CqAssistant::appFilePath(const char *srcName) const
{
    if ((srcName[0] == '/') || (srcName[0] == '\\')) {
        return QDir::cleanPath(d_ptr->basePath % QString::fromUtf8(srcName));
    }

    return QDir::cleanPath(d_ptr->basePath % QStringLiteral("/") % QString::fromUtf8(srcName));
}

QString CqAssistant::appFilePath(const QString &name) const
{
    if (name.startsWith(QLatin1Char('/')) || name.startsWith(QLatin1Char('\\'))) {
        return QDir::cleanPath(d_ptr->basePath % name);
    }

    return QDir::cleanPath(d_ptr->basePath % QStringLiteral("/") % name);
}

QString CqAssistant::usrFilePath(const char *srcName) const
{
    QString currentId = QString::number(d_ptr->currentId);

    if ((srcName[0] == '/') || (srcName[0] == '\\')) {
        return QDir::cleanPath(d_ptr->basePath % "/" % currentId % QString::fromUtf8(srcName));
    }

    return QDir::cleanPath(d_ptr->basePath % "/" % currentId % "/" % QString::fromUtf8(srcName));
}

QString CqAssistant::usrFilePath(const QString &name) const
{
    QString currentId = QString::number(d_ptr->currentId);

    if (name.startsWith(QLatin1Char('/')) || name.startsWith(QLatin1Char('\\'))) {
        return QDir::cleanPath(d_ptr->basePath % "/" % currentId % name);
    }

    return QDir::cleanPath(d_ptr->basePath % "/" % currentId % "/" % name);
}

CqCode CqAssistant::sendPrivateMessage(qint64 uid, const char *gbkMsg)
{
    return cqCode(CQ_sendPrivateMsg(d_ptr->token, uid, gbkMsg));
}

CqCode CqAssistant::sendGroupMessage(qint64 gid, const char *gbkMsg)
{
    return cqCode(CQ_sendGroupMsg(d_ptr->token, gid, gbkMsg));
}

CqCode CqAssistant::sendDiscussMessage(qint64 did, const char *gbkMsg)
{
    return cqCode(CQ_sendDiscussMsg(d_ptr->token, did, gbkMsg));
}

CqCode CqAssistant::sendPrivateMessage(qint64 uid, const QString &msg)
{
    return sendPrivateMessage(uid, conv(msg).constData());
}

CqCode CqAssistant::sendGroupMessage(qint64 gid, const QString &msg)
{
    return sendGroupMessage(gid, conv(msg).constData());
}

CqCode CqAssistant::sendDiscussMessage(qint64 did, const QString &msg)
{
    return sendDiscussMessage(did, conv(msg).constData());
}

CqCode CqAssistant::banGroupMember(qint64 gid, qint64 uid, int duration)
{
    return cqCode(CQ_setGroupBan(d_ptr->token, gid, uid, duration));
}

CqCode CqAssistant::kickGroupMember(qint64 gid, qint64 uid, bool lasting)
{
    return cqCode(CQ_setGroupKick(d_ptr->token, gid, uid, lasting));
}

CqCode CqAssistant::adminGroupMember(qint64 gid, qint64 uid, bool enabled)
{
    return cqCode(CQ_setGroupAdmin(d_ptr->token, gid, uid, enabled));
}

CqCode CqAssistant::renameGroupMember(qint64 gid, qint64 uid, const char *gbkNewNickName)
{
    return cqCode(CQ_setGroupCard(d_ptr->token, gid, uid, gbkNewNickName));
}

CqCode CqAssistant::renameGroupMember(qint64 gid, qint64 uid, const QString &newNickName)
{
    return cqCode(CQ_setGroupCard(d_ptr->token, gid, uid, conv(newNickName).constData()));
}

CqCode CqAssistant::acceptRequest(const char *gbkTag)
{
    return cqCode(CQ_setFriendAddRequest(d_ptr->token, gbkTag, REQUEST_ALLOW, ""));
}

CqCode CqAssistant::rejectRequest(const char *gbkTag)
{
    return cqCode(CQ_setFriendAddRequest(d_ptr->token, gbkTag, REQUEST_DENY, ""));
}

CqCode CqAssistant::acceptRequest(qint32 type, const char *gbkTag)
{
    if (1 == type) {
        return cqCode(CQ_setGroupAddRequestV2(d_ptr->token, gbkTag, REQUEST_GROUPADD, REQUEST_ALLOW, ""));
    } else if (2 == type) {
        return cqCode(CQ_setGroupAddRequestV2(d_ptr->token, gbkTag, REQUEST_GROUPINVITE, REQUEST_ALLOW, ""));
    }

    return CqCode::Unknown;
}

CqCode CqAssistant::rejectRequest(qint32 type, const char *gbkTag)
{
    if (1 == type) {
        return cqCode(CQ_setGroupAddRequestV2(d_ptr->token, gbkTag, REQUEST_GROUPADD, REQUEST_DENY, ""));
    } else if (2 == type) {
        return cqCode(CQ_setGroupAddRequestV2(d_ptr->token, gbkTag, REQUEST_GROUPINVITE, REQUEST_DENY, ""));
    }

    return CqCode::Unknown;
}

CqCode CqAssistant::leaveGroup(qint64 gid)
{
    return cqCode(CQ_setGroupLeave(d_ptr->token, gid, false));
}

CqCode CqAssistant::leaveDiscuss(qint64 did)
{
    return cqCode(CQ_setDiscussLeave(d_ptr->token, did));
}

CqCode CqAssistant::mute(qint64 gid, bool muted)
{
    return cqCode(CQ_setGroupWholeBan(d_ptr->token, gid, muted));
}

QString CqAssistant::msgAt(qint64 id)
{
    return QString::fromLatin1("[CQ:at,qq=%1]").arg(id);
}

/*
const char *CqAssistant::memberInfo(qint64 gid, qint64 uid, bool cache)
{
    return CQ_getGroupMemberInfoV2(d_ptr->token, gid, uid, !cache);
}

const char *CqAssistant::strangerInfo(qint64 id, bool cache)
{
    return CQ_getStrangerInfo(d_ptr->token, id, !cache);
}
*/

// class CqAssistantPrivate

CqAssistantPrivate::CqAssistantPrivate()
    : q_ptr(Q_NULLPTR)
    , agent(Q_NULLPTR)
    , token(-1)
    , currentId(0)
    , levels(Q_NULLPTR)
    , welcome(Q_NULLPTR)
    , blacklist(Q_NULLPTR)
    , deathHouse(Q_NULLPTR)
{
}

CqAssistantPrivate::~CqAssistantPrivate()
{
}

void CqAssistantPrivate::initThread()
{
    agent = new QThread(this);

    moveToThread(agent);
    connect(agent, &QThread::started,
            this, &CqAssistantPrivate::initialize);

    agent->start();
}

void CqAssistantPrivate::initialize()
{
    if (currentId == 0) {
        return;
    }

    QString path = basePath % "/" % QString::number(currentId);
    SqlData::initPath(QDir::cleanPath(path));
    QDir().mkpath(path);

    levels = new MasterLevels(this);
    welcome = new MemberWelcome(this);
    blacklist = new MemberBlacklist(this);
    deathHouse = new MemberDeathHouse(this);

    startTimer(10000);
}

#include <QDateTime>

void CqAssistantPrivate::timerEvent(QTimerEvent *)
{
    Q_Q(CqAssistant);

    auto deaths = deathHouse->deathHouse();
    if (!deaths.isEmpty()) {
        qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
        QHashIterator<Member, qint64> i(deaths);
        while (i.hasNext()) {
            i.next();
            if ((i.value() + 360) < now) {
                deathHouse->removeMember(i.key().first, i.key().second);
                q->kickGroupMember(i.key().first, i.key().second, false);
                QString msg = "Killed: " + QString::number(i.key().second);
                q->sendGroupMessage(i.key().first, msg);
            }
        }
    }
}
