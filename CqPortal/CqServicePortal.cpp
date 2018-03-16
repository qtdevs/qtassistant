#include "CqServicePortal.h"
#include "CqServicePortal_p.h"

#include "CqServiceModule.h"
#include "CqServiceModule_p.h"

namespace CoolQ {

// class ServicePortal

ServicePortal::ServicePortal(QObject *parent)
    : Interface(*new ServicePortalPrivate(), parent)
{
    Q_ASSERT(nullptr == ServicePortalPrivate::instance);
    ServicePortalPrivate::instance = this;
}

ServicePortal::~ServicePortal()
{
    ServicePortalPrivate::instance = nullptr;
}

ServicePortal *ServicePortal::instance()
{
    return ServicePortalPrivate::instance;
}

bool ServicePortal::initialize()
{
    Q_D(ServicePortal);

    for(ServiceModule *module : d->modules) {
        if (!module->initialize())
            return false;
    }

    for(ServiceModule *module : d->modules) {
        if (module->privateMessageEventPriority() > 0)
            d->privateMessageModules.append(module);
        if (module->groupMessageEventPriority() > 0)
            d->groupMessageModules.append(module);
        if (module->discussMessageEventPriority() > 0)
            d->discussMessageModules.append(module);

        if (module->masterChangeEventPriority() > 0)
            d->masterChangeModules.append(module);
        if (module->friendRequestEventPriority() > 0)
            d->friendRequestModules.append(module);
        if (module->groupRequestEventPriority() > 0)
            d->groupRequestModules.append(module);

        if (module->friendAddEventPriority() > 0)
            d->friendAddModules.append(module);
        if (module->memberJoinEventPriority() > 0)
            d->memberJoinModules.append(module);
        if (module->memberLeaveEventPriority() > 0)
            d->memberLeaveModules.append(module);
    }

    return true;
}

bool ServicePortal::privateMessageEvent(const MessageEvent &ev)
{
    Q_D(ServicePortal);

    for (ServiceModule *module : d->privateMessageModules)
        if (module->privateMessageEvent(ev))
            return true;

    return false;
}

bool ServicePortal::groupMessageEvent(const MessageEvent &ev)
{
    Q_D(ServicePortal);

    for (ServiceModule *module : d->groupMessageModules)
        if (module->groupMessageEvent(ev))
            return true;

    return false;
}

bool ServicePortal::discussMessageEvent(const MessageEvent &ev)
{
    Q_D(ServicePortal);

    for (ServiceModule *module : d->discussMessageModules)
        if (module->discussMessageEvent(ev))
            return true;

    return false;
}

bool ServicePortal::masterChangeEvent(const MasterChangeEvent &ev)
{
    Q_D(ServicePortal);

    for (ServiceModule *module : d->masterChangeModules)
        if (module->masterChangeEvent(ev))
            return true;

    return false;
}

bool ServicePortal::friendRequestEvent(const FriendRequestEvent &ev)
{
    Q_D(ServicePortal);

    for (ServiceModule *module : d->friendRequestModules)
        if (module->friendRequestEvent(ev))
            return true;

    return false;
}

bool ServicePortal::groupRequestEvent(const GroupRequestEvent &ev)
{
    Q_D(ServicePortal);

    for (ServiceModule *module : d->groupRequestModules)
        if (module->groupRequestEvent(ev))
            return true;

    return false;
}

bool ServicePortal::friendAddEvent(const FriendAddEvent &ev)
{
    Q_D(ServicePortal);

    for (ServiceModule *module : d->friendAddModules)
        if (module->friendAddEvent(ev))
            return true;

    return false;
}

bool ServicePortal::memberJoinEvent(const MemberJoinEvent &ev)
{
    Q_D(ServicePortal);

    for (ServiceModule *module : d->memberJoinModules)
        if (module->memberJoinEvent(ev))
            return true;

    return false;
}

bool ServicePortal::memberLeaveEvent(const MemberLeaveEvent &ev)
{
    Q_D(ServicePortal);

    for (ServiceModule *module : d->memberLeaveModules)
        if (module->memberLeaveEvent(ev))
            return true;

    return false;
}

// class ServicePortalPrivate

ServicePortal *ServicePortalPrivate::instance = nullptr;

qint32 ServicePortalPrivate::accessToken = 0;

ServicePortalPrivate::ServicePortalPrivate()
{
}

ServicePortalPrivate::~ServicePortalPrivate()
{
}

void ServicePortalPrivate::installModule(ServiceModule *module)
{
    Q_CHECK_PTR(module);

    modules.append(module);
}

} // namespace CoolQ
