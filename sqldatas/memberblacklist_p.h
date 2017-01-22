#ifndef MEMBERBLACKLIST_P_H
#define MEMBERBLACKLIST_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "cqsqlite_p.h"
#include "memberblacklist.h"

class MemberBlacklistPrivate : public CqSqlitePrivate
{
    Q_DECLARE_PUBLIC(MemberBlacklist)

public:
    MemberBlacklistPrivate();
    virtual ~MemberBlacklistPrivate();

public:
    QHash<Member, qint64> members;
};

#endif // MEMBERBLACKLIST_P_H
