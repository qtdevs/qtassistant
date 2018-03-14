#ifndef DONATEMODULE_P_H
#define DONATEMODULE_P_H

#include <QMutex>

#include "donatemodule.h"

class DonateModulePrivate
{
    Q_DECLARE_PUBLIC(DonateModule)

public:
    DonateModulePrivate();
    virtual ~DonateModulePrivate();
protected:
    DonateModule *q_ptr;

protected:
    static DonateModule *instance;

private:
    QString ergouString;
    QString gougeString;
    QString maodaString;


    QHash<qint64, QDateTime> lastSents;
    QHash<qint64, int>       retryCounters;
    QMutex                   safeGuard;
};

#endif // DONATEMODULE_P_H
