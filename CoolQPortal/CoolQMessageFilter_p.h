#ifndef COOLQMESSAGEFILTER_P_H
#define COOLQMESSAGEFILTER_P_H

#include "CoolQInterface_p.h"
#include "CoolQMessageFilter.h"

namespace CoolQ {

class MessageFilterPrivate : public InterfacePrivate
{
    Q_DECLARE_PUBLIC(MessageFilter)

public:
    MessageFilterPrivate();
    virtual ~MessageFilterPrivate();

protected:
    ServiceEngine *engine;
    ServiceModule *module;
};

} // namespace CoolQ

#endif // COOLQMESSAGEFILTER_P_H
