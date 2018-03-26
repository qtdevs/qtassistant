#include "CqMessageFilter.h"
#include "CqMessageFilter_p.h"

#include "CqServiceEngine.h"
#include "CqServiceEngine_p.h"

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
    d->engine = parent->engine();
    d->module = parent;
}

MessageFilter::MessageFilter(ServiceModule *parent)
    : Interface(*new MessageFilterPrivate(), parent)
{
    if (auto module = ServiceModulePrivate::get(parent)) {
        module->installFilter(this);
    }

    Q_D(MessageFilter);
    d->engine = parent->engine();
    d->module = parent;
}

MessageFilter::~MessageFilter()
{
}

ServiceEngine *MessageFilter::engine() const
{
    Q_D(const MessageFilter);

    return d->engine;
}

ServiceModule *MessageFilter::module() const
{
    Q_D(const MessageFilter);

    return d->module;
}

QStringList MessageFilter::keywords() const
{
    return QStringList();
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
    : engine(nullptr)
    , module(nullptr)
{
}

MessageFilterPrivate::~MessageFilterPrivate()
{
}

} // namespace CoolQ
