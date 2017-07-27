#ifndef CQENGINE_P_H
#define CQENGINE_P_H

#include "cqengine.h"

class CqEnginePrivate
{
    Q_DECLARE_PUBLIC(CqEngine)

public:
    CqEnginePrivate();
    virtual ~CqEnginePrivate();
protected:
    CqEngine *q_ptr;

public:
    static CqEnginePrivate *get(CqEngine *o) { return o ? o->d_func() : nullptr; }
    static const CqEnginePrivate *get(const CqEngine *o) { return o ? o->d_func() : nullptr; }

public:
    static CqEnginePrivate *self() { return get(instance); }
public:
    static qint32 accessToken;
    static CqEngine *instance;

public:
    QVector<CqModule *> privateMessageModules;
    QVector<CqModule *> groupMessageModules;
    QVector<CqModule *> discussMessageModules;
public:
    QVector<CqModule *> masterChangeModules;
    QVector<CqModule *> friendRequestModules;
    QVector<CqModule *> groupRequestModules;
public:
    QVector<CqModule *> friendAddModules;
    QVector<CqModule *> memberJoinModules;
    QVector<CqModule *> memberLeaveModules;
};

#endif // CQENGINE_P_H
