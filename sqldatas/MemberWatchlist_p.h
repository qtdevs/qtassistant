#ifndef MEMBERWATCHLIST_P_H
#define MEMBERWATCHLIST_P_H

#include "CqSqliteService_p.h"
#include "MemberWatchlist.h"

class MemberWatchlistPrivate : public CoolQ::SqliteServicePrivate
{
    Q_DECLARE_PUBLIC(MemberWatchlist)

public:
    MemberWatchlistPrivate();
    virtual ~MemberWatchlistPrivate();

public:
    QHash<CoolQ::Member, qint64> members;
};

#endif // MEMBERWATCHLIST_P_H
