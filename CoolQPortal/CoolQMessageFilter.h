#ifndef COOLQMESSAGEFILTER_H
#define COOLQMESSAGEFILTER_H

#include "CoolQInterface.h"

#include "CoolQPersonInfo.h"
#include "CoolQMemberInfo.h"

namespace CoolQ {

class ServiceEngine;
class ServiceModule;

class MessageFilterPrivate;
class MessageFilter : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MessageFilter)

protected:
    MessageFilter(MessageFilterPrivate &dd, ServiceModule *parent);
public:
    explicit MessageFilter(ServiceModule *parent);
public:
    virtual ~MessageFilter();

public:
    ServiceEngine *engine() const;
    ServiceModule *module() const;

public:
    enum FilterType {
        PrivateFilter = 0x01,
        GroupFilter   = 0x02,
        DiscussFilter = 0x04,
    };
    Q_DECLARE_FLAGS(Filters, FilterType)

public:
    virtual Filters filters() const = 0;
    virtual QStringList keywords() const;
    virtual QString description() const;

public:
    virtual bool privateMessageFilter(int i, const MessageEvent &ev);
    virtual bool groupMessageFilter(int i, const MessageEvent &ev);
    virtual bool discussMessageFilter(int i, const MessageEvent &ev);
};

} // namespace CoolQ

#endif // COOLQMESSAGEFILTER_H
