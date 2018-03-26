#ifndef COOLQMEMBERINFO_P_H
#define COOLQMEMBERINFO_P_H

#include "CoolQMemberInfo.h"

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

#endif // COOLQMEMBERINFO_P_H
