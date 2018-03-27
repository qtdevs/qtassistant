/*!
 * \class CoolQ::Interface
 * \brief 通用接口类
 */

/*!
 * \struct CoolQ::MessageEvent
 * \brief 通用消息事件
 */

/*!
 * \var qint32 CoolQ::MessageEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 CoolQ::MessageEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 CoolQ::MessageEvent::from
 * \brief 事件来源
 *
 * 当此消息来自群或讨论组时，\a from 表示群或讨论组的号码；档次消息来自个人时，\a from 永远为 0。
 */

/*!
 * \var qint32 CoolQ::MessageEvent::font
 * \brief 消息字体
 */

/*!
 * \var qint64 CoolQ::MessageEvent::sender
 * \brief 发送者
 */

/*!
 * \var const char *CoolQ::MessageEvent::gbkMsg
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
 * \struct CoolQ::MasterChangeEvent
 * \brief 管理变更事件
 */

/*!
 * \var qint32 CoolQ::MasterChangeEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 CoolQ::MasterChangeEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 CoolQ::MasterChangeEvent::from
 * \brief 事件来源
 */

/*!
 * \var qint64 CoolQ::MasterChangeEvent::master
 * \brief 执行管理（暂时无用）
 */

/*!
 * \var qint64 CoolQ::MasterChangeEvent::member
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
 * \struct CoolQ::FriendRequestEvent
 * \brief 好友请求事件
 */

/*!
 * \var qint32 CoolQ::FriendRequestEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 CoolQ::FriendRequestEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 CoolQ::FriendRequestEvent::from
 * \brief 事件来源
 */

/*!
 * \var const char *CoolQ::FriendRequestEvent::gbkMsg
 * \brief 请求附带内容
 */

/*!
 * \var const char *CoolQ::FriendRequestEvent::gbkTag
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
 * \struct CoolQ::GroupRequestEvent
 * \brief 群组请求事件
 */

/*!
 * \var qint32 CoolQ::GroupRequestEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 CoolQ::GroupRequestEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 CoolQ::GroupRequestEvent::from
 * \brief 事件来源
 */

/*!
 * \var qint64 CoolQ::GroupRequestEvent::user
 * \brief 目标成员
 */

/*!
 * \var const char *CoolQ::GroupRequestEvent::gbkMsg
 * \brief 请求附带内容
 */

/*!
 * \var const char *CoolQ::GroupRequestEvent::gbkTag
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
 * \struct CoolQ::FriendAddEvent
 * \brief 好友添加事件
 */

/*!
 * \var qint32 CoolQ::FriendAddEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 CoolQ::FriendAddEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 CoolQ::FriendAddEvent::from
 * \brief 事件来源
 */

/*struct FriendAddEvent
{
    qint32 type;
    qint32 time;
    qint64 from;
};*/

/*!
 * \struct CoolQ::MemberJoinEvent
 * \brief 成员加入事件
 */

/*!
 * \var qint32 CoolQ::MemberJoinEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 CoolQ::MemberJoinEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 CoolQ::MemberJoinEvent::from
 * \brief 事件来源
 */

/*!
 * \var qint64 CoolQ::MemberJoinEvent::master
 * \brief 执行管理
 */

/*!
 * \var qint64 CoolQ::MemberJoinEvent::member
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
 * \struct CoolQ::MemberLeaveEvent
 * \brief 成员离开事件
 */

/*!
 * \var qint32 CoolQ::MemberLeaveEvent::type
 * \brief 事件类型
 */

/*!
 * \var qint32 CoolQ::MemberLeaveEvent::time
 * \brief 发送时间
 */

/*!
 * \var qint64 CoolQ::MemberLeaveEvent::from
 * \brief 事件来源
 */

/*!
 * \var qint64 CoolQ::MemberLeaveEvent::master
 * \brief 执行管理
 */

/*!
 * \var qint64 CoolQ::MemberLeaveEvent::member
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

#include "CoolQInterface.h"
#include "CoolQInterface_p.h"

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

/*!
 * \brief 构造函数
 */
Interface::Interface(InterfacePrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
 * \brief 析构函数
 */
Interface::~Interface()
{
}

/*!
 * \brief 标准初始化函数
 */
bool Interface::initialize()
{
    return true;
}

// class InterfacePrivate

/*!
 * \internal
 */
InterfacePrivate::InterfacePrivate()
    : q_ptr(nullptr)
{
}

/*!
 * \internal
 */
InterfacePrivate::~InterfacePrivate()
{
}

} // namespace CoolQ
