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
    Q_UNUSED(ev);
    return false;
}

bool CqAssistant::memberLeaveEventFilter(const MemberLeaveEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}
