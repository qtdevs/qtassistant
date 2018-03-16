#ifndef CQPERSONINFO_H
#define CQPERSONINFO_H

#include "CqInterface.h"
#include <QSharedDataPointer>
#include <QDateTime>

namespace CoolQ {

class PersonInfoData;
class PersonInfo
{
public:
    PersonInfo(const char *info);
    PersonInfo(const PersonInfo &other);
    PersonInfo &operator=(const PersonInfo &r);
    ~PersonInfo();

public:
    bool isValid() const;

public:
    qint64 uid() const;
    qint32 sex() const;
    qint32 age() const;

    QString nickName() const;

private:
    QSharedDataPointer<PersonInfoData> data;
};

} // namespace CoolQ

#endif // CQPERSONINFO_H
