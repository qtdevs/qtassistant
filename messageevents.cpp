#include "qtassistant.h"
#include "qtassistant_p.h"

#include <QStringBuilder>
#include <QTextStream>
#include <QtDebug>

#include "sqldatas/masterlevels.h"
#include "sqldatas/memberwelcome.h"
#include "sqldatas/memberblacklist.h"
#include "sqldatas/memberdeathhouse.h"

// class QtAssistant

bool QtAssistant::privateMessageEventFilter(const MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool QtAssistant::groupMessageEventFilter(const MessageEvent &ev)
{
    Q_D(QtAssistant);

    // 黑名单检查，如果发现匹配，直接踢出。
    if (d->blacklist->contains(ev.from, ev.sender)) {
        kickGroupMember(ev.from, ev.sender, false);
        qDebug("kickGroupMember(ev.from, ev.sender, false)");
        return true;
    }

    // 移出新手监控，不再进行监视。
    d->welcome->removeMember(ev.from, ev.sender);

    // 分离参数。
    QStringList args;
    if ((ev.gbkMsg[0] == '!')) {
        QString msgs = convert(ev.gbkMsg).mid(1);
        args = msgs.split(' ', QString::SkipEmptyParts);
    } else if (strncmp(ev.gbkMsg, "sudo", 4) == 0) {
        QString msgs = convert(ev.gbkMsg).mid(4);
        args = msgs.split(' ', QString::SkipEmptyParts);
    }

    // 命令派发。
    if (!args.isEmpty()) {
        QString c = args.value(0);
        if ((c == "h") || (c == "help")) {
            groupHelp(ev, args.mid(1));
            return true;
        }
        if ((c == "l") || (c == "level")) {
            groupLevel(ev, args.mid(1));
            return true;
        }
        if ((c == "r") || (c == "rename")) {
            groupRename(ev, args.mid(1));
            return true;
        }

        if ((c == "b") || (c == "ban")) {
            groupBan(ev, args.mid(1));
            return true;
        }
        if ((c == "k") || (c == "kill")) {
            groupKill(ev, args.mid(1));
            return true;
        }
        if ((c == "p") || (c == "power")) {
            groupPower(ev, args.mid(1));
            return true;
        }

        if ((c == "ub") || (c == "unban")) {
            groupUnban(ev, args.mid(1));
            return true;
        }
        if ((c == "uk") || (c == "unkill")) {
            groupUnkill(ev, args.mid(1));
            return true;
        }
        if ((c == "up") || (c == "unpower")) {
            groupUnpower(ev, args.mid(1));
            return true;
        }

        if ((c == "wl") || (c == "welcome")) {
            groupWelcome(ev, args.mid(1));
            return true;
        }
        if ((c == "bl") || (c == "blacklist")) {
            groupBlacklist(ev, args.mid(1));
            return true;
        }
    }

    return false;
}

bool QtAssistant::discussMessageEventFilter(const MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

void QtAssistant::groupHelp(const MessageEvent &ev, const QStringList &args)
{
    Q_D(QtAssistant);

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

    QString argv = args.value(0);
    if (!argv.isEmpty()) {
        if (argv == "h" || argv == "help") {
            groupHelpHelp(ev.from);
            return;
        } else if (argv == "l" || argv == "level") {
            groupLevelHelp(ev.from);
            return;
        } else if (argv == "r" || argv == "rename") {
            groupRenameHelp(ev.from);
            return;
        } else if (argv == "b" || argv == "ban") {
            groupBanHelp(ev.from);
            return;
        } else if (argv == "k" || argv == "kill") {
            groupKillHelp(ev.from);
            return;
        } else if (argv == "p" || argv == "power") {
            groupPowerHelp(ev.from);
            return;
        } else if (argv == "ub" || argv == "unban") {
            groupUnbanHelp(ev.from);
            return;
        } else if (argv == "uk" || argv == "unkill") {
            groupUnkillHelp(ev.from);
            return;
        } else if (argv == "up" || argv == "unpower") {
            groupUnpowerHelp(ev.from);
            return;
        }
    }

    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>  </code>" << tr("帮助信息") << " <code>(5+): help(h)</code>\n";
    ts << "<code>  </code>" << tr("等级查询") << " <code>( *): level(l)</code>\n";
    ts << "<code>  </code>" << tr("修改名片") << " <code>(5+): rename(r)</code>\n";
    ts << "<code>  </code>" << tr("禁言命令") << " <code>(5+): ban(b)</code>\n";
    ts << "<code>  </code>" << tr("取消禁言") << " <code>(5+): unban(ub)</code>\n";
    ts << "<code>  </code>" << tr("踢出命令") << " <code>(3+): kill(k)</code>\n";
    ts << "<code>  </code>" << tr("取消踢出") << " <code>(3+): unkill(uk)</code>\n";
    ts << "<code>  </code>" << tr("提权命令") << " <code>(1+): power(p)</code>\n";
    ts << "<code>  </code>" << tr("取消提权") << " <code>(1+): unpower(up)</code>\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(ev.from, tr("全部命令清单"), usage);
}

void QtAssistant::groupLevel(const MessageEvent &ev, const QStringList &args)
{
    Q_D(QtAssistant);

    LevelInfoList ll = d->findUsers(args);
    MasterLevel level = d->levels->level(ev.from, ev.sender);

    // 分析其他命令行参数

    bool argvGlobal = false;
    bool argvList = false;

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
        } else if ((argv == "l") || (argv == "list")) {
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
        groupLevelHelp(ev.from);
        return;
    }

    // 执行具体操作

    if (ll.isEmpty()) {
        if (argvList) {
            if (MasterLevel::Unknown != level) {
                ll = d->levels->levels(argvGlobal ? 0 : ev.from);
                if (ll.isEmpty()) {
                    if (argvGlobal) {
                        showPrompt(ev.from, tr("跨群等级列表"), tr("等级列表中没有任何成员"));
                    } else {
                        showPrompt(ev.from, tr("本群等级列表"), tr("等级列表中没有任何成员"));
                    }
                } else {
                    if (argvGlobal) {
                        showPromptList(ev.from, tr("跨群等级列表"), ll, true);
                    } else {
                        showPromptList(ev.from, tr("本群等级列表"), ll, true);
                    }
                }
            }
        } else {
            MasterLevel level = d->levels->level(argvGlobal ? 0 : ev.from, ev.sender);
            ll.append(LevelInfo(ev.sender, level));
            if (argvGlobal) {
                showPromptList(ev.from, tr("跨群等级"), ll, true);
            } else {
                showPromptList(ev.from, tr("本群等级"), ll, true);
            }
        }
    } else if (!argvList) {
        if (MasterLevel::Unknown != level) {
            d->levels->update(argvGlobal ? 0 : ev.from, ll);
            if (argvGlobal) {
                showPromptList(ev.from, tr("跨群等级"), ll, true);
            } else {
                showPromptList(ev.from, tr("本群等级"), ll, true);
            }
        }
    } else {
        groupLevelHelp(ev.from);
    }
}

void QtAssistant::groupRename(const MessageEvent &ev, const QStringList &args)
{
    Q_D(QtAssistant);

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

    QString name = convert(ev.gbkMsg);

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
                int r = name.indexOf(arg);
                name = name.mid(r + arg.count());
            } else {
                invalidArgs = true;
                break;
            }
        } else if (ll.isEmpty()) {
            // 在还没有找到号码的情况下，我们认为这是新名片的开始。
            if (!prefixFound) {
                int r = name.indexOf(arg);
                name = name.mid(r);
            }
            prefixFound = true;
        }
    }
    if (invalidArgs) {
        groupRenameHelp(ev.from);
        return;
    }

    // 在这里，我们对新名片做规范化处理。
    name.remove(' '); // 消除空格，不允许有空格。
    name.replace("【", "["); // 替换全角方括号，用半角方括号替代。
    name.replace("】", "]"); // 替换全角方括号，用半角方括号替代。

    if (name.isEmpty()) {
        groupRenameHelp(ev.from);
        return;
    }

    // 执行具体操作

    if (ll.isEmpty()) {
        renameGroupMember(ev.from, ev.sender, name);
    } else {
        renameGroupMember(ev.from, ll.at(0).uid, name);
    }

    showSuccess(ev.from, tr("修改名片"), tr("新的名片：%1").arg(name));
}

