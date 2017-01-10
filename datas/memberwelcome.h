#ifndef MEMBERWELCOME_H
#define MEMBERWELCOME_H

#include <QHash>

#include "sqldata.h"

class MemberWelcomePrivate;
class MemberWelcome : public SqlData
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
    QHash<Member, qint64> welcome() const;
};

#endif // MEMBERWELCOME_H
