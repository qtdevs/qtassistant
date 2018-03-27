#include "AssistantModule.h"
#include "AssistantModule_p.h"

#include <QRegularExpression>
#include <QStringBuilder>
#include <QTextStream>
#include <QtDebug>

#include "SqlDatas/MasterLevels.h"
#include "SqlDatas/MemberBlacklist.h"
#include "SqlDatas/MemberWatchlist.h"

// class AssistantModule

bool AssistantModule::privateMessageEvent(const CoolQ::MessageEvent &ev)
{
    if (CoolQ::ServiceModule::privateMessageEvent(ev)) {
        return true;
    }

    return false;
}

bool AssistantModule::groupMessageEvent(const CoolQ::MessageEvent &ev)
{
    Q_D(AssistantModule);

    if (!d->managedGroups.contains(ev.from)) {
        return false;
    }

    // 移出监控表，不再进行监视。
    if (d->watchlist->contains(ev.from, ev.sender)) {
        d->watchlist->removeMember(ev.from, ev.sender);
    }

    // 黑名单检查，如果发现匹配，直接踢出。
    if (d->blacklist->contains(ev.from, ev.sender)) {
        kickGroupMember(ev.from, ev.sender, false);
        return true;
    }

    return CoolQ::ServiceModule::groupMessageEvent(ev);

    /*
    // 分离参数。
    QStringList args;
    if ((ev.gbkMsg[0] == '!')) {
        QString msgs = CoolQ::trGbk(ev.gbkMsg).mid(1);
        args = msgs.split(' ', QString::SkipEmptyParts);
    } else if (strncmp(ev.gbkMsg, "sudo", 4) == 0) {
        QString msgs = CoolQ::trGbk(ev.gbkMsg).mid(4);
        args = msgs.split(' ', QString::SkipEmptyParts);
    } else if ((-93 == ev.gbkMsg[0]) && (-95 == ev.gbkMsg[1])) {
        QString msgs = CoolQ::trGbk(ev.gbkMsg).mid(1);
        args = msgs.split(' ', QString::SkipEmptyParts);
    }

    // 命令派发。
    if (!args.isEmpty()) {
        QString c = args.value(0);

        if ((c == "l") || (c == "level")) {
            groupLevelAction(ev, args.mid(1));
            return true;
        }
        if ((c == "r") || (c == "rename")) {
            groupRenameAction(ev, args.mid(1));
            return true;
        }
        if ((c == "f") || (c == "format")) {
            groupFormatAction(ev, args.mid(1));
            return true;
        }
        if ((c == "m") || (c == "member")) {
            groupMemberAction(ev, args.mid(1));
            return true;
        }

        if (c == "show") {
            showWelcomes(ev.from, ev.sender);
            return true;
        }
        if (c == "save") {
            saveWelcomes(ev.from, ev.sender);
            return true;
        }
    }
    */

    return false;
}

bool AssistantModule::discussMessageEvent(const CoolQ::MessageEvent &ev)
{
    if (CoolQ::ServiceModule::discussMessageEvent(ev)) {
        return true;
    }

    return false;
}

void AssistantModule::groupHelpAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_UNUSED(args);

    Q_D(AssistantModule);

    // 普通成员不应答。
    MasterLevel level = d->levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 五级管理及以上。
    if (level > MasterLevel::Master5) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>  </code>" << QString(u8"重命名命令") << QString(u8" <code>(5+): 重命名 [@成员]  新名片</code>\n");
    ts << "<code>  </code>" << QString(u8"等级查询") << QString(u8"    <code>(5+): 等级 [参数] [@成员] [@...]</code>\n");
    ts << "<code>  </code>" << QString(u8"格式化命令") << QString(u8" <code>(5+): 格式化 [@成员] [@...]</code>\n");
    ts << "\n";
    ts << "<code>  </code>" << QString(u8"禁言命令") << QString(u8" <code>(5+): 禁言 @成员 @...</code>\n");
    ts << "<code>  </code>" << QString(u8"解禁命令") << QString(u8" <code>(5+): 解禁 @成员 @...</code>\n");
    ts << "<code>  </code>" << QString(u8"踢出命令") << QString(u8" <code>(3+): 踢出 @成员 @...</code>\n");
    ts << "<code>  </code>" << QString(u8"提权命令") << QString(u8" <code>(1+): 提权 @成员 @...</code>\n");
    ts << "<code>  </code>" << QString(u8"降权命令") << QString(u8" <code>(1+): 降权 @成员 @...</code>\n");
    ts << "\n";
    ts << "<code>  </code>" << QString(u8"监控表命令") << QString(u8" <code>(1+): 监控表 [+] [@成员 @...]</code>\n");
    ts << "<code>  </code>" << QString(u8"黑名单命令") << QString(u8" <code>(1+): 黑名单 [-] [QQ号码 ...]</code>\n");
    ts << "</pre>";

    ts.flush();

    showPrompt(ev.from, QString(u8"命令清单"), usage);
}

