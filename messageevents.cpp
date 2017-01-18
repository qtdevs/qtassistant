#include "qtassistant.h"
#include "qtassistant_p.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QStringBuilder>
#include <QTextStream>
#include <QPixmap>

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

        if (c == "member") {
            CqMemberInfo mi = memberInfo(ev.from, ev.sender);
            sendGroupMessage(ev.from, QString("%1 : %2 : %3 : %4 : %5 : %6 : %7 : %8 : %9 : %10 : %11 : %12")
                             .arg(mi.gid()).arg(mi.uid()).arg(mi.nickName()).arg(mi.nameCard()).arg(mi.sex()).arg(mi.age()).arg(mi.location())
                             .arg(mi.joinTime().toString()).arg(mi.lastSent().toString()).arg(mi.levelName()).arg(mi.permission()).arg(mi.unfriendly()));

            CqPersonInfo pi = personInfo(ev.sender);
            sendGroupMessage(ev.from, QString("%1 : %2 : %3 : %4")
                             .arg(pi.uid()).arg(pi.nickName()).arg(pi.sex()).arg(pi.age()));

            d->permissionDenied(ev.from, ev.sender, MasterLevel::Unknown, "");
        }

        if (c == QLatin1String("h") || c == QLatin1String("help")) {
            d->groupHelp(ev, args.mid(1));
            return true;
        }
        if (c == QLatin1String("l") || c == QLatin1String("level")) {
            d->groupLevel(ev, args.mid(1));
            return true;
        }
        if (c == QLatin1String("r") || c == QLatin1String("rename")) {
            d->groupRename(ev, args.mid(1));
            return true;
        }

        if (c == QLatin1String("b") || c == QLatin1String("ban")) {
            d->groupBan(ev, args.mid(1));
            return true;
        }
        if (c == QLatin1String("k") || c == QLatin1String("kill")) {
            d->groupKill(ev, args.mid(1));
            return true;
        }
        if (c == QLatin1String("p") || c == QLatin1String("power")) {
            d->groupPower(ev, args.mid(1));
            return true;
        }

        if (c == QLatin1String("ub") || c == QLatin1String("unban")) {
            d->groupUnban(ev, args.mid(1));
            return true;
        }
        if (c == QLatin1String("uk") || c == QLatin1String("unkill")) {
            d->groupUnkill(ev, args.mid(1));
            return true;
        }
        if (c == QLatin1String("up") || c == QLatin1String("unpower")) {
            d->groupUnpower(ev, args.mid(1));
            return true;
        }

        if (c == QLatin1String("wl") || c == QLatin1String("welcome")) {
            d->groupWelcome(ev, args.mid(1));
            return true;
        }
        if (c == QLatin1String("bl") || c == QLatin1String("blacklist")) {
            d->groupBlacklist(ev, args.mid(1));
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

// class QtAssistantPrivate

void QtAssistantPrivate::groupHelp(const MessageEvent &ev, const QStringList &args)
{
    // 普通成员不应答。
    MasterLevel level = levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 五级管理及以上。
    if (level > MasterLevel::Master5) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    Q_UNUSED(args);

    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString::fromUtf8("  帮助信息(5+): <code>h,help</code>\n");
    ts << QString::fromUtf8("  查询等级(5+): <code>l,level</code>\n");
    ts << QString::fromUtf8("  修改昵称(5+): <code>r,rename</code>\n");
    ts << QString::fromUtf8("  禁言命令(5+): <code>b,ban</code>\n");
    ts << QString::fromUtf8("  取消禁言(5+): <code>ub,unban</code>\n");
    ts << QString::fromUtf8("  踢出命令(3+): <code>k,kill</code>\n");
    ts << QString::fromUtf8("  取消踢出(3+): <code>uk,unkill</code>\n");
    ts << QString::fromUtf8("  提权命令(1+): <code>p,power</code>\n");
    ts << QString::fromUtf8("  取消提权(1+): <code>up,unpower</code>\n");
    ts << "</pre>";

    ts.flush();

    showPrompt(ev.from, "全部命令清单", usage);
}

void QtAssistantPrivate::groupLevel(const MessageEvent &ev, const QStringList &args)
{
    Q_Q(QtAssistant);

    MasterLevel level = levels->level(ev.from, ev.sender);

    LevelInfoList ll = findUsers(args);

    // Parse Command Lines

    bool argvListRead = false;
    bool argvGlobalRead = false;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);

        if ((argv == QLatin1String("g"))
                || (argv == QLatin1String("global"))) {
            if (argvGlobalRead) {
                invalidArg = true;
                break;
            }
            argvGlobalRead = true;
        } else if ((argv == QLatin1String("l"))
                   || (argv == QLatin1String("list"))) {
            if (argvListRead) {
                invalidArg = true;
                break;
            }
            argvListRead = true;
        } else {
            invalidArg = true;
            break;
        }
    }
    if (invalidArg) {
        groupLevelHelp(ev.from);
        return;
    }

    if (ll.isEmpty()) {
        if (argvListRead) {
            if (MasterLevel::Unknown != level) {
                ll = levels->levels(argvGlobalRead ? 0 : ev.from);
                if (argvGlobalRead) {
                    showPromptList(ev.from, q->tr("Global Level List"), ll, true);
                } else {
                    showPromptList(ev.from, q->tr("Local Level List"), ll, true);
                }
            }
        } else {
            MasterLevel level = levels->level(argvGlobalRead ? 0 : ev.from, ev.sender);
            ll.append(LevelInfo(ev.sender, level));
            showPromptList(ev.from, argvGlobalRead ? q->tr("Global Level") : q->tr("Local Level"), ll, true);
        }
    } else if (!argvListRead) {
        if (MasterLevel::Unknown != level) {
            levels->update(argvGlobalRead ? 0 : ev.from, ll);
            showPromptList(ev.from, argvGlobalRead ? q->tr("Global Level List") : q->tr("Local Level List"), ll, true);
        }
    }
}

