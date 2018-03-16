#ifndef CQMEMBERINFO_P_H
#define CQMEMBERINFO_P_H

#include "CqMemberInfo.h"

namespace CoolQ {

class MemberInfoData : public QSharedData
{
public:
    MemberInfoData();

public:
    qint64 gid;
    qint64 uid;
    qint32 sex;
    qint32 age;

    QString nickName;
    QString nameCard;
    QString location;

    QString levelName;
    qint32 permission;
    qint32 unfriendly;

    QDateTime joinTime;
    QDateTime lastSent;
};

} // namespace CoolQ

#endif // CQMEMBERINFO_P_H
