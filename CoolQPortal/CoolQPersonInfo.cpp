/*! \class CqPersonInfo
 * \brief 个人信息
 */

#include "CoolQPersonInfo.h"
#include "CoolQPersonInfo_p.h"

#include <QDataStream>

namespace CoolQ {

// class CqPersonInfo

/*! \brief 构造函数
 *
 * 通过裸数据构造一个 CqPersonInfo 的对象。
 */
PersonInfo::PersonInfo(const char *info)
    : data(new PersonInfoData())
{
    if (Q_NULLPTR == info) {
        return;
    }

    // README: https://cqp.cc/t/26287
    // README: https://cqp.cc/t/25702
    QByteArray datas = QByteArray::fromBase64(info);
    QDataStream ds(datas);

    ds >> data->uid;
    if (data->uid == 0) {
        return;
    }

    do {
        qint16 size = 0;
        ds >> size;
        QByteArray name(size, 0);
        ds.readRawData(name.data(), size);
        data->nickName = trGbk(name);
    } while (false);

    ds >> data->sex;
    ds >> data->age;
}

/*! \brief 赋值构造函数
 *
 * \internal
 */
PersonInfo::PersonInfo(const PersonInfo &other)
    : data(other.data)
{
}

/*! \brief 赋值函数
 *
 * \internal
 */
PersonInfo &PersonInfo::operator=(const PersonInfo &r)
{
    if (this != &r)
        data.operator=(r.data);

    return *this;
}

/*! \brief 析构函数
 *
 * \internal
 */
PersonInfo::~PersonInfo()
{
}

/*! \brief 验证有效信息
 *
 * 如果填充的个人信息有效，此函数会返回 true；否则返回 false。
 */
bool PersonInfo::isValid() const
{
    return (data->uid != 0);
}

/*! \brief 返回个人号码
 *
 * 如果个人信息有效，返回个人号码；否则返回 0。
 */
qint64 PersonInfo::uid() const
{
    return data->uid;
}

/*! \brief 返回性别
 *
 * 如果个人信息有效，返回性别，1 代表男性，2 代表女性，99 代表未知；否则返回 0。
 */
qint32 PersonInfo::sex() const
{
    return data->sex;
}

/*! \brief 返回年龄
 *
 * 如果个人信息有效，返回年龄；否则返回 0。
 */
qint32 PersonInfo::age() const
{
    return data->age;
}

/*! \brief 返回昵称
 *
 * 如果个人信息有效，返回昵称，不为空；如果信息无效，返回空。
 */
QString PersonInfo::nickName() const
{
    return data->nickName;
}

// class CqPersonInfoData

PersonInfoData::PersonInfoData()
    : uid(0)
    , sex(0)
    , age(0)
{
}

} // namespace CoolQ