void QtAssistantPrivate::groupRename(const MessageEvent &ev, const QStringList &args)
{
    Q_Q(QtAssistant);

    // 普通成员不应答。
    MasterLevel level = levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 五级管理及以上。
    if (level > MasterLevel::Master5) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    // !!! 由于此命令的特殊性，命令行解析自行分析。

    QString name = q->convert(ev.gbkMsg);

    LevelInfoList ll;
    bool prefixFound = false;
    bool invalidArgs = false;
    for (const QString &arg : args) {
        if (arg.startsWith("[CQ:at")) {
            int i = arg.lastIndexOf("]");
            QString uid = arg.mid(10, i - 10);
            ll.append(LevelInfo(uid.toLongLong(),
                                MasterLevel::Unknown));

            // 如果找到了号码，同时找到了部分新名片，我们认为它是无效的命令。
            if (prefixFound) {
                invalidArgs = true;
                break;
            }

            // 如果只找到一个号码，我们认为号码以后的部分都是新名片；否则就是无效的命令。
            if (ll.count() == 1) {
                int r = name.indexOf(arg);
                name = name.mid(r + arg.count());
            } else {
                invalidArgs = true;
                break;
            }
        } else if (ll.isEmpty()) {
            // 在还没有找到号码的情况下，我们认为这是新名片的开始。
            prefixFound = true;
            int r = name.indexOf(arg);
            name = name.mid(r);
        }
    }
    if (invalidArgs) {
        groupRenameHelp(ev.from);
        return;
    }

    // 在这里，我们对新名片做规范化处理。

    name.remove(' '); // 消除空格，不允许有空格。
    name.replace('\u3010', '['); // 替换全角方括号，用半角方括号替代。
    name.replace('\u3011', ']'); // 替换全角方括号，用半角方括号替代。

    if (name.isEmpty()) {
        groupRenameHelp(ev.from);
        return;
    }

    // 执行重命名操作。

    if (ll.isEmpty()) {
        q->renameGroupMember(ev.from, ev.sender, name);
    } else {
        q->renameGroupMember(ev.from, ll.at(0).uid, name);
    }

    showSuccess(ev.from, q->tr("Rename"), q->tr("Nickname Changed: %1").arg(name));
}

