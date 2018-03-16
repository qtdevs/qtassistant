#include "CqMessageFilter.h"
#include "CqMessageFilter_p.h"

#include "CqServicePortal.h"
#include "CqServicePortal_p.h"

#include "CqServiceModule.h"
#include "CqServiceModule_p.h"

namespace CoolQ {

// class MessageFilter

MessageFilter::MessageFilter(MessageFilterPrivate &dd, ServiceModule *parent)
    : Interface(dd, parent)
{
    if (auto module = ServiceModulePrivate::get(parent)) {
        module->installFilter(this);
    }

    Q_D(MessageFilter);
    d->portal = parent->portal();
    d->module = parent;
}

MessageFilter::MessageFilter(ServiceModule *parent)
    : Interface(*new MessageFilterPrivate(), parent)
{
    if (auto module = ServiceModulePrivate::get(parent)) {
        module->installFilter(this);
    }

    Q_D(MessageFilter);
    d->portal = parent->portal();
    d->module = parent;
}

MessageFilter::~MessageFilter()
{
}

ServicePortal *MessageFilter::portal() const
{
    Q_D(const MessageFilter);

    return d->portal;
}

ServiceModule *MessageFilter::module() const
{
    Q_D(const MessageFilter);

    return d->module;
}

QString MessageFilter::description() const
{
    return QString();
}

bool MessageFilter::privateMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

bool MessageFilter::groupMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

bool MessageFilter::discussMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

// class MessageFilterPrivate

MessageFilterPrivate::MessageFilterPrivate()
    : portal(nullptr)
    , module(nullptr)
{
}

MessageFilterPrivate::~MessageFilterPrivate()
{
}

} // namespace CoolQ
