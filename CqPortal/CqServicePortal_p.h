#ifndef CQSERVICEPORTAL_P_H
#define CQSERVICEPORTAL_P_H

#include "CqInterface_p.h"
#include "CqServicePortal.h"

namespace CoolQ {

class ServicePortalPrivate : public InterfacePrivate
{
    Q_DECLARE_PUBLIC(ServicePortal)

public:
    ServicePortalPrivate();
    virtual ~ServicePortalPrivate();

public:
    static ServicePortalPrivate *get(ServicePortal *o) { return o ? o->d_func() : nullptr; }
    static const ServicePortalPrivate *get(const ServicePortal *o) { return o ? o->d_func() : nullptr; }

public:
    void installModule(ServiceModule *module);
protected:
    QList<ServiceModule *> modules;

public:
    static qint32 accessToken;
    static ServicePortal *instance;

protected:
    QVector<ServiceModule *> privateMessageModules;
    QVector<ServiceModule *> groupMessageModules;
    QVector<ServiceModule *> discussMessageModules;

    QVector<ServiceModule *> masterChangeModules;
    QVector<ServiceModule *> friendRequestModules;
    QVector<ServiceModule *> groupRequestModules;

    QVector<ServiceModule *> friendAddModules;
    QVector<ServiceModule *> memberJoinModules;
    QVector<ServiceModule *> memberLeaveModules;
};

} // namespace CoolQ

#endif // CQSERVICEPORTAL_P_H