void QtAssistantPrivate::groupBan(const MessageEvent &ev, const QStringList &args)
{
    Q_Q(QtAssistant);

    // 普通成员不应答。
    MasterLevel level = levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 五级管理及以上。
    if (level > MasterLevel::Master5) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    LevelInfoList ll = findUsers(args);
    if (ll.isEmpty()) {
        groupBanHelp(ev.from);
        return;
    }

    // Parse Command Lines

    int ds = 0;
    int hs = 0;
    int ms = 0;

    bool dsRead = false;
    bool hsRead = false;
    bool msRead = false;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);

        if (argv.endsWith('d')) {
            if (dsRead) {
                invalidArg = true;
                break;
            }
            dsRead = true;

            ds = argv.left(argv.length() - 1).toInt();
            if ((ds <= 0) || (ds > 30)) {
                invalidArg = true;
                break;
            }
        } else if (argv.endsWith('h')) {
            if (hsRead) {
                invalidArg = true;
                break;
            }
            hsRead = true;

            hs = argv.left(argv.length() - 1).toInt();
            if ((hs <= 0) || (hs > 24)) {
                invalidArg = true;
                break;
            }
        } else if (argv.endsWith('m')) {
            if (msRead) {
                invalidArg = true;
                break;
            }
            msRead = true;

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

    // Master Level Checks

    LevelInfoList masters;
    levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            masters << li;
        }
    }
    if (!masters.isEmpty()) {
        showDangerList(ev.from, q->tr("You have no rights to ban the following masters"), masters, true);
        return;
    }

    for (const LevelInfo &li : ll) {
        q->banGroupMember(ev.from, li.uid, duration);
    }
    showSuccessList(ev.from, q->tr("The following members have been banned"), ll, false);
}

void QtAssistantPrivate::groupKill(const MessageEvent &ev, const QStringList &args)
{
    Q_Q(QtAssistant);

    // 普通成员不应答。
    MasterLevel level = levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 三级管理及以上。
    if (level > MasterLevel::Master3) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    LevelInfoList ll = findUsers(args);
    if (ll.isEmpty()) {
        groupKillHelp(ev.from);
        return;
    }

    // Parse Command Lines

    if (args.count() != ll.count()) {
        groupKillHelp(ev.from);
        return;
    }

    // Master Level Checks

    LevelInfoList masters;
    levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= MasterLevel::Master5) {
            masters << li;
        }
    }
    if (!masters.isEmpty()) {
        showDangerList(ev.from, q->tr("You have no rights to kill the following masters"), masters, true);
        return;
    }

    for (const LevelInfo &li : ll) {
        deathHouse->addMember(ev.from, li.uid);
    }

    showSuccessList(ev.from, q->tr("The following members have been killed soon"), ll, true);
}

void QtAssistantPrivate::groupPower(const MessageEvent &ev, const QStringList &args)
{
    Q_Q(QtAssistant);

    // 普通成员不应答。
    MasterLevel level = levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 首席管理及以上。
    if (level > MasterLevel::Master1) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    LevelInfoList ll = findUsers(args);
    if (ll.isEmpty()) {
        groupPowerHelp(ev.from);
        return;
    }

    // Parse Command Lines

    int argvLevel = 0;
    int argvGlobal = 0;

    bool argvLevelRead = false;
    bool argvGlobalRead = false;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);

        if (argv.startsWith('m')) {
            if (argvLevelRead) {
                invalidArg = true;
                break;
            }
            argvLevelRead = true;

            argvLevel = argv.mid(1).toInt();
            if ((argvLevel < 1) || (argvLevel > 5)) {
                invalidArg = true;
                break;
            }
        } else if ((argv == QLatin1String("g"))
                   || (argv == QLatin1String("global"))) {
            if (argvGlobalRead) {
                invalidArg = true;
                break;
            }
            argvGlobalRead = true;

            argvGlobal = 1;
        } else {
            invalidArg = true;
            break;
        }
    }
    if (invalidArg) {
        groupPowerHelp(ev.from);
        return;
    }

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

    // Master Level Checks

    if (newLevel <= level) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    } else if (argvGlobal && (level != MasterLevel::ATField)) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    LevelInfoList masters;
    levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            masters << li;
        }
    }
    if (!masters.isEmpty()) {
        showDangerList(ev.from, q->tr("You have no rights to kill the following masters"), masters, true);
        return;
    }

    for (LevelInfo &li : ll) {
        levels->setLevel(argvGlobal ? 0 : ev.from, li.uid, newLevel);
        li.level = newLevel;
    }
    showSuccessList(ev.from, q->tr("The following members have been repowered"), ll, true);
}

