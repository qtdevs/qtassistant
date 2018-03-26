#ifndef CQSERVICEENGINE_P_H
#define CQSERVICEENGINE_P_H

#include "CqInterface_p.h"
#include "CqServiceEngine.h"

namespace CoolQ {

class ServiceEnginePrivate : public InterfacePrivate
{
    Q_DECLARE_PUBLIC(ServiceEngine)

public:
    ServiceEnginePrivate();
    virtual ~ServiceEnginePrivate();

public:
    static ServiceEnginePrivate *get(ServiceEngine *o) { return o ? o->d_func() : nullptr; }
    static const ServiceEnginePrivate *get(const ServiceEngine *o) { return o ? o->d_func() : nullptr; }

public:
    void installModule(ServiceModule *module);
public:
    QList<ServiceModule *> modules;
    static qint32 accessToken;
    static ServiceEngine *instance;

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

#endif // CQSERVICEENGINE_P_H
