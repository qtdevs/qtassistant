#ifndef CQASSISTANT_H
#define CQASSISTANT_H

#include <QObject>
#include "cqaevents.h"

enum class CqCode {
    NoError = 0,
    Unknown = 1
};

class CqAssistantPrivate;
class CqAssistant : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CqAssistant)

public:
    explicit CqAssistant(qint32 token, QObject *parent = Q_NULLPTR);
    virtual ~CqAssistant();
private:
    QScopedPointer<CqAssistantPrivate> d_ptr;

public:
    QByteArray conv(const QString &str) const;
    QString conv(const char *gbkStr) const;
    QString conv(const QByteArray &str) const;

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
    CqCode sendPrivateMessage(qint64 uid, const char *gbkMsg);
    CqCode sendGroupMessage(qint64 gid, const char *gbkMsg);
    CqCode sendDiscussMessage(qint64 did, const char *gbkMsg);

    CqCode sendPrivateMessage(qint64 uid, const QString &msg);
    CqCode sendGroupMessage(qint64 gid, const QString &msg);
    CqCode sendDiscussMessage(qint64 did, const QString &msg);

    CqCode banGroupMember(qint64 gid, qint64 uid, int duration);
    CqCode kickGroupMember(qint64 gid, qint64 uid, bool lasting);
    CqCode adminGroupMember(qint64 gid, qint64 uid, bool enabled);

    CqCode renameGroupMember(qint64 gid, qint64 uid, const char *gbkNewNickName);
    CqCode renameGroupMember(qint64 gid, qint64 uid, const QString &newNickName);

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
    static QString msgAt(qint64 id);

    /*
private:
    const char *memberInfo(qint64 gid, qint64 uid, bool cache = true);
    const char *strangerInfo(qint64 id, bool cache = true);
    */
};

#endif // CQASSISTANT_H