void QtAssistantPrivate::groupUnban(const MessageEvent &ev, const QStringList &args)
{
    Q_Q(QtAssistant);

    // 普通成员不应答。
    MasterLevel level = levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 五级管理及以上。
    if (level > MasterLevel::Master5) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    LevelInfoList ll = findUsers(args);
    if (ll.isEmpty()) {
        groupUnbanHelp(ev.from);
        return;
    }

    // Parse Command Lines

    if (args.count() != ll.count()) {
        groupUnbanHelp(ev.from);
        return;
    }

    // Master Level Checks

    LevelInfoList masters;
    levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            masters << li;
        }
    }
    if (!masters.isEmpty()) {
        showDangerList(ev.from, q->tr("You have no rights to unban the following masters"), masters, true);
        return;
    }

    for (const LevelInfo &li : ll) {
        q->banGroupMember(ev.from, li.uid, 0);
    }
    showSuccessList(ev.from, q->tr("The following members have been unbanned"), ll, false);
}

void QtAssistantPrivate::groupUnkill(const MessageEvent &ev, const QStringList &args)
{
    Q_Q(QtAssistant);

    // 普通成员不应答。
    MasterLevel level = levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 三级管理及以上。
    if (level > MasterLevel::Master3) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    LevelInfoList ll = findUsers(args);
    if (ll.isEmpty()) {
        groupUnkillHelp(ev.from);
        return;
    }

    // Parse Command Lines

    if (args.count() != ll.count()) {
        groupUnkillHelp(ev.from);
        return;
    }

    // Master Level Checks

    LevelInfoList masters;
    levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            masters << li;
        }
    }
    if (!masters.isEmpty()) {
        showDangerList(ev.from, q->tr("You have no rights to unkill the following masters"), masters, true);
        return;
    }

    for (const LevelInfo &li : ll) {
        deathHouse->removeMember(ev.from, li.uid);
    }
    showSuccessList(ev.from, q->tr("The following members have been unkilled"), ll, false);
}

void QtAssistantPrivate::groupUnpower(const MessageEvent &ev, const QStringList &args)
{
    Q_Q(QtAssistant);

    // 普通成员不应答。
    MasterLevel level = levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 首席管理及以上。
    if (level > MasterLevel::Master1) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    LevelInfoList ll = findUsers(args);
    if (ll.isEmpty()) {
        groupUnpowerHelp(ev.from);
        return;
    }

    // Parse Command Lines

    int argvGlobal = 0;

    bool argvGlobalRead = false;

    bool invalidArg = false;
    int c = args.count() - ll.count();
    for (int i = 0; i < c; ++i) {
        const auto &argv = args.at(i);

        if ((argv == QLatin1String("g"))
                || (argv == QLatin1String("global"))) {
            if (argvGlobalRead) {
                invalidArg = true;
                break;
            }
            argvGlobalRead = true;

            argvGlobal = 1;
        } else {
            invalidArg = true;
            break;
        }
    }
    if (invalidArg) {
        groupUnpowerHelp(ev.from);
        return;
    }

    // Master Level Checks

    if (argvGlobal && (level != MasterLevel::ATField)) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    LevelInfoList masters;
    levels->update(ev.from, ll);
    for (const LevelInfo &li : ll) {
        if (li.level <= level) {
            masters << li;
        }
    }
    if (!masters.isEmpty()) {
        showDangerList(ev.from, q->tr("You have no rights to unpower the following masters"), masters, true);
        return;
    }

    for (const LevelInfo &li : ll) {
        levels->setLevel(argvGlobal ? 0 : ev.from, li.uid, MasterLevel::Unknown);
    }
    showSuccessList(ev.from, q->tr("The following members have been unpowered"), ll, true);
}

