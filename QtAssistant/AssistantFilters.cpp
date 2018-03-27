#include "AssistantFilters.h"

#include "AssistantModule.h"
#include "AssistantModule_p.h"

#include <QDir>
#include <QtDebug>
#include <QStandardPaths>
#include <QCoreApplication>

/*

AssistantFilters::AssistantFilters(CoolQ::ServiceModule *parent)
    : CoolQ::MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters AssistantFilters::filters() const
{
    return PrivateFilter | GroupFilter | DiscussFilter;
}

QStringList AssistantFilters::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"xxxx");

    return keywords;
}

bool AssistantFilters::privateMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

bool AssistantFilters::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

bool AssistantFilters::discussMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

*/

// class PrivateCleanDataCaches

PrivateCleanDataCaches::PrivateCleanDataCaches(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters PrivateCleanDataCaches::filters() const
{
    return PrivateFilter;
}

QStringList PrivateCleanDataCaches::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"清理缓存");
    keywords << QString(u8"清理数据缓存");
    keywords << QString(u8"缓存清理");

    return keywords;
}

bool PrivateCleanDataCaches::privateMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        if (MasterLevel::ATField == mm->level(0, ev.sender)) {
            QString filePath = module()->resFilePath("image");
            for (auto fileInfo : QDir(filePath).entryInfoList(QDir::Files)) {
                QFile::remove(fileInfo.absoluteFilePath());
            }

            mm->sendPrivateMessage(ev.sender, QString(u8"缓存清理完毕..."));
        }
    }

    return true;
}

// class PrivateRestartComputer

PrivateRestartComputer::PrivateRestartComputer(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters PrivateRestartComputer::filters() const
{
    return PrivateFilter;
}

QStringList PrivateRestartComputer::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"重启系统");
    keywords << QString(u8"系统重启");
    keywords << QString(u8"重启电脑");
    keywords << QString(u8"电脑重启");

    return keywords;
}

bool PrivateRestartComputer::privateMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        if (MasterLevel::ATField == mm->level(0, ev.sender)) {
            mm->sendPrivateMessage(ev.sender, QString(u8"系统即将重启..."));
        }
    }

    return true;
}

// class PrivateCreateStartupShortcut

PrivateCreateStartupShortcut::PrivateCreateStartupShortcut(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters PrivateCreateStartupShortcut::filters() const
{
    return PrivateFilter;
}

QStringList PrivateCreateStartupShortcut::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"创建登录启动");
    keywords << QString(u8"创建自动启动");
    keywords << QString(u8"开启登录启动");
    keywords << QString(u8"开启自动启动");
    keywords << QString(u8"禁用登录启动");
    keywords << QString(u8"禁用自动启动");

    return keywords;
}

bool PrivateCreateStartupShortcut::privateMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        if (MasterLevel::ATField == mm->level(0, ev.sender)) {
            QString execFileName = QCoreApplication::applicationFilePath();
            QString linkPath = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
            if (QFile::link(execFileName, linkPath + QString(u8"/Startup/Qt 助手.lnk"))) {
                mm->sendPrivateMessage(ev.sender, QString(u8"开启自动启动成功..."));
            } else {
                mm->sendPrivateMessage(ev.sender, QString(u8"开启自动启动失败..."));
            }
        }
    }

    return true;
}

// class PrivateDeleteStartupShortcut

PrivateDeleteStartupShortcut::PrivateDeleteStartupShortcut(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters PrivateDeleteStartupShortcut::filters() const
{
    return PrivateFilter;
}

QStringList PrivateDeleteStartupShortcut::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"关闭登录启动");
    keywords << QString(u8"关闭自动启动");
    keywords << QString(u8"禁用登录启动");
    keywords << QString(u8"禁用自动启动");

    return keywords;
}

bool PrivateDeleteStartupShortcut::privateMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        if (MasterLevel::ATField == mm->level(0, ev.sender)) {
            QString linkPath = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
            if (QFile::remove(linkPath + QString(u8"/Startup/Qt 助手.lnk"))) {
                mm->sendPrivateMessage(ev.sender, QString(u8"关闭自动启动成功..."));
            } else {
                mm->sendPrivateMessage(ev.sender, QString(u8"关闭自动启动失败..."));
            }
        }
    }

    return true;
}

// class GroupBanHongbaoAction

GroupBanHongbaoAction::GroupBanHongbaoAction(const QSet<qint64> &groups, CoolQ::ServiceModule *parent)
    : MessageFilter(parent), monitoringGroups(groups)
{
}

CoolQ::MessageFilter::Filters GroupBanHongbaoAction::filters() const
{
    return GroupFilter;
}

