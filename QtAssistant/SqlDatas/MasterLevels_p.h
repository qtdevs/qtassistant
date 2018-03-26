#ifndef MASTERLEVELS_P_H
#define MASTERLEVELS_P_H

#include "CoolQSqliteService_p.h"
#include "MasterLevels.h"

class MasterLevelsPrivate : public CoolQ::SqliteServicePrivate
{
    Q_DECLARE_PUBLIC(MasterLevels)

public:
    MasterLevelsPrivate();
    virtual ~MasterLevelsPrivate();

private:
    QSet<qint64> superUsers;
    QHash<CoolQ::Member, MasterLevel> levels;
};

#endif // MASTERLEVELS_P_H
