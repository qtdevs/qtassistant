#include "qtassistant.h"
#include "qtassistant_p.h"

#include <QStringBuilder>

#include "sqldatas/masterlevels.h"
#include "sqldatas/memberwelcome.h"
#include "sqldatas/memberblacklist.h"
#include "sqldatas/memberdeathhouse.h"

// class QtAssistant

bool QtAssistant::masterChangeEventFilter(const MasterChangeEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool QtAssistant::friendRequestEventFilter(const FriendRequestEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool QtAssistant::groupRequestEventFilter(const GroupRequestEvent &ev)
{
    Q_D(QtAssistant);

    // 黑名单检查，如果发现匹配，直接拒绝。
    if (d->blacklist->contains(ev.from, ev.user)) {
        rejectRequest(ev.type, ev.gbkTag);
        return false;
    }

    return false;
}

bool QtAssistant::friendAddEventFilter(const FriendAddEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool QtAssistant::memberJoinEventFilter(const MemberJoinEvent &ev)
{
    Q_D(QtAssistant);

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
    CqMemberInfo mi = memberInfo(ev.from, ev.member);
    if (mi.isValid()) {
        QString nickName = mi.nickName().trimmed();
        QString location = mi.location().trimmed();

        if (!nickName.isEmpty()) {
            if (location.isEmpty()) {
                unknownLocation = true;
                location = tr("所在地");
            }
            nameCard = '[' % location % ']' % nickName;
            renameGroupMember(ev.from, ev.member, nameCard);
        }
    }

    // 发送欢迎卡片。
    if (unknownLocation) {
        showPrimary(ev.from, nameCard, tr("欢迎加入我们！请你<span class=\"warning\">在三十分钟内发言一次</span>，并<span class=\"warning\">修改你的群名片</span>，否则将被自动踢出。<br />谢谢你的配合！"));
    } else {
        showPrimary(ev.from, nameCard, tr("欢迎加入我们！请你<span class=\"warning\">在三十分钟内发言一次</span>，否则将被自动踢出。谢谢你的配合！"));
    }

    sendGroupMessage(ev.from, cqAt(ev.member));

    return false;
}

bool QtAssistant::memberLeaveEventFilter(const MemberLeaveEvent &ev)
{
    Q_D(QtAssistant);

    // 移出新手监控，不再进行监视。
    d->welcome->removeMember(ev.from, ev.member);

    return false;
}
