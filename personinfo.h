#ifndef PERSONINFO_H
#define PERSONINFO_H

#include <QDateTime>
#include <QSharedDataPointer>

class PersonInfoData;
class PersonInfo
{
public:
    PersonInfo(const char *pi);
    PersonInfo(const PersonInfo &other);
    PersonInfo &operator=(const PersonInfo &rhs);
    ~PersonInfo();

public:
    bool isValid() const;

private:
    QSharedDataPointer<PersonInfoData> data;
};

#endif // PERSONINFO_H
