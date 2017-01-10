#ifndef MEMBERDEATHHOUSE_P_H
#define MEMBERDEATHHOUSE_P_H

#include "sqldata_p.h"
#include "memberdeathhouse.h"

class MemberDeathHousePrivate : public SqlDataPrivate
{
    Q_DECLARE_PUBLIC(MemberDeathHouse)

public:
    MemberDeathHousePrivate();
    virtual ~MemberDeathHousePrivate();

private:
    QHash<Member, qint64> deathHouse;
};

#endif // MEMBERDEATHHOUSE_P_H
