#ifndef CQMEMBERINFO_P_H
#define CQMEMBERINFO_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "cqmemberinfo.h"

class CqMemberInfoData : public QSharedData
{
public:
    CqMemberInfoData();

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

#endif // CQMEMBERINFO_P_H
