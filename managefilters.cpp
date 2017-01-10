#include "cqassistant.h"
#include "cqassistant_p.h"

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
    Q_UNUSED(ev);
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
