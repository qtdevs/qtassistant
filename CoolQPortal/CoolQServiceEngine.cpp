/*!
 * \class CoolQ::ServiceEngine
 * \brief 服务引擎类
 */

#include "CoolQServiceEngine.h"
#include "CoolQServiceEngine_p.h"

#include "CoolQServiceModule.h"
#include "CoolQServiceModule_p.h"

namespace CoolQ {

// class ServiceEngine

/*!
 * \brief 构造函数
 *
 * 构造一个 ServiceEngine，同时只允许一个 Engine 的存在。
 */
ServiceEngine::ServiceEngine(QObject *parent)
    : Interface(*new ServiceEnginePrivate(), parent)
{
    Q_ASSERT(nullptr == ServiceEnginePrivate::instance);
    ServiceEnginePrivate::instance = this;
}

/*!
 * \brief 析构函数
 */
ServiceEngine::~ServiceEngine()
{
    ServiceEnginePrivate::instance = nullptr;
}

/*!
 * \brief 返回单例函数
 *
 * 返回当前引擎的单例，这个单例必须是手动创建的，此方法不会主动创建任何对象。
 */
ServiceEngine *ServiceEngine::instance()
{
    return ServiceEnginePrivate::instance;
}

/*!
 * \brief 标准初始化函数
 */
bool ServiceEngine::initialize()
{
    Q_D(ServiceEngine);

    for(ServiceModule *module : d->modules) {
        if (!module->initialize())
            return false;
    }

    for(ServiceModule *module : d->modules) {
        if (module->privateMessageEventPriority() > 0)
            d->privateMessageModules.append(module);
        if (module->groupMessageEventPriority() > 0)
            d->groupMessageModules.append(module);
        if (module->discussMessageEventPriority() > 0)
            d->discussMessageModules.append(module);

        if (module->masterChangeEventPriority() > 0)
            d->masterChangeModules.append(module);
        if (module->friendRequestEventPriority() > 0)
            d->friendRequestModules.append(module);
        if (module->groupRequestEventPriority() > 0)
            d->groupRequestModules.append(module);

        if (module->friendAddEventPriority() > 0)
            d->friendAddModules.append(module);
        if (module->memberJoinEventPriority() > 0)
            d->memberJoinModules.append(module);
        if (module->memberLeaveEventPriority() > 0)
            d->memberLeaveModules.append(module);
    }

    return true;
}

/*!
 * \brief 私有消息过滤器
 *
 * 过滤私有消息。如果返回 true，就代表过滤；返回 false，代表不过滤。
 * 此方法默认有调用模块的实现，如果继承，需要调用基类的此方法。
 */
bool ServiceEngine::privateMessageEvent(const MessageEvent &ev)
{
    Q_D(ServiceEngine);

    for (ServiceModule *module : d->privateMessageModules)
        if (module->privateMessageEvent(ev))
            return true;

    return false;
}

/*!
 * \brief 群组消息过滤器
 *
 * 过滤群组消息。如果返回 true，就代表过滤；返回 false，代表不过滤。
 * 此方法默认有调用模块的实现，如果继承，需要调用基类的此方法。
 */
bool ServiceEngine::groupMessageEvent(const MessageEvent &ev)
{
    Q_D(ServiceEngine);

    for (ServiceModule *module : d->groupMessageModules)
        if (module->groupMessageEvent(ev))
            return true;

    return false;
}

/*!
 * \brief 讨论组消息过滤器
 *
 * 过滤讨论组消息。如果返回 true，就代表过滤；返回 false，代表不过滤。
 * 此方法默认有调用模块的实现，如果继承，需要调用基类的此方法。
 */
bool ServiceEngine::discussMessageEvent(const MessageEvent &ev)
{
    Q_D(ServiceEngine);

    for (ServiceModule *module : d->discussMessageModules)
        if (module->discussMessageEvent(ev))
            return true;

    return false;
}

/*!
 * \brief 管理员变更事件过滤器
 *
 * 过滤管理员变更的事件。如果返回 true，就代表过滤；返回 false，代表不过滤。
 * 此方法默认有调用模块的实现，如果继承，需要调用基类的此方法。
 */
bool ServiceEngine::masterChangeEvent(const MasterChangeEvent &ev)
{
    Q_D(ServiceEngine);

    for (ServiceModule *module : d->masterChangeModules)
        if (module->masterChangeEvent(ev))
            return true;

    return false;
}

/*!
 * \brief 好友请求事件过滤器
 *
 * 过滤好友请求的事件。如果返回 true，就代表过滤；返回 false，代表不过滤。
 * 此方法默认有调用模块的实现，如果继承，需要调用基类的此方法。
 */
bool ServiceEngine::friendRequestEvent(const FriendRequestEvent &ev)
{
    Q_D(ServiceEngine);

    for (ServiceModule *module : d->friendRequestModules)
        if (module->friendRequestEvent(ev))
            return true;

    return false;
}

/*!
 * \brief 加入群组事件过滤器
 *
 * 过滤加入群组的事件。如果返回 true，就代表过滤；返回 false，代表不过滤。
 * 此方法默认有调用模块的实现，如果继承，需要调用基类的此方法。
 */
bool ServiceEngine::groupRequestEvent(const GroupRequestEvent &ev)
{
    Q_D(ServiceEngine);

    for (ServiceModule *module : d->groupRequestModules)
        if (module->groupRequestEvent(ev))
            return true;

    return false;
}

/*!
 * \brief 好友添加事件过滤器
 *
 * 过滤好友添加的事件。如果返回 true，就代表过滤；返回 false，代表不过滤。
 * 此方法默认有调用模块的实现，如果继承，需要调用基类的此方法。
 */
bool ServiceEngine::friendAddEvent(const FriendAddEvent &ev)
{
    Q_D(ServiceEngine);

    for (ServiceModule *module : d->friendAddModules)
        if (module->friendAddEvent(ev))
            return true;

    return false;
}

/*!
 * \brief 成员添加事件过滤器
 *
 * 过滤成员添加的事件。如果返回 true，就代表过滤；返回 false，代表不过滤。
 * 此方法默认有调用模块的实现，如果继承，需要调用基类的此方法。
 */
bool ServiceEngine::memberJoinEvent(const MemberJoinEvent &ev)
{
    Q_D(ServiceEngine);

    for (ServiceModule *module : d->memberJoinModules)
        if (module->memberJoinEvent(ev))
            return true;

    return false;
}

/*!
 * \brief 成员退出事件过滤器
 *
 * 过滤成员退出的事件。如果返回 true，就代表过滤；返回 false，代表不过滤。
 * 此方法默认有调用模块的实现，如果继承，需要调用基类的此方法。
 */
bool ServiceEngine::memberLeaveEvent(const MemberLeaveEvent &ev)
{
    Q_D(ServiceEngine);

    for (ServiceModule *module : d->memberLeaveModules)
        if (module->memberLeaveEvent(ev))
            return true;

    return false;
}

// class ServiceEnginePrivate

ServiceEngine *ServiceEnginePrivate::instance = nullptr;

qint32 ServiceEnginePrivate::accessToken = 0;

/*!
 * \internal
 */
ServiceEnginePrivate::ServiceEnginePrivate()
{
}

/*!
 * \internal
 */
ServiceEnginePrivate::~ServiceEnginePrivate()
{
}

/*!
 * \internal
 */
void ServiceEnginePrivate::installModule(ServiceModule *module)
{
    Q_CHECK_PTR(module);

    modules.append(module);
}

} // namespace CoolQ
