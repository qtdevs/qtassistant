#ifndef CQINTERFACE_H
#define CQINTERFACE_H

#include <QObject>
#include <QVector>

namespace CoolQ {

QByteArray trGbk(const QString &str);
QString trGbk(const char *gbkStr);
QString trGbk(const QByteArray &str);

typedef QPair<qint64, qint64> Member;
typedef QVector<Member>       MemberList;

// struct MessageEvent

struct MessageEvent
{
    qint32 type;
    qint32 time;
    qint64 from;
    qint32 font;

    qint64      sender;
    const char *gbkMsg;

    bool equals(int i, int v) const
    { return (v == (0x000000ff & gbkMsg[i])); }
};

// struct MasterChangeEvent

struct MasterChangeEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    qint64 master;
    qint64 member;
};

// struct FriendRequestEvent

struct FriendRequestEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    const char *gbkMsg;
    const char *gbkTag;

    bool equals(int i, int v) const
    { return (v == (0x000000ff & gbkMsg[i])); }
};

// struct GroupRequestEvent

struct GroupRequestEvent
{
    qint32 type;
    qint32 time;
    qint64 from;
    qint64 user;

    const char *gbkMsg;
    const char *gbkTag;

    bool equals(int i, int v) const
    { return (v == (0x000000ff & gbkMsg[i])); }
};

// struct FriendAddEvent

struct FriendAddEvent
{
    qint32 type;
    qint32 time;
    qint64 from;
};

// struct MemberJoinEvent

struct MemberJoinEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    qint64 master;
    qint64 member;
};

// struct MemberLeaveEvent

struct MemberLeaveEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    qint64 master;
    qint64 member;
};

// class Interface

class InterfacePrivate;
class Interface : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Interface)

protected:
    explicit Interface(InterfacePrivate &dd, QObject *parent);
protected:
    QScopedPointer<InterfacePrivate> d_ptr;
public:
    virtual ~Interface();

public:
    virtual bool initialize();
};

} // namespace CoolQ

#endif // CQINTERFACE_H
