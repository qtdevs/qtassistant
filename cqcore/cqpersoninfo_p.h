#ifndef CQPERSONINFO_P_H
#define CQPERSONINFO_P_H

#include "cqpersoninfo.h"

class CqPersonInfoData : public QSharedData
{
public:
    CqPersonInfoData();

public:
    qint64 uid;
    qint32 sex;
    qint32 age;
    QString nickName;
};

#endif // CQPERSONINFO_P_H
