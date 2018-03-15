#include "managemodule.h"
#include "managemodule_p.h"

#include <QStringBuilder>
#include <QTextStream>

#include "sqldatas/masterlevels.h"
#include "sqldatas/memberwelcome.h"
#include "sqldatas/memberblacklist.h"
#include "sqldatas/memberdeathhouse.h"

// class ManageModule

bool ManageModule::masterChangeEvent(const MasterChangeEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool ManageModule::friendRequestEvent(const FriendRequestEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool ManageModule::groupRequestEvent(const GroupRequestEvent &ev)
{
    Q_D(ManageModule);

    if (!d->managedGroups.contains(ev.from)) {
        return false;
    }

    // 黑名单检查，如果发现匹配，直接拒绝。
    if (d->blacklist->contains(ev.from, ev.user)) {
        rejectRequest(ev.type, ev.gbkTag);
        return false;
    }

    return false;
}

bool ManageModule::friendAddEvent(const FriendAddEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool ManageModule::memberJoinEvent(const MemberJoinEvent &ev)
{
    Q_D(ManageModule);

    if (!d->managedGroups.contains(ev.from)) {
        return false;
    }

    // 黑名单检查，如果发现匹配，直接踢出。
    if (d->blacklist->contains(ev.from, ev.member)) {
        kickGroupMember(ev.from, ev.member, false);
        return false;
    }

    // 加入新手监控，监视首次发言。
    d->welcome->addMember(ev.from, ev.member);

    // 尝试修改昵称。
    QString nameCard;
    bool unknownLocation = false;
    CqMemberInfo mi = memberInfo(ev.from, ev.member, false);
    if (mi.isValid()) {
        QString nickName = mi.nickName().trimmed();
        QString location = mi.location().trimmed();
        d->safetyNameCard(nickName);

        if (!nickName.isEmpty()) {
            if (location.isEmpty()) {
                unknownLocation = true;
                location = tr("所在地");
            }
            nameCard = '[' % location % ']' % nickName;
            renameGroupMember(ev.from, ev.member, nameCard);
        }
    }

    QString msg;
    QTextStream ts(&msg);

    for (auto fileName : d->welcomeImages) {
        ts << cqImage(fileName) << "\n";
    }
    ts << at(ev.member);
    ts.flush();

    sendGroupMessage(ev.from, msg);

    return false;
}

bool ManageModule::memberLeaveEvent(const MemberLeaveEvent &ev)
{
    Q_D(ManageModule);

    if (!d->managedGroups.contains(ev.from)) {
        return false;
    }

    // 移出新手监控，不再进行监视。
    d->welcome->removeMember(ev.from, ev.member);

    return false;
}
