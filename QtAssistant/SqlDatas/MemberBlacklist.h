#ifndef MEMBERBLACKLIST_H
#define MEMBERBLACKLIST_H

#include <QHash>

#include "CoolQSqliteService.h"

class MemberBlacklistPrivate;
class MemberBlacklist : public CoolQ::SqliteService
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
    QHash<CoolQ::Member, qint64> members() const;
    bool contains(qint64 gid, qint64 uid) const;
};

#endif // MEMBERBLACKLIST_H
