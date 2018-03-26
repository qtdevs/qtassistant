#ifndef COOLQPERSONINFO_P_H
#define COOLQPERSONINFO_P_H

#include "CoolQPersonInfo.h"

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

#endif // COOLQPERSONINFO_P_H