void AssistantModule::groupRenameAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    MasterLevel level = d->levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 五级管理及以上。
    if (level > MasterLevel::Master5) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // !!! 由于此操作的特殊性，命令行解析自行分析

    QString nameCard = CoolQ::trGbk(ev.gbkMsg);

    LevelInfoList ll;
    bool prefixFound = false;
    bool invalidArgs = false;
    for (const QString &arg : args) {
        if (arg.startsWith("[CQ:at")) {
            int i = arg.lastIndexOf("]");
            QString uid = arg.mid(10, i - 10);
            ll.append(LevelInfo(uid.toLongLong(),
                                MasterLevel::Unknown));

            // 如果找到了号码，同时找到了部分新名片，我们认为它是无效的操作。
            if (prefixFound) {
                invalidArgs = true;
                break;
            }

            // 如果只找到一个号码，我们认为号码以后的部分都是新名片；否则就是无效的操作。
            if (ll.count() == 1) {
                int r = nameCard.indexOf(arg);
                nameCard = nameCard.mid(r + arg.count());
            } else {
                invalidArgs = true;
                break;
            }
        } else if (ll.isEmpty()) {
            // 在还没有找到号码的情况下，我们认为这是新名片的开始。
            if (!prefixFound) {
                int r = nameCard.indexOf(arg);
                nameCard = nameCard.mid(r);
            }
            prefixFound = true;
        }
    }
    if (args.isEmpty() || invalidArgs) {
        groupRenameHelpAction(ev.from);
        return;
    }

    // 管理等级检查

    if (!ll.isEmpty()) {
    }

    // 在这里，我们对新名片做规范化处理。
    d->formatNameCard(nameCard);

    if (nameCard.isEmpty()) {
        groupRenameHelpAction(ev.from);
        return;
    }

    // 执行具体操作

    if (ll.isEmpty()) {
        renameGroupMember(ev.from, ev.sender, nameCard);
    } else {
        renameGroupMember(ev.from, ll.at(0).uid, nameCard);
    }

    showSuccess(ev.from, QString(u8"修改名片"), QString(u8"新的名片：%1").arg(nameCard));
}

void AssistantModule::groupLevelAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    LevelInfoList ll = d->findUsers(args);
    MasterLevel level = d->levels->level(ev.from, ev.sender);

    // 分析其他命令行参数

    bool argvGlobal = false;
    bool argvList = false;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);

        if ((argv == "g") || (argv == QString(u8"全局"))) {
            if (argvGlobal) {
                invalidArg = true;
                break;
            }
            argvGlobal = true;
        } else if ((argv == "l") || (argv == QString(u8"列表")) || (argv == QString(u8"名单"))) {
            if (argvList) {
                invalidArg = true;
                break;
            }
            argvList = true;
        } else {
            invalidArg = true;
            break;
        }
    }
    if (invalidArg) {
        groupLevelHelpAction(ev.from);
        return;
    }

    // 执行具体操作

    if (ll.isEmpty()) {
        if (argvList) {
            if (MasterLevel::Unknown != level) {
                ll = d->levels->levels(argvGlobal ? 0 : ev.from);
                if (ll.isEmpty()) {
                    if (argvGlobal) {
                        showPrompt(ev.from, QString(u8"跨群等级列表"), QString(u8"等级列表中没有任何成员"));
                    } else {
                        showPrompt(ev.from, QString(u8"本群等级列表"), QString(u8"等级列表中没有任何成员"));
                    }
                } else {
                    if (argvGlobal) {
                        showPromptList(ev.from, QString(u8"跨群等级列表"), ll, true);
                    } else {
                        showPromptList(ev.from, QString(u8"本群等级列表"), ll, true);
                    }
                }
            }
        } else {
            MasterLevel level = d->levels->level(argvGlobal ? 0 : ev.from, ev.sender);
            ll.append(LevelInfo(ev.sender, level));
            if (argvGlobal) {
                showPromptList(ev.from, QString(u8"跨群等级"), ll, true);
            } else {
                showPromptList(ev.from, QString(u8"本群等级"), ll, true);
            }
        }
    } else if (!argvList) {
        if (MasterLevel::Unknown != level) {
            d->levels->update(argvGlobal ? 0 : ev.from, ll);
            if (argvGlobal) {
                showPromptList(ev.from, QString(u8"跨群等级"), ll, true);
            } else {
                showPromptList(ev.from, QString(u8"本群等级"), ll, true);
            }
        }
    } else {
        groupLevelHelpAction(ev.from);
    }
}

