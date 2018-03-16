#ifndef CQMEMBERINFO_H
#define CQMEMBERINFO_H

#include "CqInterface.h"
#include <QSharedDataPointer>
#include <QDateTime>

namespace CoolQ {

class MemberInfoData;
class MemberInfo
{
public:
    MemberInfo(const char *info);
    MemberInfo(const MemberInfo &other);
    MemberInfo &operator=(const MemberInfo &r);
    ~MemberInfo();

public:
    bool isValid() const;

public:
    qint64 gid() const;
    qint64 uid() const;
    qint32 sex() const;
    qint32 age() const;

    QString nickName() const;
    QString nameCard() const;
    QString location() const;

    QString levelName() const;
    qint32 permission() const;
    qint32 unfriendly() const;

    QDateTime joinTime() const;
    QDateTime lastSent() const;

private:
    QSharedDataPointer<MemberInfoData> data;
};

} // namespace CoolQ

#endif // CQMEMBERINFO_H
