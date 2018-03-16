#ifndef MEMBERDEATHHOUSE_P_H
#define MEMBERDEATHHOUSE_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "CqSqliteService_p.h"
#include "memberdeathhouse.h"

class MemberDeathHousePrivate : public CoolQ::SqliteServicePrivate
{
    Q_DECLARE_PUBLIC(MemberDeathHouse)

public:
    MemberDeathHousePrivate();
    virtual ~MemberDeathHousePrivate();

private:
    QHash<CoolQ::Member, qint64> members;
};

#endif // MEMBERDEATHHOUSE_P_H
