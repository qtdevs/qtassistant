#include "cqassistant.h"
#include "cqassistant_p.h"

#include <QCoreApplication>
#include <QStringBuilder>
#include <QTextStream>

#include "datas/masterlevels.h"
#include "datas/memberwelcome.h"
#include "datas/memberblacklist.h"
#include "datas/memberdeathhouse.h"

bool CqAssistant::masterChangeEventFilter(const MasterChangeEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool CqAssistant::friendRequestEventFilter(const FriendRequestEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool CqAssistant::groupRequestEventFilter(const GroupRequestEvent &ev)
{
    Q_D(CqAssistant);

    if (d->blacklist->contains(ev.from, ev.user)) {
        // sendGroupMessage(ev.from, tr("%1 is in the blacklist, reject.").arg(ev.user));
        rejectRequest(ev.type, ev.gbkTag);
    }

    return false;
}

bool CqAssistant::friendAddEventFilter(const FriendAddEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool CqAssistant::memberJoinEventFilter(const MemberJoinEvent &ev)
{
    Q_D(CqAssistant);

    if (d->blacklist->contains(ev.from, ev.member)) {
        // sendGroupMessage(ev.from, tr("%1 is in the blacklist, reject.").arg(ev.user));
        kickGroupMember(ev.from, ev.member, false);
    } else {
        d->welcome->addMember(ev.from, ev.member);

        QString nickName;
        QString location;

        CqMemberInfo mi = memberInfo(ev.from, ev.member);
        if (mi.isValid()) {
            nickName = mi.nickName();
            location = mi.location();
        }

        if (!location.isEmpty()) {
            QString newNameCard = QString::fromLatin1("[%1]%2").arg(location, nickName);
            renameGroupMember(ev.from, ev.member, newNameCard);
        }

        sendGroupMessage(ev.from, tr("%1, Welcome to join us, please say something in 30 minutes.").arg(at(ev.member)));
    }

    return false;
}

bool CqAssistant::memberLeaveEventFilter(const MemberLeaveEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}
