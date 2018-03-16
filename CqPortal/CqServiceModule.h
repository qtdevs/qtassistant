#ifndef CQSERVICEMODULE_H
#define CQSERVICEMODULE_H

#include "CqInterface.h"

#include "CqPersonInfo.h"
#include "CqMemberInfo.h"

namespace CoolQ {

class ServicePortal;
class ServiceModulePrivate;
class ServiceModule : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ServiceModule)

protected:
    ServiceModule(ServiceModulePrivate &dd, ServicePortal *parent);
public:
    explicit ServiceModule(ServicePortal *parent);
public:
    virtual ~ServiceModule();

public:
    ServicePortal *portal() const;

public:
    virtual bool initialize() override;

public:
    static QString at(qint64 uid);
    static QString cqEmoji(int emojiCode);
    static QString cqImage(const QString &name);

    static QString face(int value);
    static QString emoji(qint32 value);

public:
    qint64 currentId() const;
    QString basePath() const;

public:
    QString appFilePath(const char *srcName) const;
    QString appFilePath(const QString &name) const;
    QString usrFilePath(const char *srcName) const;
    QString usrFilePath(const QString &name) const;
    QString resFilePath(const char *srcName) const;
    QString resFilePath(const QString &name) const;
    QString imgFilePath(const char *srcName) const;
    QString imgFilePath(const QString &name) const;

public:
    int privateMessageEventPriority() const;
    int groupMessageEventPriority() const;
    int discussMessageEventPriority() const;

    int masterChangeEventPriority() const;
    int friendRequestEventPriority() const;
    int groupRequestEventPriority() const;

    int friendAddEventPriority() const;
    int memberJoinEventPriority() const;
    int memberLeaveEventPriority() const;

public:
    virtual bool privateMessageEvent(const MessageEvent &ev);
    virtual bool groupMessageEvent(const MessageEvent &ev);
    virtual bool discussMessageEvent(const MessageEvent &ev);

    virtual bool masterChangeEvent(const MasterChangeEvent &ev);
    virtual bool friendRequestEvent(const FriendRequestEvent &ev);
    virtual bool groupRequestEvent(const GroupRequestEvent &ev);

    virtual bool friendAddEvent(const FriendAddEvent &ev);
    virtual bool memberJoinEvent(const MemberJoinEvent &ev);
    virtual bool memberLeaveEvent(const MemberLeaveEvent &ev);

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
    PersonInfo personInfo(qint64 uid, bool cached = true);
    MemberInfo memberInfo(qint64 gid, qint64 uid, bool cached = true);

public:
    QString saveImage(const QPixmap &data) const;
    QPixmap loadImage(const QString &name) const;
};

} // namespace CoolQ

#endif // CQSERVICEMODULE_H
