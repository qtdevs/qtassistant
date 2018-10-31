#include "AssistantModule.h"
#include "AssistantModule_p.h"

#include <QRegularExpression>
#include <QStringBuilder>
#include <QTextStream>
#include <QtDebug>

#include "SqlDatas/MemberWatchlist.h"
#include "SqlDatas/MemberBlacklist.h"

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

    // 移出观察室，不再进行观察。
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

    // return false;
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

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    // ts << "<code>  </code>" << QString(u8"重命名命令: ") << QString(u8"<code>重命名 [@成员]  新名片</code>\n");
    // ts << "<code>  </code>" << QString(u8"格式化命令: ") << QString(u8"<code>格式化 [@成员] [@...]</code>\n");
    // ts << "\n";
    ts << "<code>  </code>" << QString(u8"禁言命令: ") << QString(u8"<code>禁言 @成员 @...</code>\n");
    ts << "<code>  </code>" << QString(u8"解禁命令: ") << QString(u8"<code>解禁 @成员 @...</code>\n");
    ts << "<code>  </code>" << QString(u8"踢出命令: ") << QString(u8"<code>踢出 @成员 @...</code>\n");
    ts << "\n";
    ts << "<code>  </code>" << QString(u8"观察室命令: ") << QString(u8"<code>观察室 [+] [@成员 @...]</code>\n");
    ts << "<code>  </code>" << QString(u8"黑名单命令: ") << QString(u8"<code>黑名单 [-] [QQ号码 ...]</code>\n");
    ts << "</pre>";

    ts.flush();

    showPrompt(ev.from, QString(u8"命令清单"), usage);
}

void AssistantModule::groupRenameAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    // !!! 由于此操作的特殊性，命令行解析自行分析

    QString nameCard = CoolQ::trGbk(ev.gbkMsg);

    QList<qint64> uids;
    bool prefixFound = false;
    bool invalidArgs = false;
    for (const QString &arg : args) {
        if (arg.startsWith("[CQ:at")) {
            int i = arg.lastIndexOf("]");
            QString uid = arg.mid(10, i - 10);
            uids.append(uid.toLongLong());

            // 如果找到了号码，同时找到了部分新名片，我们认为它是无效的操作。
            if (prefixFound) {
                invalidArgs = true;
                break;
            }

            // 如果只找到一个号码，我们认为号码以后的部分都是新名片；否则就是无效的操作。
            if (uids.count() == 1) {
                int r = nameCard.indexOf(arg);
                nameCard = nameCard.mid(r + arg.count());
            } else {
                invalidArgs = true;
                break;
            }
        } else if (uids.isEmpty()) {
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

    if (!uids.isEmpty()) {
    }

    // 在这里，我们对新名片做规范化处理。
    d->formatNameCard(nameCard);

    if (nameCard.isEmpty()) {
        groupRenameHelpAction(ev.from);
        return;
    }

    // 执行具体操作

    if (uids.isEmpty()) {
    } else {
        renameGroupMember(ev.from, uids.at(0), nameCard);
    }

    showSuccess(ev.from, QString(u8"修改名片"), QString(u8"新的名片：%1").arg(nameCard));
}

void AssistantModule::groupFormatAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    auto uids = d->findUsers(args);
    if (uids.isEmpty()) {
        groupBanHelpAction(ev.from);
        return;
    }

    QRegularExpression correctNameCard("^[\\[【][^\\[\\]【】]+[\\]】][^\\[\\]【】]+$");
    QRegularExpression correctLocation("^[\\[【][^\\[\\]【】]+[\\]】]$");
    QRegularExpression correctNickName("^[^\\[\\]【】]+$");

    // 执行具体操作

    QList<qint64> affectedIds;

    for (auto uid : uids) {
        auto mi = memberInfo(ev.from, uid, false);
        if (mi.isValid()) {
            if (mi.permission() == 1) {
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
                    if (renameGroupMember(ev.from, uid, nameCard) == NoError) {
                        affectedIds.append(uid);
                    } else {
                        qDebug("RPC failed: %lld", uid);
                    }
                }
            } else {
                qDebug("Invalid permission: %lld", uid);
            }
        } else {
            qDebug("Get member info failed: %lld", uid);
        }
    }

    showSuccessList(ev.from, QString(u8"格式化后新的名片"), affectedIds);
}

