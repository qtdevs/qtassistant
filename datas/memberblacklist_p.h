#ifndef MEMBERBLACKLIST_P_H
#define MEMBERBLACKLIST_P_H

#include "sqldata_p.h"
#include "memberblacklist.h"

class MemberBlacklistPrivate : public SqlDataPrivate
{
    Q_DECLARE_PUBLIC(MemberBlacklist)

public:
    MemberBlacklistPrivate();
    virtual ~MemberBlacklistPrivate();

public:
    QHash<Member, qint64> blacklist;
};

#endif // MEMBERBLACKLIST_P_H
