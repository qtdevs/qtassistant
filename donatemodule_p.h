#ifndef DONATEMODULE_P_H
#define DONATEMODULE_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include <QMutex>

#include "cqmodule_p.h"
#include "donatemodule.h"

class DonateModulePrivate : public CqModulePrivate
{
    Q_DECLARE_PUBLIC(DonateModule)

public:
    DonateModulePrivate();
    virtual ~DonateModulePrivate();

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
