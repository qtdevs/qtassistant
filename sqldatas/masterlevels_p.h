#ifndef MASTERLEVELS_P_H
#define MASTERLEVELS_P_H

#include "cqsqlite_p.h"
#include "masterlevels.h"

class MasterLevelsPrivate : public CqSqlitePrivate
{
    Q_DECLARE_PUBLIC(MasterLevels)

public:
    MasterLevelsPrivate();
    virtual ~MasterLevelsPrivate();

private:
    QHash<Member, MasterLevel> levels;
};

#endif // MASTERLEVELS_P_H
