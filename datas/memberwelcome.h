#ifndef MEMBERWELCOME_H
#define MEMBERWELCOME_H

#include "sqldata.h"

class MemberWelcomePrivate;
class MemberWelcome : public SqlData
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MemberWelcome)

public:
    explicit MemberWelcome(QObject *parent = Q_NULLPTR);
    virtual ~MemberWelcome();
};

#endif // MEMBERWELCOME_H