void AssistantModule::groupFormatAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    MasterLevel level = d->levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 五级管理及以上。
    if (level > MasterLevel::Master5) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    LevelInfoList ll = d->findUsers(args);

    QRegularExpression correctNameCard("^[\\[【][^\\[\\]【】]+[\\]】][^\\[\\]【】]+$");
    QRegularExpression correctLocation("^[\\[【][^\\[\\]【】]+[\\]】]$");
    QRegularExpression correctNickName("^[^\\[\\]【】]+$");

    // 管理等级检查

    if (!ll.isEmpty()) {
        // 管理等级检查

        // 检查一：管理只能处理比自己等级低的成员。
        LevelInfoList ml;
        d->levels->update(ev.from, ll);
        for (const LevelInfo &li : ll) {
            if (li.level <= level) {
                ml << li;
            }
        }
        if (!ml.isEmpty()) {
            showDangerList(ev.from, QString(u8"你没有权限执行此操作"), ml, true);
            return;
        }

        // 执行具体操作

        for (const LevelInfo &li : ll) {
            // 尝试修改昵称。
            CoolQ::MemberInfo mi = memberInfo(ev.from, li.uid, false);
            if (mi.isValid()) {
                QString nameCard = mi.nameCard().remove(' ');
                d->safetyNameCard(nameCard);

                if (!correctNameCard.match(nameCard).hasMatch()) {
                    QString nickName = mi.nickName().trimmed();
                    QString location = mi.location().trimmed();
                    d->safetyNameCard(nickName);
                    if (location.isEmpty()) {
                        location = QString(u8"所在地");
                    }

                    if (correctLocation.match(nameCard).hasMatch()) {
                        nameCard = nameCard % nickName;
                    } else if (correctNickName.match(nameCard).hasMatch()) {
                        nameCard = '[' % location % ']' % nameCard;
                    } else {
                        if (nameCard.isEmpty()) {
                            nameCard = '[' % location % ']' % nickName;
                        } else {
                            nameCard = '[' % location % ']' % nameCard;
                        }
                    }
                }

                // 在这里，我们对新名片做规范化处理。
                d->formatNameCard(nameCard);

                if (!nameCard.isEmpty() && (nameCard != mi.nameCard().trimmed())) {
                    renameGroupMember(ev.from, li.uid, nameCard);
                }
            }
        }

        showSuccessList(ev.from, QString(u8"格式化后新的名片"), ll, false);
    } else {
        // 尝试修改昵称。
        CoolQ::MemberInfo mi = memberInfo(ev.from, ev.sender, false);
        if (mi.isValid()) {
            QString nameCard = mi.nameCard().remove(' ');
            d->safetyNameCard(nameCard);

            if (!correctNameCard.match(nameCard).hasMatch()) {
                QString nickName = mi.nickName().trimmed();
                QString location = mi.location().trimmed();
                d->safetyNameCard(nickName);
                if (location.isEmpty()) {
                    location = QString(u8"所在地");
                }

                if (correctLocation.match(nameCard).hasMatch()) {
                    nameCard = nameCard % nickName;
                } else if (correctNickName.match(nameCard).hasMatch()) {
                    nameCard = '[' % location % ']' % nameCard;
                } else {
                    if (nameCard.isEmpty()) {
                        nameCard = '[' % location % ']' % nickName;
                    } else {
                        nameCard = '[' % location % ']' % nameCard;
                    }
                }
            }

            // 在这里，我们对新名片做规范化处理。
            d->formatNameCard(nameCard);

            if (!nameCard.isEmpty() && (nameCard != mi.nameCard().trimmed())) {
                renameGroupMember(ev.from, ev.sender, nameCard);
            }

            showSuccess(ev.from, QString(u8"修改名片"), QString(u8"新的名片：%1").arg(nameCard));
        } else {
            showDanger(ev.from, QString(u8"修改名片"), QString(u8"数据同步错误，请重试。"));
        }
    }
}

