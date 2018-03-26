#ifndef COOLQSERVICEENGINE_P_H
#define COOLQSERVICEENGINE_P_H

#include "CoolQInterface_p.h"
#include "CoolQServiceEngine.h"

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

#endif // COOLQSERVICEENGINE_P_H
