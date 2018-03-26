#include "ManagementFilters.h"

#include "ManagementModule.h"
#include "ManagementModule_p.h"

#include <QtDebug>

/*

ManagementFilters::ManagementFilters(CoolQ::ServiceModule *parent)
    : CoolQ::MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters ManagementFilters::filters() const
{
    return PrivateFilter | GroupFilter | DiscussFilter;
}

QStringList ManagementFilters::keywords() const
{
    QStringList keywords;

    keywords << tr("xxxx");

    return keywords;
}

bool ManagementFilters::privateMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

bool ManagementFilters::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

bool ManagementFilters::discussMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

*/

// class PrivateRestartComputer

PrivateRestartComputer::PrivateRestartComputer(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters PrivateRestartComputer::filters() const
{
    return PrivateFilter;
}

QStringList PrivateRestartComputer::keywords() const
{
    QStringList keywords;

    keywords << tr("重启系统");
    keywords << tr("系统重启");
    keywords << tr("重启电脑");
    keywords << tr("电脑重启");

    return keywords;
}

bool PrivateRestartComputer::privateMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        if (MasterLevel::ATField == mm->level(0, ev.sender)) {
            mm->sendPrivateMessage(ev.sender, tr("系统即将重启..."));
        }
    }

    return true;
}

// class PrivateCreateStartupShortcut

PrivateCreateStartupShortcut::PrivateCreateStartupShortcut(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters PrivateCreateStartupShortcut::filters() const
{
    return PrivateFilter;
}

QStringList PrivateCreateStartupShortcut::keywords() const
{
    QStringList keywords;

    keywords << tr("启动登录启动");
    keywords << tr("启动自动启动");
    keywords << tr("开启登录启动");
    keywords << tr("开启自动启动");
    keywords << tr("禁用登录启动");
    keywords << tr("禁用自动启动");

    return keywords;
}

bool PrivateCreateStartupShortcut::privateMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        if (MasterLevel::ATField == mm->level(0, ev.sender)) {
            mm->sendPrivateMessage(ev.sender, tr("已经开启登录启动"));
        }
    }

    return true;
}

// class PrivateDeleteStartupShortcut

PrivateDeleteStartupShortcut::PrivateDeleteStartupShortcut(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters PrivateDeleteStartupShortcut::filters() const
{
    return PrivateFilter;
}

QStringList PrivateDeleteStartupShortcut::keywords() const
{
    QStringList keywords;

    keywords << tr("关闭登录启动");
    keywords << tr("关闭自动启动");
    keywords << tr("禁用登录启动");
    keywords << tr("禁用自动启动");

    return keywords;
}

bool PrivateDeleteStartupShortcut::privateMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        if (MasterLevel::ATField == mm->level(0, ev.sender)) {
            mm->sendPrivateMessage(ev.sender, tr("已经关闭登录启动"));
        }
    }

    return true;
}

// class GroupBanHongbaoAction

GroupBanHongbaoAction::GroupBanHongbaoAction(const QSet<qint64> &groups, ServiceModule *parent)
    : MessageFilter(parent), monitoringGroups(groups)
{
}

MessageFilter::Filters GroupBanHongbaoAction::filters() const
{
    return GroupFilter;
}

bool GroupBanHongbaoAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        if (strncmp(ev.gbkMsg, "[CQ:hb", 6) == 0) {
            if (monitoringGroups.contains(ev.from)) {
                QString msg = tr("<span class=\"warning\">行为警告！！！</span>本群禁止发送任何形式的红包。<br/>因此，您的行为将被禁言 %2 分钟，并通知相关管理员。").arg(60);
                MemberInfo mi = mm->memberInfo(ev.from, ev.sender);
                mm->showDanger(ev.from, mi.safetyName(), msg);
                mm->banGroupMember(ev.from, ev.sender, 3600);

                return true;
            }
        }
    }

    return false;
}

// class GroupCommandsAction

GroupCommandsAction::GroupCommandsAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupCommandsAction::filters() const
{
    return GroupFilter;
}

QStringList GroupCommandsAction::keywords() const
{
    QStringList keywords;

    keywords << tr("命令清单");
    keywords << tr("命令");
    keywords << tr("命令列表");

    return keywords;
}

bool GroupCommandsAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        QStringList args = trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupHelpAction(ev, args);
    }

    return true;
}

// class GroupBanMemberAction

GroupBanMemberAction::GroupBanMemberAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupBanMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupBanMemberAction::keywords() const
{
    QStringList keywords;

    keywords << tr("禁言");

    return keywords;
}

bool GroupBanMemberAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        QStringList args = trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupBanAction(ev, args);
    }

    return true;
}

// class GroupKickMemberAction

GroupKickMemberAction::GroupKickMemberAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupKickMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupKickMemberAction::keywords() const
{
    QStringList keywords;

    keywords << tr("踢出");
    keywords << tr("踢");
    keywords << tr("踢掉");

    return keywords;
}

bool GroupKickMemberAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        QStringList args = trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupKickAction(ev, args);
    }

    return true;
}

// class GroupRaiseMemberAction

