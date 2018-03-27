/*!
 * \class CoolQ::MessageFilter
 * \brief 消息过滤类
 */

#include "CoolQMessageFilter.h"
#include "CoolQMessageFilter_p.h"

#include "CoolQServiceEngine.h"
#include "CoolQServiceEngine_p.h"

#include "CoolQServiceModule.h"
#include "CoolQServiceModule_p.h"

namespace CoolQ {

// class MessageFilter

/*!
 * \brief 构造函数
 *
 * \internal
 */
MessageFilter::MessageFilter(MessageFilterPrivate &dd, ServiceModule *parent)
    : Interface(dd, parent)
{
    if (auto module = ServiceModulePrivate::get(parent)) {
        module->installFilter(this);
    }

    Q_D(MessageFilter);
    d->engine = parent->engine();
    d->module = parent;
}

/*!
 * \brief 构造函数
 */
MessageFilter::MessageFilter(ServiceModule *parent)
    : Interface(*new MessageFilterPrivate(), parent)
{
    if (auto module = ServiceModulePrivate::get(parent)) {
        module->installFilter(this);
    }

    Q_D(MessageFilter);
    d->engine = parent->engine();
    d->module = parent;
}

/*!
 * \brief 析构函数
 */
MessageFilter::~MessageFilter()
{
}

/*!
 * \brief 返回所属的引擎对象指针。
 */
ServiceEngine *MessageFilter::engine() const
{
    Q_D(const MessageFilter);

    return d->engine;
}

/*!
 * \brief 返回所属的模块对象指针。
 */
ServiceModule *MessageFilter::module() const
{
    Q_D(const MessageFilter);

    return d->module;
}

/*!
 * \enum MessageFilter::FilterType
 * \brief 消息过滤器类型
 */

/*!
 * \var MessageFilter::PrivateFilter
 * \brief 私有消息过滤器类型
 */

/*!
 * \var MessageFilter::GroupFilter
 * \brief 群组消息过滤器类型
 */

/*!
 * \var MessageFilter::DiscussFilter
 * \brief 讨论组消息过滤器类型
 */

/*!
 * \fn MessageFilter::Filters MessageFilter::filters() const = 0
 * \brief 返回过滤器类型
 *
 * 这里可以返回三种类型，表名此种过滤器过滤哪些信息。三种类型可以进行与或操作。
 * \sa MessageFilter::FilterType
 */

/*!
 * \brief 返回关键词列表。
 *
 * 以关键字开头作为命令的过滤器，可以通过此方法返回过滤关键词。
 * 比如：禁言 @成员 @...，此处的列表应返回("禁言")。
 * 如果返回空列表，讲作为整体过滤器使用，类似敏感词汇检测，需要整体过滤器。
 * \note 关键词长度不能超过 16 个汉字，或 32 个英文字母。
 */
QStringList MessageFilter::keywords() const
{
    return QStringList();
}

/*!
 * \brief 返回过滤器描述信息。
 */
QString MessageFilter::description() const
{
    return QString();
}

/*!
 * \brief 私有消息过滤器
 *
 * \arg i 返回命令后首个空格的位置，\a i 左侧为命令，右侧为参数。
 * \arg ev 消息事件本身。
 * \return 过滤返回 true；不过滤返回 false。
 */
bool MessageFilter::privateMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

/*!
 * \brief 群组消息过滤器
 *
 * \arg i 返回命令后首个空格的位置，\a i 左侧为命令，右侧为参数。
 * \arg ev 消息事件本身。
 * \return 过滤返回 true；不过滤返回 false。
 */
bool MessageFilter::groupMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

/*!
 * \brief 讨论组消息过滤器
 *
 * \arg i 返回命令后首个空格的位置，\a i 左侧为命令，右侧为参数。
 * \arg ev 消息事件本身。
 * \return 过滤返回 true；不过滤返回 false。
 */
bool MessageFilter::discussMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

// class MessageFilterPrivate

/*!
 * \internal
 */
MessageFilterPrivate::MessageFilterPrivate()
    : engine(nullptr)
    , module(nullptr)
{
}

/*!
 * \internal
 */
MessageFilterPrivate::~MessageFilterPrivate()
{
}

} // namespace CoolQ