bool GroupBanHongbaoAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        if (strncmp(ev.gbkMsg, "[CQ:hb", 6) == 0) {
            if (monitoringGroups.contains(ev.from)) {
                QString msg = QString(u8"<span class=\"warning\">行为警告！！！</span>本群禁止发送任何形式的红包。<br/>因此，您的行为将被禁言 %2 分钟，并通知相关管理员。").arg(60);
                CoolQ::MemberInfo mi = mm->memberInfo(ev.from, ev.sender);
                mm->showDanger(ev.from, mi.safetyName(), msg);
                mm->banGroupMember(ev.from, ev.sender, 3600);

                return true;
            }
        }
    }

    return false;
}

// class GroupCommandsAction

GroupCommandsAction::GroupCommandsAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupCommandsAction::filters() const
{
    return GroupFilter;
}

QStringList GroupCommandsAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"命令清单");
    keywords << QString(u8"命令");
    keywords << QString(u8"帮助");
    keywords << QString(u8"命令列表");

    return keywords;
}

bool GroupCommandsAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupHelpAction(ev, args);
    }

    return true;
}

// class GroupRenameMemberAction

GroupRenameMemberAction::GroupRenameMemberAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupRenameMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupRenameMemberAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"重命名");

    return keywords;
}

bool GroupRenameMemberAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupRenameAction(ev, args);
    }

    return true;
}

// class GroupMemberLevelAction

GroupMemberLevelAction::GroupMemberLevelAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupMemberLevelAction::filters() const
{
    return GroupFilter;
}

QStringList GroupMemberLevelAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"等级");

    return keywords;
}

bool GroupMemberLevelAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupLevelAction(ev, args);
    }

    return true;
}

// class GroupFormatMemberAction

GroupFormatMemberAction::GroupFormatMemberAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupFormatMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupFormatMemberAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"格式化");
    keywords << QString(u8"标准化");

    return keywords;
}

bool GroupFormatMemberAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupFormatAction(ev, args);
    }

    return true;
}

// class GroupBanMemberAction

GroupBanMemberAction::GroupBanMemberAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupBanMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupBanMemberAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"禁言");

    return keywords;
}

bool GroupBanMemberAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupBanAction(ev, args);
    }

    return true;
}

// class GroupKickMemberAction

GroupKickMemberAction::GroupKickMemberAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupKickMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupKickMemberAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"踢出");
    keywords << QString(u8"踢");
    keywords << QString(u8"踢掉");

    return keywords;
}

bool GroupKickMemberAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupKickAction(ev, args);
    }

    return true;
}

// class GroupRaiseMemberAction

GroupRaiseMemberAction::GroupRaiseMemberAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupRaiseMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupRaiseMemberAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"提权");
    keywords << QString(u8"升级");

    return keywords;
}

bool GroupRaiseMemberAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupRaiseAction(ev, args);
    }

    return true;
}

// class GroupUnbanMemberAction

GroupUnbanMemberAction::GroupUnbanMemberAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupUnbanMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupUnbanMemberAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"解禁");

    return keywords;
}

bool GroupUnbanMemberAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupUnbanAction(ev, args);
    }

    return true;
}

// class GroupLowerMemberAction

GroupLowerMemberAction::GroupLowerMemberAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupLowerMemberAction::filters() const
{
    return GroupFilter;
}

QStringList GroupLowerMemberAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"降权");
    keywords << QString(u8"降级");

    return keywords;
}

bool GroupLowerMemberAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupLowerAction(ev, args);
    }

    return true;
}

// class GroupWatchlistAction

GroupWatchlistAction::GroupWatchlistAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupWatchlistAction::filters() const
{
    return GroupFilter;
}

QStringList GroupWatchlistAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"监控表");
    keywords << QString(u8"监视表");

    return keywords;
}

bool GroupWatchlistAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupWatchlistAction(ev, args);
    }

    return true;
}

// class GroupBlacklistAction

GroupBlacklistAction::GroupBlacklistAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupBlacklistAction::filters() const
{
    return GroupFilter;
}

QStringList GroupBlacklistAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"黑名单");

    return keywords;
}

bool GroupBlacklistAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupBlacklistAction(ev, args);
    }

    return true;
}

// class GroupMemberInfoAction

GroupMemberInfoAction::GroupMemberInfoAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupMemberInfoAction::filters() const
{
    return GroupFilter;
}

QStringList GroupMemberInfoAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"成员信息");

    return keywords;
}

bool GroupMemberInfoAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    if (auto mm = qobject_cast<AssistantModule *>(module())) {
        QStringList args = CoolQ::trGbk(&ev.gbkMsg[i]).split(' ', QString::SkipEmptyParts);
        mm->groupMemberAction(ev, args);
    }

    return true;
}

// class GroupRenameMemberHelpAction