void QtAssistant::groupBan(const MessageEvent &ev, const QStringList &args)
{
    Q_D(QtAssistant);

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
        groupBanHelp(ev.from);
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
        groupBanHelp(ev.from);
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
        showDangerList(ev.from, tr("你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    for (const LevelInfo &li : ll) {
        banGroupMember(ev.from, li.uid, duration);
    }

    showSuccessList(ev.from, tr("下列成员已经被禁言"), ll, false);
}

void QtAssistant::groupKill(const MessageEvent &ev, const QStringList &args)
{
    Q_D(QtAssistant);

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
        groupKillHelp(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != ll.count()) {
        groupKillHelp(ev.from);
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
        showDangerList(ev.from, tr("你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    for (const LevelInfo &li : ll) {
        d->deathHouse->addMember(ev.from, li.uid);
    }

    showSuccessList(ev.from, tr("下列成员已经被加入驱逐队列"), ll, true);
}

void QtAssistant::groupPower(const MessageEvent &ev, const QStringList &args)
{
    Q_D(QtAssistant);

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
        groupPowerHelp(ev.from);
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
        groupPowerHelp(ev.from);
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
        groupPowerHelp(ev.from);
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
        showDangerList(ev.from, tr("你没有权限执行此操作"), ml, true);
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

    showSuccessList(ev.from, tr("下列成员已经被赋权"), ll, true);
}

void QtAssistant::groupUnban(const MessageEvent &ev, const QStringList &args)
{
    Q_D(QtAssistant);

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
        groupUnbanHelp(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != ll.count()) {
        groupUnbanHelp(ev.from);
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
        showDangerList(ev.from, tr("你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    for (const LevelInfo &li : ll) {
        banGroupMember(ev.from, li.uid, 0);
    }

    showSuccessList(ev.from, tr("下列成员已经被解禁"), ll, false);
}

void QtAssistant::groupUnkill(const MessageEvent &ev, const QStringList &args)
{
    Q_D(QtAssistant);

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
        groupUnkillHelp(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != ll.count()) {
        groupUnkillHelp(ev.from);
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
        showDangerList(ev.from, tr("你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    for (const LevelInfo &li : ll) {
        d->deathHouse->removeMember(ev.from, li.uid);
    }

    showSuccessList(ev.from, tr("下列成员已经被移出驱逐队列"), ll, false);
}

void QtAssistant::groupUnpower(const MessageEvent &ev, const QStringList &args)
{
    Q_D(QtAssistant);

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
        groupUnpowerHelp(ev.from);
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
        groupUnpowerHelp(ev.from);
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
        showDangerList(ev.from, tr("你没有权限执行此操作"), ml, true);
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

    showSuccessList(ev.from, tr("下列成员已经被降权"), ll, true);
}

void QtAssistant::groupWelcome(const MessageEvent &ev, const QStringList &args)
{
    Q_D(QtAssistant);

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

    // 分析其他命令行参数。
    int argvOption = 0;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);

        if ((argv == "a") || (argv == "add")) {
            if (argvOption == 0) {
                argvOption = 1;
            } else {
                invalidArg = true;
                break;
            }
        } else if ((argv == "d") || (argv == "delete")) {
            if (argvOption == 0) {
                argvOption = 2;
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
        groupWelcomeHelp(ev.from);
        return;
    } else if (!ll.isEmpty() && (0 == argvOption)) {
        groupWelcomeHelp(ev.from);
        return;
    }

    // 打印新人监控。
    if (0 == argvOption) { // list
        QHashIterator<Member, qint64> i(d->welcome->members());
        while (i.hasNext()) {
            i.next();
            if (i.key().first == ev.from) {
                ll.append(LevelInfo(i.key().second, MasterLevel::Unknown));
            }
        }
        if (ll.isEmpty()) {
            showPrompt(ev.from, tr("新人监控"), tr("新人监控中没有任何成员"));
        } else {
            d->levels->update(ev.from, ll);
            showPromptList(ev.from, tr("新人监控"), ll, true);
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
        showDangerList(ev.from, tr("你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    if (1 == argvOption) { // add
        for (const LevelInfo &li : ll) {
            d->welcome->addMember(ev.from, li.uid);
        }

        showSuccessList(ev.from, tr("下列成员已经被加入新人监控"), ll, true);
    } else if (2 == argvOption) { // delete
        for (const LevelInfo &li : ll) {
            d->welcome->removeMember(ev.from, li.uid);
        }

        showSuccessList(ev.from, tr("下列成员已经被移出新人监控"), ll, true);
    }
}

void QtAssistant::groupBlacklist(const MessageEvent &ev, const QStringList &args)
{
    Q_D(QtAssistant);

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

    // 分析其他命令行参数。
    bool argvGlobal = false;
    int argvOption = 0;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);

        if ((argv == "a") || (argv == "add")) {
            if (argvOption == 0) {
                argvOption = 1;
            } else {
                invalidArg = true;
                break;
            }
        } else if ((argv == "d") || (argv == "delete")) {
            if (argvOption == 0) {
                argvOption = 2;
            } else {
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
        groupBlacklistHelp(ev.from);
        return;
    } else if (!ll.isEmpty() && (0 == argvOption)) {
        groupBlacklistHelp(ev.from);
        return;
    }

    // 打印黑名单列表。
    if (0 == argvOption) {
        qint64 gid = argvGlobal ? 0 : ev.from;
        QHashIterator<Member, qint64> i(d->blacklist->members());
        while (i.hasNext()) {
            i.next();
            if (i.key().first == gid) {
                ll.append(LevelInfo(i.key().second, MasterLevel::Unknown));
            }
        }
        if (ll.isEmpty()) {
            if (argvGlobal) {
                showPrompt(ev.from, tr("跨群黑名单"), tr("黑名单中没有任何成员"));
            } else {
                showPrompt(ev.from, tr("本群黑名单"), tr("黑名单中没有任何成员"));
            }
        } else {
            d->levels->update(ev.from, ll);
            if (argvGlobal) {
                showPromptList(ev.from, tr("跨群黑名单"), ll, true);
            } else {
                showPromptList(ev.from, tr("本群黑名单"), ll, true);
            }
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
        showDangerList(ev.from, tr("你没有权限执行此操作"), ml, true);
        return;
    }

    // 执行具体操作

    if (1 == argvOption) { // add
        for (const LevelInfo &li : ll) {
            d->blacklist->addMember(argvGlobal ? 0 : ev.from, li.uid);
        }

        showSuccessList(ev.from, tr("下列成员已经被加入黑名单"), ll, true);
    } else if (2 == argvOption) { // delete
        for (const LevelInfo &li : ll) {
            d->blacklist->removeMember(argvGlobal ? 0 : ev.from, li.uid);
        }

        showSuccessList(ev.from, tr("下列成员已经被移出黑名单"), ll, true);
    }
}

void QtAssistant::groupHelpHelp(qint64 gid)
{
    Q_UNUSED(gid);
}

void QtAssistant::groupLevelHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>sudo level(l) [" << tr("参数") << "] [" << tr("成员") << "]</code>\n";
    ts << tr("权限要求：") << "5+\n";
    ts << tr("参数列表：") << "\n";
    ts << "<code>  global(g) </code>" << tr("跨群查询") << "\n";
    ts << "<code>  list(l)   </code>" << tr("列表查询") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, tr("等级查询的用法"), usage);
}

void QtAssistant::groupRenameHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>sudo rename(r) [" << tr("成员") << "] " << tr("名片") << "</code>\n";
    ts << tr("权限要求：") << "5+\n";
    ts << "<code>  </code>" << tr("新的名片将会被格式化为统一样式，不允许出现空格。") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, "修改名片的用法", usage);
}

void QtAssistant::groupBanHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>sudo ban(b) [" << tr("参数") << "] " << tr("成员") << " ...</code>\n";
    ts << tr("权限要求：") << "5+\n";
    ts << tr("参数列表：") << "\n";
    ts << "<code>  [1-30]d </code>" << tr("禁言天数") << "\n";
    ts << "<code>  [1-24]h </code>" << tr("禁言时数") << "\n";
    ts << "<code>  [1-60]m </code>" << tr("禁言分数") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, tr("禁言命令的用法"), usage);
}

void QtAssistant::groupKillHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>sudo kill(k) " << tr("成员") << " ...</code>\n";
    ts << tr("权限要求：") << "3+\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, tr("踢出命令的用法"), usage);
}

void QtAssistant::groupPowerHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>sudo power(p) [" << tr("参数") << "] " << tr("成员") << " ...</code>\n";
    ts << tr("权限要求：") << "1+\n";
    ts << tr("参数列表：") << "\n";
    ts << "<code>  m[1-5] </code>" << tr("权限等级") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, tr("提权命令的用法"), usage);
}

void QtAssistant::groupUnbanHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>sudo unban(ub) " << tr("成员") << " ...</code>\n";
    ts << tr("权限要求：") << "5+\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, tr("取消禁言的用法"), usage);
}

void QtAssistant::groupUnkillHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>sudo unkill(uk) " << tr("成员") << " ...</code>\n";
    ts << tr("权限要求：") << "3+\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, tr("取消踢出的用法"), usage);
}

void QtAssistant::groupUnpowerHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>sudo unpower(up) " << tr("成员") << " ...</code>\n";
    ts << tr("权限要求：") << "1+\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, tr("取消提权的用法"), usage);
}

void QtAssistant::groupWelcomeHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>sudo welcome(wl) [" << tr("参数") << "] [" << tr("成员") << "] ...</code>\n";
    ts << tr("权限要求：") << "1+\n";
    ts << tr("参数列表：") << "\n";
    ts << "<code>  add(a)|delete(d) </code>" << tr("加入|移除新人监控") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, tr("新人监控的用法"), usage);
}

void QtAssistant::groupBlacklistHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << "<code>sudo blacklist(bl) [" << tr("参数") << "] [" << tr("成员") << "] ...</code>\n";
    ts << tr("权限要求：") << "1+\n";
    ts << tr("参数列表：") << "\n";
    ts << "<code>  add(a)|delete(d) </code>" << tr("加入|移除黑名单") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, tr("黑名单的用法"), usage);
}
