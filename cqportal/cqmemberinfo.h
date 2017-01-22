#ifndef CQMEMBERINFO_H
#define CQMEMBERINFO_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include <QDateTime>
#include <QSharedDataPointer>

class CqMemberInfoData;
class CqMemberInfo
{
public:
    CqMemberInfo(const char *info);
    CqMemberInfo(const CqMemberInfo &other);
    CqMemberInfo &operator=(const CqMemberInfo &rhs);
    ~CqMemberInfo();

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
    QSharedDataPointer<CqMemberInfoData> data;
};

#endif // CQMEMBERINFO_H
