#include "cqmemberinfo.h"
#include "cqmemberinfo_p.h"

#include <QDataStream>

#include "cqportal.h"

// class CqMemberInfo

CqMemberInfo::CqMemberInfo(const char *info)
    : data(new CqMemberInfoData())
{
    if (Q_NULLPTR == info) {
        return;
    }

    /// README: https://cqp.cc/t/26287
    /// README: https://cqp.cc/t/25702
    QByteArray datas = QByteArray::fromBase64(info);
    QDataStream ds(datas);

    ds >> data->gid;
    ds >> data->uid;

    if ((data->gid == 0)
            || (data->uid == 0)) {
        return;
    }

    do {
        qint16 size = 0;
        ds >> size;
        QByteArray name(size, 0);
        ds.readRawData(name.data(), size);
        data->nickName = CqPortal::convert(name);
    } while (false);

    do {
        qint16 size = 0;
        ds >> size;
        QByteArray name(size, 0);
        ds.readRawData(name.data(), size);
        data->nameCard = CqPortal::convert(name);
    } while (false);

    ds >> data->sex;
    ds >> data->age;

    do {
        qint16 size = 0;
        ds >> size;
        QByteArray location(size, 0);
        ds.readRawData(location.data(), size);
        data->location = CqPortal::convert(location);
    } while (false);

    do {
        qint32 stamp = 0;
        ds >> stamp;
        data->joinTime = QDateTime::fromTime_t(stamp);
    } while (false);

    do {
        qint32 stamp = 0;
        ds >> stamp;
        data->lastSent = QDateTime::fromTime_t(stamp);
    } while (false);

    do {
        qint16 size = 0;
        ds >> size;
        QByteArray levelName(size, 0);
        ds.readRawData(levelName.data(), size);
        data->levelName = CqPortal::convert(levelName);
    } while (false);

    ds >> data->permission;
    ds >> data->unfriendly;
}

CqMemberInfo::CqMemberInfo(const CqMemberInfo &other)
    : data(other.data)
{
}

CqMemberInfo &CqMemberInfo::operator=(const CqMemberInfo &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

CqMemberInfo::~CqMemberInfo()
{
}

bool CqMemberInfo::isValid() const
{
    return (data->gid != 0);
}

qint64 CqMemberInfo::gid() const
{
    return data->gid;
}

qint64 CqMemberInfo::uid() const
{
    return data->uid;
}

qint32 CqMemberInfo::sex() const
{
    return data->sex;
}

qint32 CqMemberInfo::age() const
{
    return data->age;
}

QString CqMemberInfo::nickName() const
{
    return data->nickName;
}

QString CqMemberInfo::nameCard() const
{
    return data->nameCard;
}

QString CqMemberInfo::location() const
{
    return data->location;
}

QString CqMemberInfo::levelName() const
{
    return data->levelName;
}

qint32 CqMemberInfo::permission() const
{
    return data->permission;
}

qint32 CqMemberInfo::unfriendly() const
{
    return data->unfriendly;
}

QDateTime CqMemberInfo::joinTime() const
{
    return data->joinTime;
}

QDateTime CqMemberInfo::lastSent() const
{
    return data->lastSent;
}

// class CqMemberInfoData

CqMemberInfoData::CqMemberInfoData()
    : gid(0)
    , uid(0)
    , sex(0)
    , age(0)
    , permission(0)
    , unfriendly(0)
{
}