void AssistantModule::groupBanAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    MasterLevel level = d->levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 五级管理及以上。
    if (level > MasterLevel::Master5) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    LevelInfoList ll = d->findUsers(args);
    if (ll.isEmpty()) {
        groupBanHelpAction(ev.from);
        return;
    }

    // 分析其他命令行参数

    int ds = 0;
    int hs = 0;
    int ms = 0;

    bool dsReady = false;
    bool hsReady = false;
    bool msReady = false;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);

        if (argv.endsWith('d')) {
            if (dsReady) {
                invalidArg = true;
                break;
            }
            dsReady = true;

            ds = argv.left(argv.length() - 1).toInt();
            if ((ds <= 0) || (ds > 30)) {
                invalidArg = true;
                break;
            }
        } else if (argv.endsWith('h')) {
            if (hsReady) {
                invalidArg = true;
                break;
            }
            hsReady = true;

            hs = argv.left(argv.length() - 1).toInt();
            if ((hs <= 0) || (hs > 24)) {
                invalidArg = true;
                break;
            }
        } else if (argv.endsWith('m')) {
            if (msReady) {
                invalidArg = true;
                break;
            }
            msReady = true;

            ms = argv.left(argv.length() - 1).toInt();
            if ((ms <= 0) || (ms > 60)) {
                invalidArg = true;
                break;
            }
        } else {
            invalidArg = true;
            break;
        }
    }
    if (invalidArg) {
        groupBanHelpAction(ev.from);
        return;
    }

    int duration = ds * 86400 + hs * 3600 + ms * 60;
    if (duration > 86400 * 30) {
        duration = 86400 * 30;
    } else if (duration <= 60) {
        duration = 60;
    }

    // 管理等级检查

    // 检查一：管理只能处理比自己等级低的成员。
    LevelInfoList ml;
    d->levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            ml << li;
        }
    }
    if (!ml.isEmpty()) {
        showDangerList(ev.from, QString(u8"你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    for (const LevelInfo &li : ll) {
        banGroupMember(ev.from, li.uid, duration);
    }

    showSuccessList(ev.from, QString(u8"下列成员已禁言"), ll, false);
}

void AssistantModule::groupKickAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    MasterLevel level = d->levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 三级管理及以上。
    if (level > MasterLevel::Master3) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    LevelInfoList ll = d->findUsers(args);
    if (ll.isEmpty()) {
        groupKickHelpAction(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != ll.count()) {
        groupKickHelpAction(ev.from);
        return;
    }

    // 管理等级检查

    // 检查一：管理只能处理比自己等级低的成员。
    LevelInfoList ml;
    d->levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            ml << li;
        }
    }
    if (!ml.isEmpty()) {
        showDangerList(ev.from, QString(u8"你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    for (const LevelInfo &li : ll) {
        kickGroupMember(ev.from, li.uid, false);
    }

    showSuccessList(ev.from, QString(u8"下列成员已踢出"), ll, true);
}

void AssistantModule::groupRaiseAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    MasterLevel level = d->levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 首席管理及以上。
    if (level > MasterLevel::Master1) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    LevelInfoList ll = d->findUsers(args);
    if (ll.isEmpty()) {
        groupRaiseHelpAction(ev.from);
        return;
    }

    // 分析其他命令行参数

    int argvLevel = 0;

    bool levelReady = false;
    bool argvGlobal = false;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);
        if (argv.startsWith('m')) {
            if (levelReady) {
                invalidArg = true;
                break;
            }
            levelReady = true;

            argvLevel = argv.mid(1).toInt();
            if ((argvLevel < 1) || (argvLevel > 5)) {
                invalidArg = true;
                break;
            }
        } else if ((argv == "g") || (argv == "global")) {
            if (argvGlobal) {
                invalidArg = true;
                break;
            }
            argvGlobal = true;
        } else {
            invalidArg = true;
            break;
        }
    }
    if (invalidArg) {
        groupRaiseHelpAction(ev.from);
        return;
    }

    // 转换到等级的枚举类型。
    MasterLevel newLevel = MasterLevel::Unknown;
    switch (argvLevel) {
    case 1:
        newLevel = MasterLevel::Master1;
        break;
    case 2:
        newLevel = MasterLevel::Master2;
        break;
    case 3:
        newLevel = MasterLevel::Master3;
        break;
    case 4:
        newLevel = MasterLevel::Master4;
        break;
    case 5:
        newLevel = MasterLevel::Master5;
        break;
    }
    if (MasterLevel::Unknown == newLevel) {
        groupRaiseHelpAction(ev.from);
        return;
    }

    // 管理等级检查

    // 检查一：只有 ATField 有跨群操作的权限。
    if (argvGlobal && (level != MasterLevel::ATField)) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }
    // 检查二：新等级不能超过当前管理的等级。
    if (newLevel <= level) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // 检查三：管理只能处理比自己等级低的成员。
    LevelInfoList ml;
    d->levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            ml << li;
        }
    }
    if (!ml.isEmpty()) {
        showDangerList(ev.from, QString(u8"你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    for (LevelInfo &li : ll) {
        li.level = newLevel;
        if (argvGlobal) {
            d->levels->setLevel(0, li.uid, li.level);
        } else {
            d->levels->setLevel(ev.from, li.uid, li.level);
        }
    }

    showSuccessList(ev.from, QString(u8"下列成员已提权"), ll, true);
}

void AssistantModule::groupUnbanAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    MasterLevel level = d->levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 五级管理及以上。
    if (level > MasterLevel::Master5) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    LevelInfoList ll = d->findUsers(args);
    if (ll.isEmpty()) {
        groupUnbanHelpAction(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != ll.count()) {
        groupUnbanHelpAction(ev.from);
        return;
    }

    // 管理等级检查

    // 检查一：管理只能处理比自己等级低的成员。
    LevelInfoList ml;
    d->levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            ml << li;
        }
    }
    if (!ml.isEmpty()) {
        showDangerList(ev.from, QString(u8"你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    for (const LevelInfo &li : ll) {
        banGroupMember(ev.from, li.uid, 0);
    }

    showSuccessList(ev.from, QString(u8"下列成员已解禁"), ll, false);
}

void AssistantModule::groupLowerAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    MasterLevel level = d->levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 首席管理及以上。
    if (level > MasterLevel::Master1) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    LevelInfoList ll = d->findUsers(args);
    if (ll.isEmpty()) {
        groupLowerHelpAction(ev.from);
        return;
    }

    // 分析其他命令行参数。
    bool argvGlobal = false;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);

        if ((argv == "g") || (argv == "global")) {
            if (argvGlobal) {
                invalidArg = true;
                break;
            }
            argvGlobal = true;
        } else {
            invalidArg = true;
            break;
        }
    }
    if (invalidArg) {
        groupLowerHelpAction(ev.from);
        return;
    }

    // 管理等级检查

    // 检查一：只有 ATField 有跨群操作的权限。
    if (argvGlobal && (MasterLevel::ATField != level)) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // 检查二：管理只能处理比自己等级低的成员。
    LevelInfoList ml;
    d->levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            ml << li;
        }
    }
    if (!ml.isEmpty()) {
        showDangerList(ev.from, QString(u8"你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    for (LevelInfo &li : ll) {
        li.level = MasterLevel::Unknown;
        if (argvGlobal) {
            d->levels->setLevel(0, li.uid, li.level);
        } else {
            d->levels->setLevel(ev.from, li.uid, li.level);
        }
    }

    showSuccessList(ev.from, QString(u8"下列成员已降权"), ll, true);
}

void AssistantModule::groupWatchlistAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    MasterLevel level = d->levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 首席管理及以上。
    if (level >= MasterLevel::Master1) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    LevelInfoList ll = d->findUsers(args);

    // 分析其他命令行参数。
    int argvOption = 0;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);

        if ((argv == "+") || (argv == QString(u8"加入")) || (argv == QString(u8"添加"))) {
            if (argvOption == 0) {
                argvOption = 1;
            } else {
                invalidArg = true;
                break;
            }
        } else {
            invalidArg = true;
            break;
        }
    }
    if (invalidArg) {
        groupWatchlistHelpAction(ev.from);
        return;
    } else if (!ll.isEmpty() && (0 == argvOption)) {
        groupWatchlistHelpAction(ev.from);
        return;
    }

    // 打印监控表。
    if (0 == argvOption) { // watchlist
        QHashIterator<CoolQ::Member, qint64> i(d->watchlist->members());
        while (i.hasNext()) {
            i.next();
            if (i.key().first == ev.from) {
                ll.append(LevelInfo(i.key().second, MasterLevel::Unknown));
            }
        }
        if (ll.isEmpty()) {
            showPrompt(ev.from, QString(u8"监控表"), QString(u8"监控表中没有任何成员"));
        } else {
            d->levels->update(ev.from, ll);
            showPromptList(ev.from, QString(u8"监控表"), ll, true);
        }
        return;
    }

    // 管理等级检查

    // 检查一：管理只能处理比自己等级低的成员。
    LevelInfoList ml;
    d->levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            ml << li;
        }
    }
    if (!ml.isEmpty()) {
        showDangerList(ev.from, QString(u8"你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    if (1 == argvOption) {
        for (const LevelInfo &li : ll) {
            d->watchlist->addMember(ev.from, li.uid);
        }

        showSuccessList(ev.from, QString(u8"下列成员已加入监控表"), ll, true);
    }
}

void AssistantModule::groupBlacklistAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    MasterLevel level = d->levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 首席管理及以上。
    if (level >= MasterLevel::Master1) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    LevelInfoList ll = d->findUsers(args);

    // 分析其他命令行参数。
    int argvOption = 0;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);

        if ((argv == "-") || (argv == QString(u8"移出")) || (argv == QString(u8"移除"))) {
            if (argvOption == 0) {
                argvOption = 1;
            } else {
                invalidArg = true;
                break;
            }
        } else {
            invalidArg = true;
            break;
        }
    }
    if (invalidArg) {
        groupBlacklistHelpAction(ev.from);
        return;
    } else if (!ll.isEmpty() && (0 == argvOption)) {
        groupBlacklistHelpAction(ev.from);
        return;
    }

    // 打印黑名单列表。
    if (0 == argvOption) {
        QHashIterator<CoolQ::Member, qint64> i(d->blacklist->members());
        while (i.hasNext()) {
            i.next();
            if (i.key().first == ev.from) {
                ll.append(LevelInfo(i.key().second, MasterLevel::Unknown));
            }
        }
        if (ll.isEmpty()) {
            showPrompt(ev.from, QString(u8"黑名单"), QString(u8"黑名单中没有任何成员"));
        } else {
            d->levels->update(ev.from, ll);
            showPromptList(ev.from, QString(u8"黑名单"), ll, true);
        }
        return;
    }

    // 管理等级检查

    // 检查一：管理只能处理比自己等级低的成员。
    LevelInfoList ml;
    d->levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            ml << li;
        }
    }
    if (!ml.isEmpty()) {
        showDangerList(ev.from, QString(u8"你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    if (1 == argvOption) {
        for (const LevelInfo &li : ll) {
            d->blacklist->removeMember(ev.from, li.uid);
        }

        showSuccessList(ev.from, QString(u8"下列成员已移出黑名单"), ll, true);
    }
}

void AssistantModule::groupMemberAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    MasterLevel level = d->levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 绝对领域及以上。
    if (level > MasterLevel::ATField) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    LevelInfoList ll = d->findUsers(args);
    if (ll.isEmpty()) {
        groupMemberHelpAction(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != ll.count()) {
        groupMemberHelpAction(ev.from);
        return;
    }

    // 执行具体操作

    for (const LevelInfo &li : ll) {
        CoolQ::MemberInfo mi = memberInfo(ev.from, li.uid, false);

        QString reports;
        QTextStream ts(&reports);

        ts << "<pre>";
        ts << "<code>qint64 gid: " << mi.gid() << "</code>\n";
        ts << "<code>qint64 uid: " << mi.uid() << "</code>\n";
        ts << "<code>qint32 sex: " << mi.sex() << "</code>\n";
        ts << "<code>qint32 age: " << mi.age() << "</code>\n";
        ts << "<code>QString nickName: </code>" << mi.nickName() << "\n";
        ts << "<code>QString nameCard: </code>" << mi.nameCard() << "\n";
        ts << "<code>QString location: </code>" << mi.location() << "\n";
        ts << "<code>QString levelName: </code>" << mi.levelName() << "\n";
        ts << "<code>qint32 permission: " << mi.permission() << "</code>\n";
        ts << "<code>qint32 unfriendly: " << mi.unfriendly() << "</code>\n";
        ts << "<code>QDateTime joinTime:\n  " << mi.joinTime().toString(Qt::ISODate) << "</code>\n";
        ts << "<code>QDateTime lastSent:\n  " << mi.lastSent().toString(Qt::ISODate) << "</code>\n";
        ts << "</pre>";

        ts.flush();

        showPrimary(ev.from, QString::number(li.uid), reports);
    }
}

void AssistantModule::groupRenameHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>重命名 [@成员] 新的名片</code>\n");
    ts << QString(u8"权限要求：") << "5+\n";
    ts << "<code>  </code>" << QString(u8"如果不@其他成员，则重命名自己的名片。") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"重命名命令"), usage);
}

