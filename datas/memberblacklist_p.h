#ifndef MEMBERBLACKLIST_P_H
#define MEMBERBLACKLIST_P_H

#include "cqsqlite_p.h"
#include "memberblacklist.h"

class MemberBlacklistPrivate : public CqSqlitePrivate
{
    Q_DECLARE_PUBLIC(MemberBlacklist)

public:
    MemberBlacklistPrivate();
    virtual ~MemberBlacklistPrivate();

public:
    QHash<Member, qint64> blacklist;
};

#endif // MEMBERBLACKLIST_P_H
