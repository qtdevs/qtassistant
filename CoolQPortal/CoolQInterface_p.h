#ifndef COOLQINTERFACE_P_H
#define COOLQINTERFACE_P_H

#include "CoolQInterface.h"

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

#endif // COOLQINTERFACE_P_H