void AssistantModule::groupLevelHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>等级 [参数] [@成员]</code>\n");
    ts << QString(u8"权限要求：") << "5+\n";
    ts << "<code>  </code>" << QString(u8"如果不@其他成员，则查询自己的等级。") << "\n";
    ts << QString(u8"参数列表：") << "\n";
    ts << QString(u8"<code>  全局(g) </code>") << QString(u8"全局查询") << "\n";
    ts << QString(u8"<code>  名单(l) </code>") << QString(u8"名单查询") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"等级查询"), usage);
}

void AssistantModule::groupFormatHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>格式化 [@成员]</code>\n");
    ts << QString(u8"权限要求：") << "5+\n";
    ts << "<code>  </code>" << QString(u8"如果不@其他成员，则格式化自己的名片。") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"格式化命令"), usage);
}

void AssistantModule::groupBanHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>禁言 @成员 @...</code>") << "\n";
    ts << QString(u8"权限要求：") << "5+\n";
    ts << QString(u8"参数列表：") << "\n";
    ts << QString(u8"<code>  [1-30]d</code>：") << QString(u8"天数") << "\n";
    ts << QString(u8"<code>  [1-24]h</code>：") << QString(u8"小时") << "\n";
    ts << QString(u8"<code>  [1-60]m</code>：") << QString(u8"分钟") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"禁言命令"), usage);
}

