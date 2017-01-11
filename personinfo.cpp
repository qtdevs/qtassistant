#include "personinfo.h"
#include "personinfo_p.h"

#include <QDataStream>

#include "cqassistant.h"

// class PersonInfo

PersonInfo::PersonInfo(const char *pi)
    : data(new PersonInfoData())
{
    if (Q_NULLPTR == pi) {
        return;
    }

    /// README: https://cqp.cc/t/26287
    /// README: https://cqp.cc/t/25702
    QByteArray datas = QByteArray::fromBase64(pi);
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
        data->nickName = CqAssistant::conv(name);
    } while (false);

    ds >> data->sex;
    ds >> data->age;
}

PersonInfo::PersonInfo(const PersonInfo &other)
    : data(other.data)
{
}

PersonInfo &PersonInfo::operator=(const PersonInfo &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

PersonInfo::~PersonInfo()
{
}

bool PersonInfo::isValid() const
{
    return (data->uid != 0);
}

qint64 PersonInfo::uid() const
{
    return data->uid;
}

qint32 PersonInfo::sex() const
{
    return data->sex;
}

qint32 PersonInfo::age() const
{
    return data->age;
}

QString PersonInfo::nickName() const
{
    return data->nickName;
}

// class PersonInfoData

PersonInfoData::PersonInfoData()
    : uid(0)
    , sex(0)
    , age(0)
{
}