GroupRaiseMemberAction::GroupRaiseMemberAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupRaiseMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupRaiseMemberAction::keywords() const
{
    QStringList keywords;

    keywords << tr("提权");
    keywords << tr("升级");

    return keywords;
}

bool GroupRaiseMemberAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        QStringList args = trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupRaiseAction(ev, args);
    }

    return true;
}

// class GroupUnbanMemberAction

GroupUnbanMemberAction::GroupUnbanMemberAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupUnbanMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupUnbanMemberAction::keywords() const
{
    QStringList keywords;

    keywords << tr("解禁");

    return keywords;
}

bool GroupUnbanMemberAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        QStringList args = trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupUnbanAction(ev, args);
    }

    return true;
}

// class GroupLowerMemberAction

GroupLowerMemberAction::GroupLowerMemberAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupLowerMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupLowerMemberAction::keywords() const
{
    QStringList keywords;

    keywords << tr("降权");
    keywords << tr("降级");

    return keywords;
}

bool GroupLowerMemberAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        QStringList args = trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupLowerAction(ev, args);
    }

    return true;
}

// class GroupWatchlistAction

GroupWatchlistAction::GroupWatchlistAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupWatchlistAction::filters() const
{
    return GroupFilter;
}

QStringList GroupWatchlistAction::keywords() const
{
    QStringList keywords;

    keywords << tr("监控表");
    keywords << tr("监视表");

    return keywords;
}

bool GroupWatchlistAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        QStringList args = trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupWatchlistAction(ev, args);
    }

    return true;
}
// class GroupBlacklistAction

GroupBlacklistAction::GroupBlacklistAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupBlacklistAction::filters() const
{
    return GroupFilter;
}

QStringList GroupBlacklistAction::keywords() const
{
    QStringList keywords;

    keywords << tr("黑名单");

    return keywords;
}

bool GroupBlacklistAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    if (auto mm = qobject_cast<ManagementModule *>(module())) {
        QStringList args = trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupBlacklistAction(ev, args);
    }

    return true;
}

// class GroupBanMemberHelpAction

GroupBanMemberHelpAction::GroupBanMemberHelpAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupBanMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupBanMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << tr("禁言命令");
    keywords << tr("禁言的用法");
    keywords << tr("禁言用法");

    return keywords;
}

bool GroupBanMemberHelpAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module()))
        mm->groupBanHelpAction(ev.from);

    return true;
}

// class GroupKickMemberHelpAction

GroupKickMemberHelpAction::GroupKickMemberHelpAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupKickMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupKickMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << tr("踢出命令");
    keywords << tr("踢出的用法");
    keywords << tr("踢出用法");

    keywords << tr("踢人命令");
    keywords << tr("踢人的用法");
    keywords << tr("踢人用法");

    return keywords;
}

bool GroupKickMemberHelpAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module()))
        mm->groupKickHelpAction(ev.from);

    return true;
}

// class GroupRaiseMemberHelpAction

GroupRaiseMemberHelpAction::GroupRaiseMemberHelpAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupRaiseMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupRaiseMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << tr("提权命令");
    keywords << tr("提权的用法");
    keywords << tr("提权用法");

    return keywords;
}

bool GroupRaiseMemberHelpAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module()))
        mm->groupRaiseHelpAction(ev.from);

    return true;
}

// class GroupUnbanMemberHelpAction

GroupUnbanMemberHelpAction::GroupUnbanMemberHelpAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupUnbanMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupUnbanMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << tr("解禁命令");
    keywords << tr("解禁的用法");
    keywords << tr("解禁用法");

    return keywords;
}

bool GroupUnbanMemberHelpAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module()))
        mm->groupUnbanHelpAction(ev.from);

    return true;
}

// class GroupLowerMemberHelpAction

GroupLowerMemberHelpAction::GroupLowerMemberHelpAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupLowerMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupLowerMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << tr("降权命令");
    keywords << tr("降权的用法");
    keywords << tr("降权用法");

    return keywords;
}

bool GroupLowerMemberHelpAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module()))
        mm->groupLowerHelpAction(ev.from);

    return true;
}

// class GroupWatchlistHelpAction

GroupWatchlistHelpAction::GroupWatchlistHelpAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupWatchlistHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupWatchlistHelpAction::keywords() const
{
    QStringList keywords;

    keywords << tr("监控表命令");
    keywords << tr("监控表的用法");
    keywords << tr("监控表用法");

    keywords << tr("监视表命令");
    keywords << tr("监视表的用法");
    keywords << tr("监视表用法");

    return keywords;
}

bool GroupWatchlistHelpAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module()))
        mm->groupWatchlistHelpAction(ev.from);

    return true;
}

// class GroupBlacklistHelpAction

GroupBlacklistHelpAction::GroupBlacklistHelpAction(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters GroupBlacklistHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupBlacklistHelpAction::keywords() const
{
    QStringList keywords;

    keywords << tr("黑名单命令");
    keywords << tr("黑名单的用法");
    keywords << tr("黑名单用法");

    return keywords;
}

bool GroupBlacklistHelpAction::groupMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManagementModule *>(module()))
        mm->groupBlacklistHelpAction(ev.from);

    return true;
}
