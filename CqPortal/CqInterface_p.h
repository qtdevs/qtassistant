#ifndef CQINTERFACE_P_H
#define CQINTERFACE_P_H

#include "CqInterface.h"

namespace CoolQ {

class InterfacePrivate
{
    Q_DECLARE_PUBLIC(Interface)

public:
    InterfacePrivate();
    virtual ~InterfacePrivate();
protected:
    Interface *q_ptr;
};

} // namespace CoolQ

#endif // CQINTERFACE_P_H