void AssistantModule::groupKickHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>踢出 @成员 @...</code>") << "\n";
    ts << QString(u8"权限要求：") << "3+\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"踢出命令"), usage);
}

void AssistantModule::groupRaiseHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>提权 @成员  @...</code>") << "\n";
    ts << QString(u8"权限要求：") << "1+\n";
    ts << QString(u8"参数列表：") << "\n";
    ts << QString(u8"<code>  m[1-5]</code>：") << QString(u8"等级") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"提权命令"), usage);
}

void AssistantModule::groupUnbanHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>解禁 @成员 @...</code>") << "\n";
    ts << QString(u8"权限要求：") << "5+\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"解禁命令"), usage);
}

void AssistantModule::groupLowerHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>降权 @成员 @...</code>") << "\n";
    ts << QString(u8"权限要求：") << "1+\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"降权命令"), usage);
}

void AssistantModule::groupWatchlistHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>监控表 [参数] [@成员 @...]</code>") << "\n";
    ts << QString(u8"权限要求：") << "1+\n";
    ts << QString(u8"参数列表：") << "\n";
    ts << QString(u8"<code>  加入(+)</code>：") << QString(u8"加入监控") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"监控表用法"), usage);
}

void AssistantModule::groupBlacklistHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>黑名单 [参数] [QQ号码 ...]</code>") << "\n";
    ts << QString(u8"权限要求：") << "1+\n";
    ts << QString(u8"参数列表：") << "\n";
    ts << QString(u8"<code>  移出(-)</code>：") << QString(u8"移出名单") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"黑名单命令"), usage);
}


void AssistantModule::groupMemberHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>成员信息 @成员</code>") << "\n";
    ts << QString(u8"权限要求：") << "5+\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"成员信息命令"), usage);
}

