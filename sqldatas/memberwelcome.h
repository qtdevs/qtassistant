#ifndef MEMBERWELCOME_H
#define MEMBERWELCOME_H

#include <QHash>

#include "cqsqlite.h"

class MemberWelcomePrivate;
class MemberWelcome : public CqSqlite
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MemberWelcome)

public:
    explicit MemberWelcome(QObject *parent = Q_NULLPTR);
    virtual ~MemberWelcome();

public:
    Result addMember(qint64 gid, qint64 uid);
    Result removeMember(qint64 gid, qint64 uid);

public:
    QHash<Member, qint64> members() const;
    void expiredMembers(MemberList &members);
};

#endif // MEMBERWELCOME_H
