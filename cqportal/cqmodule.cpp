/*!
 * \class CqModule
 * \brief 插件入口类
 */

/*!
 * \enum CqModule::Result
 * \brief CqModule 执行结果
 *
 * 酷Q API 密切相关的执行函数都会返回此枚举作为结果表达。
 */

/*!
 * \var CqModule::NoError
 * \brief 没有错误（成功）
 *
 * 如果函数执行成功，都会返回当前值。
 */

/*!
 * \var CqModule::Unknown
 * \brief 未知错误
 *
 * 不在当前枚举内的错误值，都会返回 Unknown。
 */

#include "cqmodule.h"
#include "cqmodule_p.h"

#include "cqengine.h"
#include "cqengine_p.h"

#include <QDir>
#include <QPixmap>
#include <QStringBuilder>
#include <QUuid>

#include "cqsqlite_p.h"
#include "cqapi/cqapi.h"

// class CqModule

/*!
 * \brief 构造函数
 *
 * \internal
 */
CqModule::CqModule(CqEngine *parent)
    : QObject(parent)
    , d_ptr(new CqModulePrivate())
{
    Q_D(CqModule);

    d->q_ptr = this;

    d->currentId = CQ_getLoginQQ(CqEnginePrivate::accessToken);

    QString path = convert(CQ_getAppDirectory(CqEnginePrivate::accessToken));
    d->basePath = QDir::cleanPath(path);
    d->resPath = QDir::cleanPath(path % "/../../data");
    d->imagePath = QDir::cleanPath(path % "/../../data/image");

    QString sqlitePath = d->basePath % '/' % QString::number(d->currentId);
    CqSqlitePrivate::basePath = QDir::cleanPath(sqlitePath);
    QDir().mkpath(CqSqlitePrivate::basePath);
}

/*!
 * \brief 析构函数
 *
 * \internal
 */
CqModule::~CqModule()
{
}

/*!
 * \brief 字符串编码转换函数
 *
 * 将 QString 类型的 \a str 转换成 GBK 编码的 QByteArray。
 */
QByteArray CqModule::convert(const QString &str)
{
    return str.toLocal8Bit();
}

/*!
 * \brief 字符串编码转换函数
 *
 * 将 GBK 编码的 \a gbkStr 转换成 QString。
 */
QString CqModule::convert(const char *gbkStr)
{
    return QString::fromLocal8Bit(gbkStr);
}

/*!
 * \brief 字符串编码转换函数
 *
 * 将 GBK 编码的 \a str 转换成 QString。
 */
QString CqModule::convert(const QByteArray &str)
{
    return QString::fromLocal8Bit(str);
}

/*!
 * \brief 返回酷Q中@他人的字符串
 *
 * 此函数将返回酷Q中@他人的字符串。
 */
QString CqModule::at(qint64 uid)
{
    return QString::fromLatin1("[CQ:at,qq=%1]").arg(uid);
}

/*!
 * \brief 返回酷Q中的 emoji 表情
 *
 * 此函数将返回酷Q中的 emoji 表情。
 */
QString CqModule::cqEmoji(int emojiCode)
{
    return QString::fromLatin1("[CQ:emoji,id=%1]").arg(emojiCode);
}

/*!
 * \brief 返回酷Q中发送图片的字符串
 *
 * 此函数将返回发送图片的字符串。此文件需要保存在酷Q待发送图片目录中。
 * \sa CqModule::loadImage CqModule::saveImage
 */
QString CqModule::cqImage(const QString &name)
{
    return QString::fromLatin1("[CQ:image,file=%1]").arg(name);
}

QString CqModule::face(int value)
{
    return QString::fromLatin1("[CQ:face,id=%1]").arg(value);
}

/*!
 * \brief 通过数值返回 QString 格式的 emoji 表情
 *
 * 此函数将通过数值返回 QString 格式的 emoji 表情。
 */