void AssistantModule::groupBanAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    auto uids = d->findUsers(args);
    if (uids.isEmpty()) {
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
    int c = args.count() - uids.count();
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

    // 执行具体操作

    QList<qint64> affectedIds;

    for (auto uid : uids) {
        auto mi = memberInfo(ev.from, uid, false);
        if (mi.isValid()) {
            if (mi.permission() == 1) {
                if (banGroupMember(ev.from, uid, duration) == NoError) {
                    affectedIds.append(uid);
                } else {
                    qDebug("RPC failed: %lld", uid);
                }
            } else {
                qDebug("Invalid permission: %lld", uid);
            }
        } else {
            qDebug("Get member info failed: %lld", uid);
        }
    }

    showSuccessList(ev.from, QString(u8"下列成员已禁言"), affectedIds);
}

void AssistantModule::groupKickAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    auto uids = d->findUsers(args);
    if (uids.isEmpty()) {
        groupKickHelpAction(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != uids.count()) {
        groupKickHelpAction(ev.from);
        return;
    }

    // 执行具体操作

    QList<qint64> affectedIds;

    for (auto uid : uids) {
        auto mi = memberInfo(ev.from, uid, false);
        if (mi.isValid()) {
            if (mi.permission() == 1) {
                if (kickGroupMember(ev.from, uid, false) == NoError) {
                    affectedIds.append(uid);
                } else {
                    qDebug("RPC failed: %lld", uid);
                }
            } else {
                qDebug("Invalid permission: %lld", uid);
            }
        } else {
            qDebug("Get member info failed: %lld", uid);
        }
    }

    showSuccessList(ev.from, QString(u8"下列成员已踢出"), affectedIds);
}

void AssistantModule::groupUnbanAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    auto uids = d->findUsers(args);
    if (uids.isEmpty()) {
        groupUnbanHelpAction(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != uids.count()) {
        groupUnbanHelpAction(ev.from);
        return;
    }

    // 执行具体操作

    QList<qint64> affectedIds;

    for (auto uid : uids) {
        auto mi = memberInfo(ev.from, uid, false);
        if (mi.isValid()) {
            if (mi.permission() == 1) {
                if (banGroupMember(ev.from, uid, 0) == NoError) {
                    affectedIds.append(uid);
                } else {
                    qDebug("RPC failed: %lld", uid);
                }
            } else {
                qDebug("Invalid permission: %lld", uid);
            }
        } else {
            qDebug("Get member info failed: %lld", uid);
        }
    }

    showSuccessList(ev.from, QString(u8"下列成员已解禁"), affectedIds);
}

void AssistantModule::groupWatchlistAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_UNUSED(args);

    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    // 打印观察室。

    QList<qint64> uids;

    QHashIterator<CoolQ::Member, qint64> i(d->watchlist->members());
    while (i.hasNext()) {
        if (i.next().key().first == ev.from) {
            uids.append(i.key().second);
        }
    }

    if (uids.isEmpty())
        showPrompt(ev.from, QString(u8"观察室"), QString(u8"观察室中没有任何成员"));
    else
        showPromptList(ev.from, QString(u8"观察室"), uids);
}

void AssistantModule::groupAddWatchlistAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    auto uids = d->findUsers(args);
    if (uids.isEmpty()) {
        groupWatchlistHelpAction(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != uids.count()) {
        groupWatchlistHelpAction(ev.from);
        return;
    }

    // 执行具体操作

    QList<qint64> affectedIds;

    for (auto uid : uids) {
        auto mi = memberInfo(ev.from, uid, false);
        if (mi.isValid()) {
            if (mi.permission() == 1) {
                d->watchlist->addMember(ev.from, uid);
                affectedIds.append(uid);
            } else {
                qDebug("Invalid permission: %lld", uid);
            }
        } else {
            qDebug("Get member info failed: %lld", uid);
        }
    }

    showSuccessList(ev.from, QString(u8"下列成员已加入观察室"), affectedIds);
}

