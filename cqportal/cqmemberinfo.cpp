/*!
 * \class CqMemberInfo
 * \brief 成员信息
 */

#include "cqmemberinfo.h"
#include "cqmemberinfo_p.h"

#include <QDataStream>

#include "cqportal.h"

// class CqMemberInfo

/*!
 * \brief 构造函数
 *
 * 通过裸数据构造一个 CqMemberInfo 的对象。
 */
CqMemberInfo::CqMemberInfo(const char *info)
    : data(new CqMemberInfoData())
{
    if (Q_NULLPTR == info) {
        return;
    }

    // README: https://cqp.cc/t/26287
    // README: https://cqp.cc/t/25702
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
        if (stamp != 0) {
            data->joinTime = QDateTime::fromTime_t(stamp);
        }
    } while (false);

    do {
        qint32 stamp = 0;
        ds >> stamp;
        if (stamp != 0) {
            data->lastSent = QDateTime::fromTime_t(stamp);
        }
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

/*!
 * \brief 赋值构造函数
 *
 * \internal
 */
CqMemberInfo::CqMemberInfo(const CqMemberInfo &other)
    : data(other.data)
{
}

/*!
 * \brief 赋值函数
 *
 * \internal
 */
CqMemberInfo &CqMemberInfo::operator=(const CqMemberInfo &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

/*!
 * \brief 析构函数
 *
 * \internal
 */
CqMemberInfo::~CqMemberInfo()
{
}

/*!
 * \brief 验证有效信息
 *
 * 如果填充的成员信息有效，此函数会返回 true；否则返回 false。
 */
bool CqMemberInfo::isValid() const
{
    return (data->gid != 0);
}

/*!
 * \brief 返回群组号码
 *
 * 如果成员信息有效，返回群组号码；否则返回 0。
 */
qint64 CqMemberInfo::gid() const
{
    return data->gid;
}

/*!
 * \brief 返回成员号码
 *
 * 如果成员信息有效，返回成员号码；否则返回 0。
 */
qint64 CqMemberInfo::uid() const
{
    return data->uid;
}

/*!
 * \brief 返回性别
 *
 * 如果成员信息有效，返回性别，1 代表男性，2 代表女性，99 代表未知；否则返回 0。
 */
qint32 CqMemberInfo::sex() const
{
    return data->sex;
}

/*!
 * \brief 返回年龄
 *
 * 如果成员信息有效，返回年龄；否则返回 0。
 */
qint32 CqMemberInfo::age() const
{
    return data->age;
}

/*!
 * \brief 返回昵称
 *
 * 如果成员信息有效，返回昵称，不为空；如果信息无效，返回空。
 */
QString CqMemberInfo::nickName() const
{
    return data->nickName;
}

/*!
 * \brief 返回名片
 *
 * 如果成员信息有效，返回名片；如果没有设置名片，或信息无效，返回空。
 */
QString CqMemberInfo::nameCard() const
{
    return data->nameCard;
}

/*!
 * \brief 返回地区
 *
 * 如果成员信息有效，返回地区。地区一般是用户设置所在地的最后一个字段；如果设置不可见，或未设置，否则返回空。
 */
QString CqMemberInfo::location() const
{
    return data->location;
}

/*!
 * \brief 返回等级
 *
 * 如果成员信息有效，返回等级，具体等级由群内设置决定；否则返回空。
 */
QString CqMemberInfo::levelName() const
{
    return data->levelName;
}

/*!
 * \brief 返回成员权限
 *
 * 如果成员信息有效，返回成员权限。1 为成员，2 为管理，3 为群主；否则返回 0。
 */
qint32 CqMemberInfo::permission() const
{
    return data->permission;
}

/*!
 * \brief 返回失信状态
 *
 * 如果成员信息有效，返回失信状态；否则返回 0。
 */
qint32 CqMemberInfo::unfriendly() const
{
    return data->unfriendly;
}

/*!
 * \brief 返回加入群组时间
 *
 * 如果成员信息有效，返回加入群组时间；否则返回一个无效的 QDateTime()。
 */
QDateTime CqMemberInfo::joinTime() const
{
    return data->joinTime;
}

/*!
 * \brief 返回最后发送时间
 *
 * 如果成员信息有效，返回最后发送时间；否则返回一个无效的 QDateTime()。
 */
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
