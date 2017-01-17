#ifndef CQPERSONINFO_H
#define CQPERSONINFO_H

#include <QDateTime>
#include <QSharedDataPointer>

class CqPersonInfoData;
class CqPersonInfo
{
public:
    CqPersonInfo(const char *info);
    CqPersonInfo(const CqPersonInfo &other);
    CqPersonInfo &operator=(const CqPersonInfo &rhs);
    ~CqPersonInfo();

public:
    bool isValid() const;

public:
    qint64 uid() const;
    qint32 sex() const;
    qint32 age() const;
    QString nickName() const;

private:
    QSharedDataPointer<CqPersonInfoData> data;
};

#endif // CQPERSONINFO_H