QString CqModule::emoji(qint32 value)
{
    char utf8Emoji[5] = {0};

    utf8Emoji[0] = char(0xF0);
    utf8Emoji[1] = char(((value >> 0xC) & 0x3f) | 0x80);
    utf8Emoji[2] = char(((value >> 0x6) & 0x3f) | 0x80);
    utf8Emoji[3] = char(((value >> 0x0) & 0x3f) | 0x80);
    utf8Emoji[4] = 0;

    return QString::fromUtf8(utf8Emoji);
}

/*!
 * \brief 返回当前号码
 */
qint64 CqModule::currentId() const
{
    return d_ptr->currentId;
}

/*!
 * \brief 返回插件路径
 */
QString CqModule::basePath() const
{
    return d_ptr->basePath;
}

/*!
 * \brief 返回插件目录内的文件
 *
 * 将插件路径与文件名 \a srcName 合并，返回此文件名的绝对路径。
 */
QString CqModule::appFilePath(const char *srcName) const
{
    return QDir::cleanPath(d_ptr->basePath % '/' % QString::fromUtf8(srcName));
}

/*!
 * \brief 返回插件目录内的文件
 *
 * 将插件路径与文件名 \a name 合并，返回此文件名的绝对路径。
 */
QString CqModule::appFilePath(const QString &name) const
{
    return QDir::cleanPath(d_ptr->basePath % '/' % name);
}

/*!
 * \brief 返回插件目录内的文件
 *
 * 将插件路径与当前号码，文件名 \a srcName 合并，返回在此号码下的文件名的绝对路径。
 */
QString CqModule::usrFilePath(const char *srcName) const
{
    return QDir::cleanPath(d_ptr->basePath % '/' % QString::number(d_ptr->currentId) % '/' % QString::fromUtf8(srcName));
}

/*!
 * \brief 返回插件目录内的文件
 *
 * 将插件路径与当前号码，文件名 \a name 合并，返回在此号码下的文件名的绝对路径。
 */
QString CqModule::usrFilePath(const QString &name) const
{
    return QDir::cleanPath(d_ptr->basePath % '/' % QString::number(d_ptr->currentId) % '/' % name);
}

QString CqModule::resFilePath(const char *srcName) const
{
    return QDir::cleanPath(d_ptr->resPath % '/' % QString::fromUtf8(srcName));
}

QString CqModule::resFilePath(const QString &name) const
{
    return QDir::cleanPath(d_ptr->resPath % '/' % name);
}

/*!
 * \brief 处理个人消息事件的过滤器
 *
 * 默认的事件过滤器什么也不做，只返回 false。
 * \return true 表示拦截此次事件；false 表示不拦截。
 */
