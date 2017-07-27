#include "cqengine.h"
#include "cqengine_p.h"

#include "cqmodule.h"
#include "cqmodule_p.h"

// class CqEngine

CqEngine::CqEngine(QObject *parent)
    : QObject(parent)
    , d_ptr(new CqEnginePrivate())
{
    d_ptr->q_ptr = this;

    Q_ASSERT(nullptr == CqEnginePrivate::instance);
    CqEnginePrivate::instance = this;
}

CqEngine::~CqEngine()
{
    CqEnginePrivate::instance = nullptr;
}

void CqEngine::installModule(CqModule *module)
{
    Q_D(CqEngine);

    CqModulePrivate *modulePriv
            = CqModulePrivate::get(module);

    if (modulePriv && modulePriv->privateMessageEventPriority > 0)
        d->privateMessageModules.append(module);
    if (modulePriv && modulePriv->groupMessageEventPriority > 0)
        d->groupMessageModules.append(module);
    if (modulePriv && modulePriv->discussMessageEventPriority > 0)
        d->discussMessageModules.append(module);

    if (modulePriv && modulePriv->masterChangeEventPriority > 0)
        d->masterChangeModules.append(module);
    if (modulePriv && modulePriv->friendRequestEventPriority > 0)
        d->friendRequestModules.append(module);
    if (modulePriv && modulePriv->groupRequestEventPriority > 0)
        d->groupRequestModules.append(module);

    if (modulePriv && modulePriv->friendAddEventPriority > 0)
        d->friendAddModules.append(module);
    if (modulePriv && modulePriv->memberJoinEventPriority > 0)
        d->memberJoinModules.append(module);
    if (modulePriv && modulePriv->memberLeaveEventPriority > 0)
        d->memberLeaveModules.append(module);
}

// class CqEnginePrivate

qint32 CqEnginePrivate::accessToken = 0;
CqEngine *CqEnginePrivate::instance = nullptr;

CqEnginePrivate::CqEnginePrivate()
    : q_ptr(nullptr)
{
}

CqEnginePrivate::~CqEnginePrivate()
{
}
