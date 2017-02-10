#ifndef MASTERDINNER_H
#define MASTERDINNER_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include <QHash>

#include "cqsqlite.h"
class DinnerInfo
{
public:
    DinnerInfo(qint64 dinnerType, QString name)
        : dinnerType(dinnerType), name(name)
    { }
public:
    qint64 dinnerType;
    QString name;
};
typedef QList<DinnerInfo> DinnerInfoList;

class MasterDinnerPrivate;
class MasterDinner : public CqSqlite
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MasterDinner)

public:
    explicit MasterDinner(QObject *parent = Q_NULLPTR);
    virtual ~MasterDinner();

public:
    Result addMember(qint64 dinnerId, QString name);
    Result removeMember(qint64 dinnerId, QString name);

public:
    QHash<MemberDinner, qint64> members() const;
    bool contains(qint64 dinnerId, QString name) const;
};
#endif // MASTERDINNER_H
