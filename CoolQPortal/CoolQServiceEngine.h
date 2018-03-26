#ifndef COOLQSERVICEENGINE_H
#define COOLQSERVICEENGINE_H

#include "CoolQInterface.h"

#include "CoolQMemberInfo.h"
#include "CoolQPersonInfo.h"

namespace CoolQ {

class ServiceModule;
class ServiceEnginePrivate;
class ServiceEngine : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ServiceEngine)

public:
    explicit ServiceEngine(QObject *parent);
    static ServiceEngine *instance();
    virtual ~ServiceEngine();

public:
    virtual bool initialize() override;

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
};

} // namespace CoolQ

#endif // COOLQSERVICEENGINE_H
