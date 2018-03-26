#ifndef MEMBERBLACKLIST_P_H
#define MEMBERBLACKLIST_P_H

#include "CqSqliteService_p.h"
#include "MemberBlacklist.h"

class MemberBlacklistPrivate : public CoolQ::SqliteServicePrivate
{
    Q_DECLARE_PUBLIC(MemberBlacklist)

public:
    MemberBlacklistPrivate();
    virtual ~MemberBlacklistPrivate();

public:
    QHash<CoolQ::Member, qint64> members;
};

#endif // MEMBERBLACKLIST_P_H
