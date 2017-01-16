/*! \class CqPortal
 * \brief 插件入口类
 */

#include "cqportal.h"
#include "cqportal_p.h"

#include <QDir>
#include <QPixmap>
#include <QStringBuilder>
#include <QUuid>

#include "cqsqlite_p.h"
#include "cqapi/cqapi.h"

// class CqPortal

CqPortal::CqPortal(CqPortalPrivate &dd, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
    Q_D(CqPortal);

    Q_ASSERT(CqPortalPrivate::instance == Q_NULLPTR);
    CqPortalPrivate::instance = this;
    d->q_ptr = this;

    d->currentId = CQ_getLoginQQ(d->accessToken);

    QString path = convert(CQ_getAppDirectory(d->accessToken));
    d->basePath = QDir::cleanPath(path);
    d->imagePath = QDir::cleanPath(path % "/../../data/image");

    QString sqlitePath = d->basePath % '/' % QString::number(d->currentId);
    CqSqlitePrivate::basePath = QDir::cleanPath(sqlitePath);
    QDir().mkpath(CqSqlitePrivate::basePath);
}

CqPortal::~CqPortal()
{
    Q_ASSERT(CqPortalPrivate::instance == this);
    CqPortalPrivate::instance = Q_NULLPTR;
}

QByteArray CqPortal::convert(const QString &str)
{
    return str.toLocal8Bit();
}

QString CqPortal::convert(const char *gbkStr)
{
    return QString::fromLocal8Bit(gbkStr);
}

QString CqPortal::convert(const QByteArray &str)
{
    return QString::fromLocal8Bit(str);
}

QString CqPortal::at(qint64 uid)
{
    return QString::fromLatin1("[CQ:at,qq=%1]").arg(uid);
}

QString CqPortal::image(const QString &name)
{
    return QString::fromLatin1("[CQ:image,file=%1]").arg(name);
}

qint64 CqPortal::currentId() const
{
    return d_ptr->currentId;
}

QString CqPortal::basePath() const
{
    return d_ptr->basePath;
}

QString CqPortal::appFilePath(const char *srcName) const
{
    return QDir::cleanPath(d_ptr->basePath % '/' % QString::fromUtf8(srcName));
}

QString CqPortal::appFilePath(const QString &name) const
{
    return QDir::cleanPath(d_ptr->basePath % '/' % name);
}

QString CqPortal::usrFilePath(const char *srcName) const
{
    return QDir::cleanPath(d_ptr->basePath % '/' % QString::number(d_ptr->currentId) % '/' % QString::fromUtf8(srcName));
}

QString CqPortal::usrFilePath(const QString &name) const
{
    return QDir::cleanPath(d_ptr->basePath % '/' % QString::number(d_ptr->currentId) % '/' % name);
}

