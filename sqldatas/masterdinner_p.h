#ifndef MASTERDINNER_P_H
#define MASTERDINNER_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "cqsqlite_p.h"
#include "masterdinner.h"

class MasterDinnerPrivate : public CqSqlitePrivate
{
    Q_DECLARE_PUBLIC(MasterDinner)

public:
    MasterDinnerPrivate();
    virtual ~MasterDinnerPrivate();
public:
    QHash<MemberDinner, qint64> members;
};
#endif // MASTERDINNER_P_H
