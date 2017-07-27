#ifndef CQMODULE_P_H
#define CQMODULE_P_H

#include "cqmodule.h"

class CqModulePrivate
{
    Q_DECLARE_PUBLIC(CqModule)

public:
    CqModulePrivate();
    virtual ~CqModulePrivate();
protected:
    CqModule *q_ptr;

public:
    static CqModulePrivate *get(CqModule *o) { return o ? o->d_func() : nullptr; }
    static const CqModulePrivate *get(const CqModule *o) { return o ? o->d_func() : nullptr; }

public:
    static CqModule::Result result(qint32 r);

public:
    int privateMessageEventPriority;
    int groupMessageEventPriority;
    int discussMessageEventPriority;

    int masterChangeEventPriority;
    int friendRequestEventPriority;
    int groupRequestEventPriority;

    int friendAddEventPriority;
    int memberJoinEventPriority;
    int memberLeaveEventPriority;

private:
    qint64  currentId;
    QString basePath;
    QString imagePath;
    QString resPath;
};

#endif // CqModule_P_H