GroupRenameMemberHelpAction::GroupRenameMemberHelpAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupRenameMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupRenameMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"重命名命令");
    keywords << QString(u8"重命名的用法");
    keywords << QString(u8"重命名用法");

    return keywords;
}

bool GroupRenameMemberHelpAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module()))
        mm->groupRenameHelpAction(ev.from);

    return true;
}

// class GroupMemberLevelHelpAction

GroupMemberLevelHelpAction::GroupMemberLevelHelpAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupMemberLevelHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupMemberLevelHelpAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"等级查询");

    return keywords;
}

bool GroupMemberLevelHelpAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module()))
        mm->groupLevelHelpAction(ev.from);

    return true;
}

// class GroupFormatMemberHelpAction

GroupFormatMemberHelpAction::GroupFormatMemberHelpAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupFormatMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupFormatMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"格式化命令");
    keywords << QString(u8"格式化的用法");
    keywords << QString(u8"格式化用法");

    return keywords;
}

bool GroupFormatMemberHelpAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module()))
        mm->groupFormatHelpAction(ev.from);

    return true;
}

// class GroupBanMemberHelpAction

GroupBanMemberHelpAction::GroupBanMemberHelpAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupBanMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupBanMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"禁言命令");
    keywords << QString(u8"禁言的用法");
    keywords << QString(u8"禁言用法");

    return keywords;
}

bool GroupBanMemberHelpAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module()))
        mm->groupBanHelpAction(ev.from);

    return true;
}

// class GroupKickMemberHelpAction

GroupKickMemberHelpAction::GroupKickMemberHelpAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupKickMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupKickMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"踢出命令");
    keywords << QString(u8"踢出的用法");
    keywords << QString(u8"踢出用法");

    keywords << QString(u8"踢人命令");
    keywords << QString(u8"踢人的用法");
    keywords << QString(u8"踢人用法");

    return keywords;
}

bool GroupKickMemberHelpAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module()))
        mm->groupKickHelpAction(ev.from);

    return true;
}

// class GroupRaiseMemberHelpAction

GroupRaiseMemberHelpAction::GroupRaiseMemberHelpAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupRaiseMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupRaiseMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"提权命令");
    keywords << QString(u8"提权的用法");
    keywords << QString(u8"提权用法");

    return keywords;
}

bool GroupRaiseMemberHelpAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module()))
        mm->groupRaiseHelpAction(ev.from);

    return true;
}

// class GroupUnbanMemberHelpAction

GroupUnbanMemberHelpAction::GroupUnbanMemberHelpAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupUnbanMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupUnbanMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"解禁命令");
    keywords << QString(u8"解禁的用法");
    keywords << QString(u8"解禁用法");

    return keywords;
}

bool GroupUnbanMemberHelpAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module()))
        mm->groupUnbanHelpAction(ev.from);

    return true;
}

// class GroupLowerMemberHelpAction

GroupLowerMemberHelpAction::GroupLowerMemberHelpAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupLowerMemberHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupLowerMemberHelpAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"降权命令");
    keywords << QString(u8"降权的用法");
    keywords << QString(u8"降权用法");

    return keywords;
}

bool GroupLowerMemberHelpAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module()))
        mm->groupLowerHelpAction(ev.from);

    return true;
}

// class GroupWatchlistHelpAction

GroupWatchlistHelpAction::GroupWatchlistHelpAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupWatchlistHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupWatchlistHelpAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"监控表命令");
    keywords << QString(u8"监控表的用法");
    keywords << QString(u8"监控表用法");

    keywords << QString(u8"监视表命令");
    keywords << QString(u8"监视表的用法");
    keywords << QString(u8"监视表用法");

    return keywords;
}

bool GroupWatchlistHelpAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module()))
        mm->groupWatchlistHelpAction(ev.from);

    return true;
}

// class GroupBlacklistHelpAction

GroupBlacklistHelpAction::GroupBlacklistHelpAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupBlacklistHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupBlacklistHelpAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"黑名单命令");
    keywords << QString(u8"黑名单的用法");
    keywords << QString(u8"黑名单用法");

    return keywords;
}

bool GroupBlacklistHelpAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module()))
        mm->groupBlacklistHelpAction(ev.from);

    return true;
}

// class GroupMemberInfoHelpAction

GroupMemberInfoHelpAction::GroupMemberInfoHelpAction(CoolQ::ServiceModule *parent)
    : MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters GroupMemberInfoHelpAction::filters() const
{
    return GroupFilter;
}

QStringList GroupMemberInfoHelpAction::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"成员信息命令");

    return keywords;
}

bool GroupMemberInfoHelpAction::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<AssistantModule *>(module()))
        mm->groupMemberHelpAction(ev.from);

    return true;
}
