#ifndef MEMBERWELCOME_P_H
#define MEMBERWELCOME_P_H

#include "cqsqlite_p.h"
#include "memberwelcome.h"

class MemberWelcomePrivate : public CqSqlitePrivate
{
    Q_DECLARE_PUBLIC(MemberWelcome)

public:
    MemberWelcomePrivate();
    virtual ~MemberWelcomePrivate();

public:
    QHash<Member, qint64> welcome;
};

#endif // MEMBERWELCOME_P_H
