#ifndef CQASSISTANT_H
#define CQASSISTANT_H

#include <QObject>
#include "cqaevents.h"
#include "memberinfo.h"
#include "personinfo.h"

#include "datas/masterlevels.h"

enum class CqCode {
    NoError = 0,
    Unknown = 1
};

// class CqEncoder

class CqEncoder
{
public:
    static QString at(qint64 uid);
    static QString image(const QString &name);
};

// class CqAssistant

class CqAssistantPrivate;
class CqAssistant : public QObject, public CqEncoder
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CqAssistant)

public:
    explicit CqAssistant(qint32 token, QObject *parent = Q_NULLPTR);
    virtual ~CqAssistant();
private:
    QScopedPointer<CqAssistantPrivate> d_ptr;

public:
    static QByteArray conv(const QString &str);
    static QString conv(const char *gbkStr);
    static QString conv(const QByteArray &str);

public:
    bool privateMessageEventFilter(const MessageEvent &ev);
    bool groupMessageEventFilter(const MessageEvent &ev);
    bool discussMessageEventFilter(const MessageEvent &ev);

    bool masterChangeEventFilter(const MasterChangeEvent &ev);
    bool friendRequestEventFilter(const FriendRequestEvent &ev);
    bool groupRequestEventFilter(const GroupRequestEvent &ev);

    bool friendAddEventFilter(const FriendAddEvent &ev);
    bool memberJoinEventFilter(const MemberJoinEvent &ev);
    bool memberLeaveEventFilter(const MemberLeaveEvent &ev);

protected:
    qint64 currentId() const;
    QString basePath() const;

protected:
    QString appFilePath(const char *srcName) const;
    QString appFilePath(const QString &name) const;
    QString usrFilePath(const char *srcName) const;
    QString usrFilePath(const QString &name) const;

protected:
    CqCode sendPrivateMessage(qint64 uid, const char *gbkMsg) const;
    CqCode sendGroupMessage(qint64 gid, const char *gbkMsg) const;
    CqCode sendDiscussMessage(qint64 did, const char *gbkMsg) const;

    CqCode sendPrivateMessage(qint64 uid, const QString &msg) const;
    CqCode sendGroupMessage(qint64 gid, const QString &msg) const;
    CqCode sendDiscussMessage(qint64 did, const QString &msg) const;

    CqCode banGroupMember(qint64 gid, qint64 uid, int duration);
    CqCode kickGroupMember(qint64 gid, qint64 uid, bool lasting);
    CqCode adminGroupMember(qint64 gid, qint64 uid, bool enabled);

    CqCode renameGroupMember(qint64 gid, qint64 uid, const char *gbkNewNickName);
    CqCode renameGroupMember(qint64 gid, qint64 uid, const QString &newNameCard);

protected:
    CqCode acceptRequest(const char *gbkTag);
    CqCode rejectRequest(const char *gbkTag);

    CqCode acceptRequest(qint32 type, const char *gbkTag);
    CqCode rejectRequest(qint32 type, const char *gbkTag);

protected:
    CqCode leaveGroup(qint64 gid);
    CqCode leaveDiscuss(qint64 did);

    CqCode mute(qint64 gid, bool muted);

protected:
    MemberInfo memberInfo(qint64 gid, qint64 uid, bool cache = true);
    PersonInfo personInfo(qint64 uid, bool cache = true);

protected:
    QString saveImage(const QImage &p) const;
    QImage loadImage(const QString &f) const;
};

#endif // CQASSISTANT_H
