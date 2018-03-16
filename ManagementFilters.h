#ifndef MANAGEMENTFILTERS_H
#define MANAGEMENTFILTERS_H

#include "CqMessageFilter.h"

using namespace CoolQ;

/*

class ManagementFilters : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit ManagementFilters(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
    bool discussMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

*/

// class RestartComputer

class RestartComputer : public MessageFilter
{
    Q_OBJECT

public:
    explicit RestartComputer(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const MessageEvent &ev) final;
};

// class CreateStartupShortcut

class CreateStartupShortcut : public MessageFilter
{
    Q_OBJECT

public:
    explicit CreateStartupShortcut(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const MessageEvent &ev) final;
};

// class DeleteStartupShortcut

class DeleteStartupShortcut : public MessageFilter
{
    Q_OBJECT

public:
    explicit DeleteStartupShortcut(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const MessageEvent &ev) final;
};

#endif // MANAGEMENTFILTERS_H
