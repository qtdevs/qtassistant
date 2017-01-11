#include "memberinfo.h"
#include "memberinfo_p.h"

#include <QDataStream>

#include "cqassistant.h"

// class MemberInfo

MemberInfo::MemberInfo(const char *mi)
    : data(new MemberInfoData())
{
    if (Q_NULLPTR == mi) {
        return;
    }

    /// README: https://cqp.cc/t/26287
    /// README: https://cqp.cc/t/25702
    QByteArray datas = QByteArray::fromBase64(mi);
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
        data->nickName = CqAssistant::conv(name);
    } while (false);

    do {
        qint16 size = 0;
        ds >> size;
        QByteArray name(size, 0);
        ds.readRawData(name.data(), size);
        data->nameCard = CqAssistant::conv(name);
    } while (false);

    ds >> data->sex;
    ds >> data->age;

    do {
        qint16 size = 0;
        ds >> size;
        QByteArray location(size, 0);
        ds.readRawData(location.data(), size);
        data->location = CqAssistant::conv(location);
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
        data->levelName = CqAssistant::conv(levelName);
    } while (false);

    ds >> data->permission;
    ds >> data->unfriendly;
}

MemberInfo::MemberInfo(const MemberInfo &other)
    : data(other.data)
{
}

MemberInfo &MemberInfo::operator=(const MemberInfo &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

MemberInfo::~MemberInfo()
{
}

bool MemberInfo::isValid() const
{
    return data->gid != 0;
}

qint64 MemberInfo::gid() const
{
    return data->gid;
}

qint64 MemberInfo::uid() const
{
    return data->uid;
}

qint32 MemberInfo::sex() const
{
    return data->sex;
}

qint32 MemberInfo::age() const
{
    return data->age;
}

QString MemberInfo::nickName() const
{
    return data->nickName;
}

QString MemberInfo::nameCard() const
{
    return data->nameCard;
}

QString MemberInfo::location() const
{
    return data->location;
}

QString MemberInfo::levelName() const
{
    return data->levelName;
}

qint32 MemberInfo::permission() const
{
    return data->permission;
}

qint32 MemberInfo::unfriendly() const
{
    return data->unfriendly;
}

QDateTime MemberInfo::joinTime() const
{
    return data->joinTime;
}

QDateTime MemberInfo::lastSent() const
{
    return data->lastSent;
}

// class MemberInfoData

MemberInfoData::MemberInfoData()
    : gid(0)
    , uid(0)
    , sex(0)
    , age(0)
    , permission(0)
    , unfriendly(0)
{
}
