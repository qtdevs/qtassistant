#ifndef MEMBERDEATHHOUSE_P_H
#define MEMBERDEATHHOUSE_P_H

#include "cqsqlite_p.h"
#include "memberdeathhouse.h"

class MemberDeathHousePrivate : public CqSqlitePrivate
{
    Q_DECLARE_PUBLIC(MemberDeathHouse)

public:
    MemberDeathHousePrivate();
    virtual ~MemberDeathHousePrivate();

private:
    QHash<Member, qint64> members;
};

#endif // MEMBERDEATHHOUSE_P_H
