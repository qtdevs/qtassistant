#ifndef MANAGEMENTFILTERS_H
#define MANAGEMENTFILTERS_H

#include <QSet>

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

// class PrivateRestartComputer

class PrivateRestartComputer : public MessageFilter
{
    Q_OBJECT

public:
    explicit PrivateRestartComputer(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const MessageEvent &ev) final;
};

// class PrivateCreateStartupShortcut

class PrivateCreateStartupShortcut : public MessageFilter
{
    Q_OBJECT

public:
    explicit PrivateCreateStartupShortcut(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const MessageEvent &ev) final;
};

// class PrivateDeleteStartupShortcut

class PrivateDeleteStartupShortcut : public MessageFilter
{
    Q_OBJECT

public:
    explicit PrivateDeleteStartupShortcut(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupBanHongbaoAction

class GroupBanHongbaoAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupBanHongbaoAction(const QSet<qint64> &groups, ServiceModule *parent);

public:
    Filters filters() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;

private:
    QSet<qint64> monitoringGroups;
};

// class GroupCommandsAction

class GroupCommandsAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupCommandsAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupBanMemberAction

class GroupBanMemberAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupBanMemberAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class KillMemberAction

class GroupKickMemberAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupKickMemberAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupRaiseMemberAction

class GroupRaiseMemberAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupRaiseMemberAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupUnbanMemberAction

class GroupUnbanMemberAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupUnbanMemberAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupLowerMemberAction

class GroupLowerMemberAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupLowerMemberAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupWatchlistAction

class GroupWatchlistAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupWatchlistAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupBlacklistAction

class GroupBlacklistAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupBlacklistAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupBanMemberHelpAction

class GroupBanMemberHelpAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupBanMemberHelpAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupKickMemberHelpAction

class GroupKickMemberHelpAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupKickMemberHelpAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupRaiseMemberHelpAction

class GroupRaiseMemberHelpAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupRaiseMemberHelpAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupUnbanMemberHelpAction

class GroupUnbanMemberHelpAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupUnbanMemberHelpAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupLowerMemberHelpAction

class GroupLowerMemberHelpAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupLowerMemberHelpAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupWatchlistHelpAction

class GroupWatchlistHelpAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupWatchlistHelpAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

// class GroupBlacklistHelpAction

class GroupBlacklistHelpAction : public MessageFilter
{
    Q_OBJECT

public:
    explicit GroupBlacklistHelpAction(ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const MessageEvent &ev) final;
};

#endif // MANAGEMENTFILTERS_H
