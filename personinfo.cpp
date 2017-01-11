#include "personinfo.h"
#include "personinfo_p.h"

// class PersonInfo

PersonInfo::PersonInfo(const char *pi)
    : data(new PersonInfoData())
{
    if (Q_NULLPTR == pi) {
        return;
    }

    /// README: https://cqp.cc/t/26287
    /// README: https://cqp.cc/t/25702
    // QByteArray datas = QByteArray::fromBase64(pi);
    // QDataStream ds(datas);

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
    return false;
}

// class PersonInfoData

PersonInfoData::PersonInfoData()
{
}
