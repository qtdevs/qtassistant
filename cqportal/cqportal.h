#ifndef CQPORTAL_H
#define CQPORTAL_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "cqglobalevents.h"
#include "cqpersoninfo.h"
#include "cqmemberinfo.h"

class CqPortalPrivate;
class CqPortal : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CqPortal)

protected:
    CqPortal(CqPortalPrivate &dd, QObject *parent = Q_NULLPTR);
protected:
    QScopedPointer<CqPortalPrivate> d_ptr;
public:
    virtual ~CqPortal();

public:
    static QByteArray convert(const QString &str);
    static QString convert(const char *gbkStr);
    static QString convert(const QByteArray &str);

public:
    static QString cqAt(qint64 uid);

    static QString cqImage(const QString &name);

public:
    qint64 currentId() const;
    QString basePath() const;

public:
    QString appFilePath(const char *srcName) const;
    QString appFilePath(const QString &name) const;
    QString usrFilePath(const char *srcName) const;
    QString usrFilePath(const QString &name) const;

public:
    virtual bool privateMessageEventFilter(const MessageEvent &ev);
    virtual bool groupMessageEventFilter(const MessageEvent &ev);
    virtual bool discussMessageEventFilter(const MessageEvent &ev);

    virtual bool masterChangeEventFilter(const MasterChangeEvent &ev);
    virtual bool friendRequestEventFilter(const FriendRequestEvent &ev);
    virtual bool groupRequestEventFilter(const GroupRequestEvent &ev);

    virtual bool friendAddEventFilter(const FriendAddEvent &ev);
    virtual bool memberJoinEventFilter(const MemberJoinEvent &ev);
    virtual bool memberLeaveEventFilter(const MemberLeaveEvent &ev);

public:
    enum Result {
        NoError = 0,
        Unknown = 1
    };

public:
    Result sendPrivateMessage(qint64 uid, const char *gbkMsg) const;
    Result sendGroupMessage(qint64 gid, const char *gbkMsg) const;
    Result sendDiscussMessage(qint64 did, const char *gbkMsg) const;

    Result sendPrivateMessage(qint64 uid, const QString &msg) const;
    Result sendGroupMessage(qint64 gid, const QString &msg) const;
    Result sendDiscussMessage(qint64 did, const QString &msg) const;

    Result banGroupMember(qint64 gid, qint64 uid, int duration);
    Result kickGroupMember(qint64 gid, qint64 uid, bool lasting);
    Result adminGroupMember(qint64 gid, qint64 uid, bool enabled);

    Result renameGroupMember(qint64 gid, qint64 uid, const char *gbkNewNameCard);
    Result renameGroupMember(qint64 gid, qint64 uid, const QString &newNameCard);

public:
    Result acceptRequest(const char *gbkTag);
    Result rejectRequest(const char *gbkTag);

    Result acceptRequest(qint32 type, const char *gbkTag);
    Result rejectRequest(qint32 type, const char *gbkTag);

public:
    Result leaveGroup(qint64 gid);
    Result leaveDiscuss(qint64 did);

    Result mute(qint64 gid, bool muted);

public:
    CqPersonInfo personInfo(qint64 uid, bool cached = true);
    CqMemberInfo memberInfo(qint64 gid, qint64 uid, bool cached = true);

public:
    QString saveImage(const QPixmap &data) const;
    QPixmap loadImage(const QString &name) const;
};

#endif // CQPORTAL_H
