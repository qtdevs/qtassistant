#ifndef DONATEMODULE_P_H
#define DONATEMODULE_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

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
};

#endif // DONATEMODULE_P_H
