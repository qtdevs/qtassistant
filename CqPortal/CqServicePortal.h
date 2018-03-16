﻿#ifndef CQSERVICEPORTAL_H
#define CQSERVICEPORTAL_H

#include "CqInterface.h"

#include "CqMemberInfo.h"
#include "CqPersonInfo.h"

namespace CoolQ {

class ServiceModule;
class ServicePortalPrivate;
class ServicePortal : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ServicePortal)

public:
    explicit ServicePortal(QObject *parent);
    static ServicePortal *instance();
    virtual ~ServicePortal();

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

#endif // CQSERVICEPORTAL_H
