#ifndef MEMBERINFO_P_H
#define MEMBERINFO_P_H

#include "memberinfo.h"

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

#endif // MEMBERINFO_P_H
