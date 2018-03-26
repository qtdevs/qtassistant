#ifndef CQMESSAGEFILTER_P_H
#define CQMESSAGEFILTER_P_H

#include "CqInterface_p.h"
#include "CqMessageFilter.h"

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

#endif // CQMESSAGEFILTER_P_H