bool CqPortal::privateMessageEventFilter(const MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool CqPortal::groupMessageEventFilter(const MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool CqPortal::discussMessageEventFilter(const MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool CqPortal::masterChangeEventFilter(const MasterChangeEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool CqPortal::friendRequestEventFilter(const FriendRequestEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool CqPortal::groupRequestEventFilter(const GroupRequestEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool CqPortal::friendAddEventFilter(const FriendAddEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool CqPortal::memberJoinEventFilter(const MemberJoinEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool CqPortal::memberLeaveEventFilter(const MemberLeaveEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

CqPortal::Result CqPortal::sendPrivateMessage(qint64 uid, const char *gbkMsg) const
{
    return CqPortalPrivate::result(CQ_sendPrivateMsg(CqPortalPrivate::accessToken, uid, gbkMsg));
}

CqPortal::Result CqPortal::sendGroupMessage(qint64 gid, const char *gbkMsg) const
{
    return CqPortalPrivate::result(CQ_sendGroupMsg(CqPortalPrivate::accessToken, gid, gbkMsg));
}

CqPortal::Result CqPortal::sendDiscussMessage(qint64 did, const char *gbkMsg) const
{
    return CqPortalPrivate::result(CQ_sendDiscussMsg(CqPortalPrivate::accessToken, did, gbkMsg));
}

CqPortal::Result CqPortal::sendPrivateMessage(qint64 uid, const QString &msg) const
{
    return sendPrivateMessage(uid, convert(msg).constData());
}

CqPortal::Result CqPortal::sendGroupMessage(qint64 gid, const QString &msg) const
{
    return sendGroupMessage(gid, convert(msg).constData());
}

CqPortal::Result CqPortal::sendDiscussMessage(qint64 did, const QString &msg) const
{
    return sendDiscussMessage(did, convert(msg).constData());
}

CqPortal::Result CqPortal::banGroupMember(qint64 gid, qint64 uid, int duration)
{
    return CqPortalPrivate::result(CQ_setGroupBan(CqPortalPrivate::accessToken, gid, uid, duration));
}

CqPortal::Result CqPortal::kickGroupMember(qint64 gid, qint64 uid, bool lasting)
{
    return CqPortalPrivate::result(CQ_setGroupKick(CqPortalPrivate::accessToken, gid, uid, lasting));
}

CqPortal::Result CqPortal::adminGroupMember(qint64 gid, qint64 uid, bool enabled)
{
    return CqPortalPrivate::result(CQ_setGroupAdmin(CqPortalPrivate::accessToken, gid, uid, enabled));
}

CqPortal::Result CqPortal::renameGroupMember(qint64 gid, qint64 uid, const char *gbkNewNameCard)
{
    return CqPortalPrivate::result(CQ_setGroupCard(CqPortalPrivate::accessToken, gid, uid, gbkNewNameCard));
}

CqPortal::Result CqPortal::renameGroupMember(qint64 gid, qint64 uid, const QString &newNameCard)
{
    return CqPortalPrivate::result(CQ_setGroupCard(CqPortalPrivate::accessToken, gid, uid, convert(newNameCard).constData()));
}

CqPortal::Result CqPortal::acceptRequest(const char *gbkTag)
{
    return CqPortalPrivate::result(CQ_setFriendAddRequest(CqPortalPrivate::accessToken, gbkTag, REQUEST_ALLOW, ""));
}

CqPortal::Result CqPortal::rejectRequest(const char *gbkTag)
{
    return CqPortalPrivate::result(CQ_setFriendAddRequest(CqPortalPrivate::accessToken, gbkTag, REQUEST_DENY, ""));
}

CqPortal::Result CqPortal::acceptRequest(qint32 type, const char *gbkTag)
{
    if (1 == type) {
        return CqPortalPrivate::result(CQ_setGroupAddRequestV2(CqPortalPrivate::accessToken, gbkTag, REQUEST_GROUPADD, REQUEST_ALLOW, ""));
    } else if (2 == type) {
        return CqPortalPrivate::result(CQ_setGroupAddRequestV2(CqPortalPrivate::accessToken, gbkTag, REQUEST_GROUPINVITE, REQUEST_ALLOW, ""));
    }

    return Result::Unknown;
}

CqPortal::Result CqPortal::rejectRequest(qint32 type, const char *gbkTag)
{
    if (1 == type) {
        return CqPortalPrivate::result(CQ_setGroupAddRequestV2(CqPortalPrivate::accessToken, gbkTag, REQUEST_GROUPADD, REQUEST_DENY, ""));
    } else if (2 == type) {
        return CqPortalPrivate::result(CQ_setGroupAddRequestV2(CqPortalPrivate::accessToken, gbkTag, REQUEST_GROUPINVITE, REQUEST_DENY, ""));
    }

    return Result::Unknown;
}

CqPortal::Result CqPortal::leaveGroup(qint64 gid)
{
    return CqPortalPrivate::result(CQ_setGroupLeave(CqPortalPrivate::accessToken, gid, false));
}

CqPortal::Result CqPortal::leaveDiscuss(qint64 did)
{
    return CqPortalPrivate::result(CQ_setDiscussLeave(CqPortalPrivate::accessToken, did));
}

CqPortal::Result CqPortal::mute(qint64 gid, bool muted)
{
    return CqPortalPrivate::result(CQ_setGroupWholeBan(CqPortalPrivate::accessToken, gid, muted));
}

CqPersonInfo CqPortal::personInfo(qint64 uid, bool cached)
{
    return CqPersonInfo(CQ_getStrangerInfo(CqPortalPrivate::accessToken, uid, !cached));
}

CqMemberInfo CqPortal::memberInfo(qint64 gid, qint64 uid, bool cached)
{
    return CqMemberInfo(CQ_getGroupMemberInfoV2(CqPortalPrivate::accessToken, gid, uid, !cached));
}

QString CqPortal::saveImage(const QImage &data) const
{
    Q_D(const CqPortal);

    QString uuid = QString::fromLatin1(QUuid::createUuid().toRfc4122().toHex());
    QString name = QString("%1/%2.png").arg(d->imagePath, uuid);
    if (data.save(name, "PNG")) {
        return uuid % ".png";
    }

    return QString();
}

QImage CqPortal::loadImage(const QString &name) const
{
    Q_UNUSED(name);
    return QImage();
}

// class CqPortalPrivate

qint32 CqPortalPrivate::accessToken = -1;
CqPortal *CqPortalPrivate::instance = Q_NULLPTR;

CqPortalPrivate::CqPortalPrivate()
    : q_ptr(Q_NULLPTR)
    , currentId(0)
{
}

CqPortalPrivate::~CqPortalPrivate()
{
}

CqPortal::Result CqPortalPrivate::result(qint32 r)
{
    switch (r) {
    case 1:
        break;
    default:
        return CqPortal::Unknown;
    }

    return CqPortal::Unknown;
}
