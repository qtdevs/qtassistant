#ifndef MEMBERDEATHHOUSE_H
#define MEMBERDEATHHOUSE_H

#include <QHash>

#include "sqldata.h"

class MemberDeathHousePrivate;
class MemberDeathHouse : public SqlData
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MemberDeathHouse)

public:
    explicit MemberDeathHouse(QObject *parent = Q_NULLPTR);
    virtual ~MemberDeathHouse();

public:
    Result addMember(qint64 gid, qint64 uid);
    Result removeMember(qint64 gid, qint64 uid);

public:
    QHash<Member, qint64> deathHouse() const;
};

#endif // MEMBERDEATHHOUSE_H
