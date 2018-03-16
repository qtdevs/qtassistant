/*!
 * \struct MessageEvent
 * \brief 通用消息事件
 */

/*!
 * \var qint32 MessageEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 MessageEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 MessageEvent::from
 * \brief 事件来源
 */

/*!
 * \var qint32 MessageEvent::font
 * \brief 消息字体
 */

/*!
 * \var qint64 MessageEvent::sender
 * \brief 发送者
 */

/*!
 * \var const char *MessageEvent::gbkMsg
 * \brief 消息源
 */

/*struct MessageEvent
{
    qint32 type;
    qint32 time;
    qint64 from;
    qint32 font;

    qint64      sender;
    const char *gbkMsg;
};*/

/*!
 * \struct MasterChangeEvent
 * \brief 管理变更事件
 */

/*!
 * \var qint32 MasterChangeEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 MasterChangeEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 MasterChangeEvent::from
 * \brief 事件来源
 */

/*!
 * \var qint64 MemberJoinEvent::master
 * \brief 执行管理
 */

/*!
 * \var qint64 MasterChangeEvent::member
 * \brief 目标成员
 */

/*struct MasterChangeEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    qint64 master;
    qint64 member;
};*/

/*!
 * \struct FriendRequestEvent
 * \brief 好友请求事件
 */

/*!
 * \var qint32 FriendRequestEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 FriendRequestEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 FriendRequestEvent::from
 * \brief 事件来源
 */

/*!
 * \var const char *FriendRequestEvent::gbkMsg
 * \brief 请求附带内容
 */

/*!
 * \var const char *FriendRequestEvent::gbkTag
 * \brief 请求附带标签
 */

/*struct FriendRequestEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    const char *gbkMsg;
    const char *gbkTag;
};*/

/*!
 * \struct GroupRequestEvent
 * \brief 群组请求事件
 */

/*!
 * \var qint32 GroupRequestEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 GroupRequestEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 GroupRequestEvent::from
 * \brief 事件来源
 */

/*!
 * \var qint64 GroupRequestEvent::user
 * \brief 目标成员
 */

/*!
 * \var const char *GroupRequestEvent::gbkMsg
 * \brief 请求附带内容
 */

/*!
 * \var const char *GroupRequestEvent::gbkTag
 * \brief 请求附带标签
 */

/*struct GroupRequestEvent
{
    qint32 type;
    qint32 time;
    qint64 from;
    qint64 user;

    const char *gbkMsg;
    const char *gbkTag;
};*/

/*!
 * \struct FriendAddEvent
 * \brief 好友添加事件
 */

/*!
 * \var qint32 FriendAddEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 FriendAddEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 FriendAddEvent::from
 * \brief 事件来源
 */

/*struct FriendAddEvent
{
    qint32 type;
    qint32 time;
    qint64 from;
};*/

/*!
 * \struct MemberJoinEvent
 * \brief 成员加入事件
 */

/*!
 * \var qint32 MemberJoinEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 MemberJoinEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 MemberJoinEvent::from
 * \brief 事件来源
 */

/*!
 * \var qint64 MemberJoinEvent::master
 * \brief 执行管理
 */

/*!
 * \var qint64 MemberJoinEvent::member
 * \brief 被执行人
 */

/*struct MemberJoinEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    qint64 master;
    qint64 member;
};*/

/*!
 * \struct MemberLeaveEvent
 * \brief 成员离开事件
 */

/*!
 * \var qint32 MemberLeaveEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 MemberLeaveEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 MemberLeaveEvent::from
 * \brief 事件来源
 */

/*!
 * \var qint64 MemberLeaveEvent::master
 * \brief 执行管理
 */

/*!
 * \var qint64 MemberLeaveEvent::member
 * \brief 被执行人
 */

/*struct MemberLeaveEvent
{
    qint32 type;
    qint32 time;
    qint64 from;

    qint64 master;
    qint64 member;
};*/

#include "CqInterface.h"
#include "CqInterface_p.h"

namespace CoolQ {


/*!
 * \brief 字符串编码转换函数
 *
 * 将 QString 类型的 \a str 转换成 GBK 编码的 QByteArray。
 */
QByteArray trGbk(const QString &str)
{
    return str.toLocal8Bit();
}

/*!
 * \brief 字符串编码转换函数
 *
 * 将 GBK 编码的 \a gbkStr 转换成 QString。
 */
QString trGbk(const char *gbkStr)
{
    return QString::fromLocal8Bit(gbkStr);
}

/*!
 * \brief 字符串编码转换函数
 *
 * 将 GBK 编码的 \a str 转换成 QString。
 */
QString trGbk(const QByteArray &str)
{
    return QString::fromLocal8Bit(str);
}

// class Interface

Interface::Interface(InterfacePrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

Interface::~Interface()
{
}

bool Interface::initialize()
{
    return true;
}

// class InterfacePrivate

InterfacePrivate::InterfacePrivate()
    : q_ptr(nullptr)
{
}

InterfacePrivate::~InterfacePrivate()
{
}

} // namespace CoolQ
