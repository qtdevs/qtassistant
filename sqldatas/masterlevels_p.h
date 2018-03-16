﻿#ifndef MASTERLEVELS_P_H
#define MASTERLEVELS_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "CqSqliteService_p.h"
#include "masterlevels.h"

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
