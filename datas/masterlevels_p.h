#ifndef MASTERLEVELS_P_H
#define MASTERLEVELS_P_H

#include "sqldata_p.h"
#include "masterlevels.h"

class MasterLevelsPrivate : public SqlDataPrivate
{
    Q_DECLARE_PUBLIC(MasterLevels)

public:
    MasterLevelsPrivate();
    virtual ~MasterLevelsPrivate();

private:
    QHash<Member, MasterLevel> levels;
};

#endif // MASTERLEVELS_P_H