void QtAssistantPrivate::groupWelcome(const MessageEvent &ev, const QStringList &args)
{
    Q_Q(QtAssistant);

    // 普通成员不应答。
    MasterLevel level = levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 首席管理及以上。
    if (level > MasterLevel::Master1) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    int argvOption = 0;

    LevelInfoList ll;
    bool invalidArgs = false;
    for (const QString &arg : args) {
        if (arg.startsWith("[CQ:at")) {
            int i = arg.lastIndexOf("]");
            QString str = arg.mid(10, i - 10);
            ll.append(LevelInfo(str.toLongLong(),
                                MasterLevel::Unknown));
        } else if ((arg == QLatin1String("a"))
                   || (arg == QLatin1String("add"))) {
            if (argvOption == 0) {
                argvOption = 1;
            } else {
                invalidArgs = true;
                break;
            }
        } else if ((arg == QLatin1String("d"))
                   || (arg == QLatin1String("delete"))) {
            if (argvOption == 0) {
                argvOption = 2;
            } else {
                invalidArgs = true;
                break;
            }
        } else if ((arg == QLatin1String("l"))
                   || (arg == QLatin1String("list"))) {
            if (argvOption == 0) {
                argvOption = 3;
            } else {
                invalidArgs = true;
                break;
            }
        } else {
            invalidArgs = true;
            break;
        }
    }

    if (!invalidArgs && (argvOption != 0)) {
        QStringList masters;
        QStringList members;

        if (argvOption == 3) {
            if (ll.isEmpty()) {
                QDateTime now = QDateTime::currentDateTime();
                QHashIterator<Member, qint64> i(welcome->members());
                while (i.hasNext()) {
                    i.next();
                    QDateTime stamp = QDateTime::fromMSecsSinceEpoch(i.value()).addSecs(1800);
                    members << q->tr("%1 will kicked in %2 minute(s).").arg(q->cqAt(i.key().second)).arg(now.secsTo(stamp) / 60);
                }
                members.prepend(q->tr("Welcome List:"));
                q->sendGroupMessage(ev.from, members.join("\n"));

                return;
            }
        } else {
            if (!ll.isEmpty()) {
                levels->update(ev.from, ll);
                for (const LevelInfo &li : ll) {
                    if (li.level <= level) {
                        masters << q->cqAt(li.uid);
                    } else {
                        if (argvOption == 1) {
                            welcome->addMember(ev.from, li.uid);
                            q->sendGroupMessage(ev.from, q->tr("%1, Welcome to join us, please say something in 30 minutes.").arg(q->cqAt(li.uid)));
                        } else if (argvOption == 2) {
                            welcome->removeMember(ev.from, li.uid);
                        }
                        members << q->cqAt(li.uid);
                    }
                }

                QStringList reply;
                if (!masters.isEmpty()) {
                    reply << q->tr("Permission Denied:");
                    reply << masters;
                }
                if (!members.isEmpty()) {
                    if (argvOption == 1) {
                        reply << q->tr("Welcome Add List:");
                    } else if (argvOption == 2) {
                        reply << q->tr("Welcome Delete List:");
                    }
                    reply << members;
                }
                q->sendGroupMessage(ev.from, reply.join("\n"));

                return;
            }
        }
    }

    q->sendGroupMessage(ev.from, q->tr("welcome [add|delete] @Member1 [@Member2] [@Member3] ..."));
}

