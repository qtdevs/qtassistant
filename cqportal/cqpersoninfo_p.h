#ifndef CQPERSONINFO_P_H
#define CQPERSONINFO_P_H

#include "CqPersonInfo.h"

namespace CoolQ {

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

} // namespace CoolQ

#endif // CQPERSONINFO_P_H
