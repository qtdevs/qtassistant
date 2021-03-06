﻿#ifndef ASSISTANTFILTERS_H
#define ASSISTANTFILTERS_H

#include <QSet>

#include "CoolQMessageFilter.h"

/*

class AssistantFilters : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit AssistantFilters(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
    bool discussMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

*/

// class PrivateCleanDataCaches

class PrivateCleanDataCaches : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit PrivateCleanDataCaches(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class PrivateRestartComputer

class PrivateRestartComputer : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit PrivateRestartComputer(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class PrivateCreateStartupShortcut

class PrivateCreateStartupShortcut : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit PrivateCreateStartupShortcut(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class PrivateDeleteStartupShortcut

class PrivateDeleteStartupShortcut : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit PrivateDeleteStartupShortcut(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool privateMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupBanHongbaoAction

class GroupBanHongbaoAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupBanHongbaoAction(const QSet<qint64> &groups, CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;

private:
    QSet<qint64> monitoringGroups;
};

// class GroupCommandsAction

class GroupCommandsAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupCommandsAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupRenameMemberAction

class GroupRenameMemberAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupRenameMemberAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupFormatMemberAction

class GroupFormatMemberAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupFormatMemberAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupBanMemberAction

class GroupBanMemberAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupBanMemberAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class KickMemberAction

class GroupKickMemberAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupKickMemberAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupUnbanMemberAction

class GroupUnbanMemberAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupUnbanMemberAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupWatchlistAction

class GroupWatchlistAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupWatchlistAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupWatchlistAction

class GroupAddWatchlistAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupAddWatchlistAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupWatchlistAction

class GroupRemoveWatchlistAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupRemoveWatchlistAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupBlacklistAction

class GroupBlacklistAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupBlacklistAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupAddBlacklistAction

class GroupAddBlacklistAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupAddBlacklistAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupRemoveBlacklistAction

class GroupRemoveBlacklistAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupRemoveBlacklistAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupMemberInfoAction

class GroupMemberInfoAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupMemberInfoAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupRenameMemberHelpAction

class GroupRenameMemberHelpAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupRenameMemberHelpAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupFormatMemberHelpAction

class GroupFormatMemberHelpAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupFormatMemberHelpAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupBanMemberHelpAction

class GroupBanMemberHelpAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupBanMemberHelpAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupKickMemberHelpAction

class GroupKickMemberHelpAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupKickMemberHelpAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupUnbanMemberHelpAction

class GroupUnbanMemberHelpAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupUnbanMemberHelpAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupWatchlistHelpAction

class GroupWatchlistHelpAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupWatchlistHelpAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupBlacklistHelpAction

class GroupBlacklistHelpAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupBlacklistHelpAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

// class GroupMemberInfoHelpAction

class GroupMemberInfoHelpAction : public CoolQ::MessageFilter
{
    Q_OBJECT

public:
    explicit GroupMemberInfoHelpAction(CoolQ::ServiceModule *parent);

public:
    Filters filters() const final;
    QStringList keywords() const final;

public:
    bool groupMessageFilter(int i, const CoolQ::MessageEvent &ev) final;
};

#endif // ASSISTANTFILTERS_H
