#ifndef CQGLOBALDEFS_H
#define CQGLOBALDEFS_H

#include <QObject>
#include <QVector>

typedef QPair<qint64, qint64> Member;
typedef QVector<Member>       MemberList;

struct MessageEvent
{
    qint32 type;
    qint32 time;
    qint64 from;
    qint32 font;

    qint64      sender;
    const char *gbkMsg;
};

struct MasterChangeEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    qint64 member;
};

struct FriendRequestEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    const char *gbkMsg;
    const char *gbkTag;
};

struct GroupRequestEvent
{
    qint32 type;
    qint32 time;
    qint64 from;
    qint64 user;

    const char *gbkMsg;
    const char *gbkTag;
};

struct FriendAddEvent
{
    qint32 type;
    qint32 time;
    qint64 from;
};

struct MemberJoinEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    qint64 master;
    qint64 member;
};

struct MemberLeaveEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    qint64 master;
    qint64 member;
};

#endif // CQGLOBALDEFS_H