bool CqModule::privateMessageEvent(const MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

/*!
 * \brief 处理群组消息事件的过滤器
 *
 * 默认的事件过滤器什么也不做，只返回 false。
 * \return true 表示拦截此次事件；false 表示不拦截。
 */
bool CqModule::groupMessageEvent(const MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

/*!
 * \brief 处理会话消息事件的过滤器
 *
 * 默认的事件过滤器什么也不做，只返回 false。
 * \return true 表示拦截此次事件；false 表示不拦截。
 */
bool CqModule::discussMessageEvent(const MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

/*!
 * \brief 处理管理变更事件的过滤器
 *
 * 默认的事件过滤器什么也不做，只返回 false。
 * \return true 表示拦截此次事件；false 表示不拦截。
 */
bool CqModule::masterChangeEvent(const MasterChangeEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

/*!
 * \brief 处理好友请求事件的过滤器
 *
 * 默认的事件过滤器什么也不做，只返回 false。
 * \return true 表示拦截此次事件；false 表示不拦截。
 */
bool CqModule::friendRequestEvent(const FriendRequestEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

/*!
 * \brief 处理群组请求事件的过滤器
 *
 * 默认的事件过滤器什么也不做，只返回 false。
 * \return true 表示拦截此次事件；false 表示不拦截。
 */
bool CqModule::groupRequestEvent(const GroupRequestEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

/*!
 * \brief 处理好友添加事件的过滤器
 *
 * 默认的事件过滤器什么也不做，只返回 false。
 * \return true 表示拦截此次事件；false 表示不拦截。
 */
bool CqModule::friendAddEvent(const FriendAddEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

/*!
 * \brief 处理成员加入事件的过滤器
 *
 * 默认的事件过滤器什么也不做，只返回 false。
 * \return true 表示拦截此次事件；false 表示不拦截。
 */
bool CqModule::memberJoinEvent(const MemberJoinEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

/*!
 * \brief 处理成员离开事件的过滤器
 *
 * 默认的事件过滤器什么也不做，只返回 false。
 * \return true 表示拦截此次事件；false 表示不拦截。
 */
bool CqModule::memberLeaveEvent(const MemberLeaveEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

/*!
 * \brief 发送个人消息
 *
 * 向个人 \a uid 发送消息 \a gbkMsg。
 * \note gbkMsg 是指使用 GBK 编码的文本，而本项目源代码是 UTF-8 编码的。
 * \return CqModule::Result
 */
CqModule::Result CqModule::sendPrivateMessage(qint64 uid, const char *gbkMsg) const
{
    return CqModulePrivate::result(CQ_sendPrivateMsg(CqEnginePrivate::accessToken, uid, gbkMsg));
}

/*!
 * \brief 发送群组消息
 *
 * 向群组 \a gid 发送消息 \a gbkMsg。
 * \note gbkMsg 是指使用 GBK 编码的文本，而本项目源代码是 UTF-8 编码的。
 * \return CqModule::Result
 */
CqModule::Result CqModule::sendGroupMessage(qint64 gid, const char *gbkMsg) const
{
    return CqModulePrivate::result(CQ_sendGroupMsg(CqEnginePrivate::accessToken, gid, gbkMsg));
}

/*!
 * \brief 发送会话消息
 *
 * 向会话 \a did 发送消息 \a gbkMsg。
 * \note gbkMsg 是指使用 GBK 编码的文本，而本项目源代码是 UTF-8 编码的。
 * \return CqModule::Result
 */
CqModule::Result CqModule::sendDiscussMessage(qint64 did, const char *gbkMsg) const
{
    return CqModulePrivate::result(CQ_sendDiscussMsg(CqEnginePrivate::accessToken, did, gbkMsg));
}

/*!
 * \brief 发送个人消息
 *
 * 向个人 \a uid 发送消息 \a gbkMsg。
 * \return CqModule::Result
 */
CqModule::Result CqModule::sendPrivateMessage(qint64 uid, const QString &msg) const
{
    return sendPrivateMessage(uid, convert(msg).constData());
}

/*!
 * \brief 发送群组消息
 *
 * 向群组 \a gid 发送消息 \a gbkMsg。
 * \return CqModule::Result
 */
CqModule::Result CqModule::sendGroupMessage(qint64 gid, const QString &msg) const
{
    return sendGroupMessage(gid, convert(msg).constData());
}

/*!
 * \brief 发送会话消息
 *
 * 向会话 \a did 发送消息 \a gbkMsg。
 * \return CqModule::Result
 */
CqModule::Result CqModule::sendDiscussMessage(qint64 did, const QString &msg) const
{
    return sendDiscussMessage(did, convert(msg).constData());
}

/*!
 * \brief 禁言群组成员
 *
 * 禁言群组 \a gid 中的成员 \a uid。\a duration 最短 60 秒，最长 30 天；0 表示解除禁言。（单位：秒）
 * \return CqModule::Result
 */
CqModule::Result CqModule::banGroupMember(qint64 gid, qint64 uid, int duration)
{
    return CqModulePrivate::result(CQ_setGroupBan(CqEnginePrivate::accessToken, gid, uid, duration));
}

/*!
 * \brief 踢出群组成员
 *
 * 踢出群组 \a gid 中的成员 \a uid。\a lasting 为 true 表示永久禁言；false 表示只是踢出，不加入黑名单。
 * \return CqModule::Result
 */
CqModule::Result CqModule::kickGroupMember(qint64 gid, qint64 uid, bool lasting)
{
    return CqModulePrivate::result(CQ_setGroupKick(CqEnginePrivate::accessToken, gid, uid, lasting));
}

/*!
 * \brief 设置群组管理
 *
 * 设置群组 \a gid 中的管理 \a uid。\a enable 为 true 表示设置为管理；false 表示回复为成员。
 * \return CqModule::Result
 */
CqModule::Result CqModule::adminGroupMember(qint64 gid, qint64 uid, bool enabled)
{
    return CqModulePrivate::result(CQ_setGroupAdmin(CqEnginePrivate::accessToken, gid, uid, enabled));
}

/*!
 * \brief 修改成员名片
 *
 * 设置群组 \a gid 中的成员 \a uid 新的名片 \a gbkNewNameCard。
 * \note gbkNewNameCard 是指使用 GBK 编码的文本，而本项目源代码是 UTF-8 编码的。
 * \return CqModule::Result
 */
CqModule::Result CqModule::renameGroupMember(qint64 gid, qint64 uid, const char *gbkNewNameCard)
{
    return CqModulePrivate::result(CQ_setGroupCard(CqEnginePrivate::accessToken, gid, uid, gbkNewNameCard));
}

/*!
 * \brief 修改成员名片
 *
 * 设置群组 \a gid 中的成员 \a uid 新的名片 \a newNameCard
 * \return CqModule::Result
 */
CqModule::Result CqModule::renameGroupMember(qint64 gid, qint64 uid, const QString &newNameCard)
{
    return CqModulePrivate::result(CQ_setGroupCard(CqEnginePrivate::accessToken, gid, uid, convert(newNameCard).constData()));
}

/*!
 * \brief 接受好友添加请求
 *
 * 接受添加为好友的请求。需要将 FriendRequestEvent::gbkTag 直接传递给 \a gbkTag。
 * \return CqModule::Result
 */
CqModule::Result CqModule::acceptRequest(const char *gbkTag)
{
    return CqModulePrivate::result(CQ_setFriendAddRequest(CqEnginePrivate::accessToken, gbkTag, REQUEST_ALLOW, ""));
}

/*!
 * \brief 拒绝好友添加请求
 *
 * 拒绝添加为好友的请求。需要将 FriendRequestEvent::gbkTag 直接传递给 \a gbkTag。
 * \return CqModule::Result
 */
CqModule::Result CqModule::rejectRequest(const char *gbkTag)
{
    return CqModulePrivate::result(CQ_setFriendAddRequest(CqEnginePrivate::accessToken, gbkTag, REQUEST_DENY, ""));
}

/*!
 * \brief 接受成员添加请求
 *
 * 接受个人加入群组的请求。需要将 GroupRequestEvent::type 直接传递给 \a type，GroupRequestEvent::gbkTag 直接传递给 \a gbkTag。
 * \return CqModule::Result
 */
CqModule::Result CqModule::acceptRequest(qint32 type, const char *gbkTag)
{
    if (1 == type) {
        return CqModulePrivate::result(CQ_setGroupAddRequestV2(CqEnginePrivate::accessToken, gbkTag, REQUEST_GROUPADD, REQUEST_ALLOW, ""));
    } else if (2 == type) {
        return CqModulePrivate::result(CQ_setGroupAddRequestV2(CqEnginePrivate::accessToken, gbkTag, REQUEST_GROUPINVITE, REQUEST_ALLOW, ""));
    }

    return Result::Unknown;
}

/*!
 * \brief 拒绝成员添加请求
 *
 * 拒绝个人加入群组的请求。需要将 GroupRequestEvent::type 直接传递给 \a type，GroupRequestEvent::gbkTag 直接传递给 \a gbkTag。
 * \return CqModule::Result
 */
CqModule::Result CqModule::rejectRequest(qint32 type, const char *gbkTag)
{
    if (1 == type) {
        return CqModulePrivate::result(CQ_setGroupAddRequestV2(CqEnginePrivate::accessToken, gbkTag, REQUEST_GROUPADD, REQUEST_DENY, ""));
    } else if (2 == type) {
        return CqModulePrivate::result(CQ_setGroupAddRequestV2(CqEnginePrivate::accessToken, gbkTag, REQUEST_GROUPINVITE, REQUEST_DENY, ""));
    }

    return Result::Unknown;
}

/*!
 * \brief 退出群组
 *
 * 这个帐号将退出群组 \a gid，注意安全！
 * \return CqModule::Result
 */
CqModule::Result CqModule::leaveGroup(qint64 gid)
{
    return CqModulePrivate::result(CQ_setGroupLeave(CqEnginePrivate::accessToken, gid, false));
}

/*!
 * \brief 退出会话
 *
 * 这个帐号将退出会话 \a did，注意安全！
 * \return CqModule::Result
 */
CqModule::Result CqModule::leaveDiscuss(qint64 did)
{
    return CqModulePrivate::result(CQ_setDiscussLeave(CqEnginePrivate::accessToken, did));
}

/*!
 * \brief 全员禁言
 *
 * 是否开启群组的全员禁言。\a muted 为 true 表示开启；否则表示关闭。
 * \return CqModule::Result
 */
CqModule::Result CqModule::mute(qint64 gid, bool muted)
{
    return CqModulePrivate::result(CQ_setGroupWholeBan(CqEnginePrivate::accessToken, gid, muted));
}

/*!
 * \brief 返回个人信息
 *
 * 此方法会返回个人 \a uid 的个人信息，默认会使用缓存数据。如果不想使用缓存，可以设置 \a cache 为 false，此时将同步获取个人信息。
 * \return 获取到的信息是否有效，可以通过返回对象的 CqPersonInfo::isValid() 函数进行验证。
 */
CqPersonInfo CqModule::personInfo(qint64 uid, bool cached)
{
    return CqPersonInfo(CQ_getStrangerInfo(CqEnginePrivate::accessToken, uid, !cached));
}

/*!
 * \brief 返回成员信息
 *
 * 此方法会返回在群组 \a gid 中的成员 \a uid 的成员信息，默认会使用缓存数据。如果不想使用缓存，可以设置 \a cache 为 false，此时将同步获取成员信息。
 * \return 获取到的信息是否有效，可以通过返回对象的 CqMemberInfo::isValid() 函数进行验证。
 */
CqMemberInfo CqModule::memberInfo(qint64 gid, qint64 uid, bool cached)
{
    return CqMemberInfo(CQ_getGroupMemberInfoV2(CqEnginePrivate::accessToken, gid, uid, !cached));
}

/*!
 * \brief 保存图片
 *
 * 将图片 \a data 保存到酷Q待发送图片目录。如果成功，返回自动生成的文件名。
 */
QString CqModule::saveImage(const QPixmap &data) const
{
    Q_D(const CqModule);

    QString uuid = QString::fromLatin1(QUuid::createUuid().toRfc4122().toHex());
    QString name = QString("%1/%2.png").arg(d->imagePath, uuid);
    if (data.save(name, "PNG", 0)) {
        return uuid % ".png";
    }

    return QString();
}

/*!
 * \brief 载入图片
 *
 * 将载入酷Q待发送图片目录内文件名为 \a name 的图片，如果成功，返回此图片对象。
 */
QPixmap CqModule::loadImage(const QString &name) const
{
    Q_UNUSED(name);
    return QPixmap();
}

// class CqModulePrivate

CqModulePrivate::CqModulePrivate()
    : q_ptr(Q_NULLPTR)
    , currentId(0)
    //
    , privateMessageEventPriority(1)
    , groupMessageEventPriority(1)
    , discussMessageEventPriority(1)
    //
    , masterChangeEventPriority(1)
    , friendRequestEventPriority(1)
    , groupRequestEventPriority(1)
    //
    , friendAddEventPriority(1)
    , memberJoinEventPriority(1)
    , memberLeaveEventPriority(1)
{
}

CqModulePrivate::~CqModulePrivate()
{
}

CqModule::Result CqModulePrivate::result(qint32 r)
{
    switch (r) {
    case 1:
        break;
    default:
        return CqModule::Unknown;
    }

    return CqModule::Unknown;
}
