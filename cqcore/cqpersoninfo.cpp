#include "cqpersoninfo.h"
#include "cqpersoninfo_p.h"

#include <QDataStream>

#include "cqportal.h"

// class CqPersonInfo

CqPersonInfo::CqPersonInfo(const char *info)
    : data(new CqPersonInfoData())
{
    if (Q_NULLPTR == info) {
        return;
    }

    /// README: https://cqp.cc/t/26287
    /// README: https://cqp.cc/t/25702
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
        data->nickName = CqPortal::convert(name);
    } while (false);

    ds >> data->sex;
    ds >> data->age;
}

CqPersonInfo::CqPersonInfo(const CqPersonInfo &other)
    : data(other.data)
{
}

CqPersonInfo &CqPersonInfo::operator=(const CqPersonInfo &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

CqPersonInfo::~CqPersonInfo()
{
}

bool CqPersonInfo::isValid() const
{
    return (data->uid != 0);
}

qint64 CqPersonInfo::uid() const
{
    return data->uid;
}

qint32 CqPersonInfo::sex() const
{
    return data->sex;
}

qint32 CqPersonInfo::age() const
{
    return data->age;
}

QString CqPersonInfo::nickName() const
{
    return data->nickName;
}

// class CqPersonInfoData

CqPersonInfoData::CqPersonInfoData()
    : uid(0)
    , sex(0)
    , age(0)
{
}