void QtAssistantPrivate::groupBlacklist(const MessageEvent &ev, const QStringList &args)
{
    Q_Q(QtAssistant);

    // 普通成员不应答。
    MasterLevel level = levels->level(ev.from, ev.sender);
    if (MasterLevel::Unknown == level) {
        return;
    }
    // 首席管理及以上。
    if (level > MasterLevel::Master1) {
        permissionDenied(ev.from, ev.sender, level);
        return;
    }

    int argvOption = 0;
    bool argvGlobal = false;

    LevelInfoList ll;
    bool invalidArgs = false;
    for (const QString &arg : args) {
        if (arg.startsWith("[CQ:at")) {
            int i = arg.lastIndexOf("]");
            QString str = arg.mid(10, i - 10);
            ll.append(LevelInfo(str.toLongLong(),
                                MasterLevel::Unknown));
        } else if ((arg == QLatin1String("a"))
                   || (arg == QLatin1String("add"))) {
            if (argvOption == 0) {
                argvOption = 1;
            } else {
                invalidArgs = true;
                break;
            }
        } else if ((arg == QLatin1String("d"))
                   || (arg == QLatin1String("delete"))) {
            if (argvOption == 0) {
                argvOption = 2;
            } else {
                invalidArgs = true;
                break;
            }
        } else if ((arg == QLatin1String("g"))
                   || (arg == QLatin1String("global"))) {
            argvGlobal = true;
        } else if ((arg == QLatin1String("l"))
                   || (arg == QLatin1String("list"))) {
            if (argvOption == 0) {
                argvOption = 3;
            } else {
                invalidArgs = true;
                break;
            }
        } else {
            invalidArgs = true;
            break;
        }
    }

    QStringList masters;
    QStringList members;

    if (!invalidArgs && (argvOption != 0)) {
        if (argvOption == 3) {
            if (ll.isEmpty()) {
                QHashIterator<Member, qint64> i(blacklist->blacklist());
                while (i.hasNext()) {
                    i.next();
                    members << q->cqAt(i.key().second);
                }
                members.prepend(q->tr("Blacklist List:"));
                q->sendGroupMessage(ev.from, members.join("\n"));

                return;
            }
        } else {
            if (!ll.isEmpty()) {
                levels->update(ev.from, ll);
                for (const LevelInfo &li : ll) {
                    if (li.level <= level) {
                        masters << q->cqAt(li.uid);
                    } else {
                        if (argvOption == 1) {
                            blacklist->addMember(ev.from, li.uid);
                        } else if (argvOption == 2) {
                            blacklist->removeMember(ev.from, li.uid);
                        }
                        members << q->cqAt(li.uid);
                    }
                }

                QStringList reply;
                if (!masters.isEmpty()) {
                    reply << q->tr("Permission Denied:");
                    reply << masters;
                }
                if (!members.isEmpty()) {
                    if (argvOption == 1) {
                        reply << q->tr("Blacklist Add List:");
                    } else if (argvOption == 2) {
                        reply << q->tr("Blacklist Delete List:");
                    }
                    reply << members;
                }
                q->sendGroupMessage(ev.from, reply.join("\n"));

                return;
            }
        }
    }

    q->sendGroupMessage(ev.from, q->tr("blacklist [add|delete] @Member1 [@Member2] [@Member3] ..."));
}

void QtAssistantPrivate::groupHelpHelp(qint64 gid)
{
    Q_UNUSED(gid);
}

void QtAssistantPrivate::groupLevelHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString::fromUtf8("<code>sudo level [成员]</code>\n");
    ts << QString::fromUtf8("权限要求: 五级管理以上\n");
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, "等级查询操作的用法", usage);
}

void QtAssistantPrivate::groupRenameHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString::fromUtf8("<code>sudo rename [成员] [新名片]</code>\n");
    ts << QString::fromUtf8("权限要求: 五级管理以上\n");
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, "改名操作的用法", usage);
}

void QtAssistantPrivate::groupBanHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString::fromUtf8("<code>sudo ban [参数] [成员]...</code>\n");
    ts << QString::fromUtf8("权限要求: 五级管理以上\n");
    ts << QString::fromUtf8("参数列表:\n");
    ts << QString::fromUtf8("<code>  [1-30]d </code>禁言天数\n");
    ts << QString::fromUtf8("<code>  [1-24]h </code>禁言时数\n");
    ts << QString::fromUtf8("<code>  [1-60]m </code>禁言分数\n");
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, "禁言操作的用法", usage);
}

void QtAssistantPrivate::groupKillHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString::fromUtf8("<code>sudo kill [成员]...</code>\n");
    ts << QString::fromUtf8("权限要求: 三级管理以上\n");
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, "踢出操作的用法", usage);
}

void QtAssistantPrivate::groupPowerHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString::fromUtf8("<code>sudo power [参数] [成员]...</code>\n");
    ts << QString::fromUtf8("权限要求: 首席管理以上\n");
    ts << QString::fromUtf8("参数列表:\n");
    ts << QString::fromUtf8("<code>  m[1-5] </code>权限等级\n");
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, "赋权操作的用法", usage);
}

void QtAssistantPrivate::groupUnbanHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString::fromUtf8("<code>sudo unban [成员]...</code>\n");
    ts << QString::fromUtf8("权限要求: 五级管理以上\n");
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, "解禁操作的用法", usage);
}

void QtAssistantPrivate::groupUnkillHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString::fromUtf8("<code>sudo unkill [成员]...</code>\n");
    ts << QString::fromUtf8("权限要求: 三级管理以上\n");
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, "取消踢出的用法", usage);
}

void QtAssistantPrivate::groupUnpowerHelp(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString::fromUtf8("<code>sudo unpower [成员]...</code>\n");
    ts << QString::fromUtf8("权限要求: 首席管理以上\n");
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, "降权操作的用法", usage);
}
