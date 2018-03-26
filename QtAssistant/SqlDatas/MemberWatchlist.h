#ifndef MEMBERWATCHLIST_H
#define MEMBERWATCHLIST_H

#include <QHash>

#include "CoolQSqliteService.h"

class MemberWatchlistPrivate;
class MemberWatchlist : public CoolQ::SqliteService
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MemberWatchlist)

public:
    explicit MemberWatchlist(QObject *parent = Q_NULLPTR);
    virtual ~MemberWatchlist();

public:
    Result addMember(qint64 gid, qint64 uid);
    Result removeMember(qint64 gid, qint64 uid);

public:
    QHash<CoolQ::Member, qint64> members() const;
    bool contains(qint64 gid, qint64 uid) const;

    void expiredMembers(CoolQ::MemberList &members);
};

#endif // MEMBERWATCHLIST_H
