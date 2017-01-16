#ifndef MEMBERBLACKLIST_H
#define MEMBERBLACKLIST_H

#include <QHash>
#include "cqsqlite.h"

class MemberBlacklistPrivate;
class MemberBlacklist : public CqSqlite
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MemberBlacklist)

public:
    explicit MemberBlacklist(QObject *parent = Q_NULLPTR);
    virtual ~MemberBlacklist();

public:
    Result addMember(qint64 gid, qint64 uid);
    Result removeMember(qint64 gid, qint64 uid);

public:
    QHash<Member, qint64> blacklist() const;
    bool contains(qint64 gid, qint64 uid) const;
};

#endif // MEMBERBLACKLIST_H
