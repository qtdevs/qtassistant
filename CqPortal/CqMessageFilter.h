#ifndef CQMESSAGEFILTER_H
#define CQMESSAGEFILTER_H

#include "CqInterface.h"

#include "CqPersonInfo.h"
#include "CqMemberInfo.h"

namespace CoolQ {

class ServicePortal;
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
    ServicePortal *portal() const;
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

#endif // CQMESSAGEFILTER_H
