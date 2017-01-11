#ifndef PERSONINFO_P_H
#define PERSONINFO_P_H

#include "personinfo.h"

class PersonInfoData : public QSharedData
{
public:
    PersonInfoData();

public:
    qint64 uid;
    qint32 sex;
    qint32 age;
    QString nickName;
};

#endif // PERSONINFO_P_H
