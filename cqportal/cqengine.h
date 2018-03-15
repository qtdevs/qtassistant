#ifndef CQENGINE_H
#define CQENGINE_H

#include <QObject>

#include "cqglobalevents.h"
#include "cqmemberinfo.h"
#include "cqpersoninfo.h"

class CqModule;
class CqEnginePrivate;
class CqEngine : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CqEngine)

public:
    explicit CqEngine(QObject *parent = nullptr);
    virtual ~CqEngine();
protected:
    QScopedPointer<CqEnginePrivate> d_ptr;

public:
    void installModule(CqModule *module);
};

#endif // CQENGINE_H