void AssistantModule::groupRemoveWatchlistAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    auto uids = d->findUsers(args);
    if (uids.isEmpty()) {
        groupWatchlistHelpAction(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != uids.count()) {
        groupWatchlistHelpAction(ev.from);
        return;
    }

    // 执行具体操作

    for (auto uid : uids)
        d->watchlist->removeMember(ev.from, uid);

    showSuccessList(ev.from, QString(u8"下列成员已移出观察室"), uids);
}

void AssistantModule::groupBlacklistAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_UNUSED(args);

    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    // 打印黑名单。

    QList<qint64> uids;

    QHashIterator<CoolQ::Member, qint64> i(d->blacklist->members());
    while (i.hasNext()) {
        if (i.next().key().first == ev.from) {
            uids.append(i.key().second);
        }
    }

    if (uids.isEmpty())
        showPrompt(ev.from, QString(u8"黑名单"), QString(u8"黑名单中没有任何成员"));
    else
        showPromptList(ev.from, QString(u8"黑名单"), uids);
}

void AssistantModule::groupAddBlacklistAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    auto uids = d->findUsers(args);
    if (uids.isEmpty()) {
        groupBlacklistHelpAction(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != uids.count()) {
        groupBlacklistHelpAction(ev.from);
        return;
    }

    // 执行具体操作

    QList<qint64> affectedIds;

    for (auto uid : uids) {
        auto mi = memberInfo(ev.from, uid, false);
        if (mi.isValid()) {
            if (mi.permission() == 1) {
                d->blacklist->addMember(ev.from, uid);
                affectedIds.append(uid);
            } else {
                qDebug("Invalid permission: %lld", uid);
            }
        } else {
            qDebug("Get member info failed: %lld", uid);
        }
    }

    showSuccessList(ev.from, QString(u8"下列成员已加入黑名单"), affectedIds);
}

void AssistantModule::groupRemoveBlacklistAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() < 2) {
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    auto uids = d->findUsers(args);
    if (uids.isEmpty()) {
        groupBlacklistHelpAction(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != uids.count()) {
        groupBlacklistHelpAction(ev.from);
        return;
    }

    // 执行具体操作

    for (auto uid : uids)
        d->blacklist->removeMember(ev.from, uid);

    showSuccessList(ev.from, QString(u8"下列成员已移出黑名单"), uids);
}

void AssistantModule::groupMemberAction(const CoolQ::MessageEvent &ev, const QStringList &args)
{
    Q_D(AssistantModule);

    // 普通成员不应答。
    auto mi = memberInfo(ev.from, ev.sender, false);
    if (mi.permission() != 3) {
        return;
    }

    // 获取目标成员的等级信息，此操作必须有至少一个目标成员。
    auto uids = d->findUsers(args);
    if (uids.isEmpty()) {
        groupMemberHelpAction(ev.from);
        return;
    }

    // 检查参数有效性。
    if (args.count() != uids.count()) {
        groupMemberHelpAction(ev.from);
        return;
    }

    // 执行具体操作

    for (qint64 uid : uids) {
        auto mi = memberInfo(ev.from, uid, false);

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

        showPrimary(ev.from, QString::number(uid), reports);

        showWelcomes(ev.from, ev.sender);
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

void AssistantModule::groupWatchlistHelpAction(qint64 gid)
{
    QString usage;
    QTextStream ts(&usage);

    ts << "<pre>";
    ts << QString(u8"命令：<code>观察室 [参数] [@成员 @...]</code>") << "\n";
    ts << QString(u8"权限要求：") << "1+\n";
    ts << QString(u8"参数列表：") << "\n";
    ts << QString(u8"<code>  加入(+)</code>：") << QString(u8"加入观察") << "\n";
    ts << "</pre>";

    ts.flush();

    showPrompt(gid, QString(u8"观察室用法"), usage);
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

