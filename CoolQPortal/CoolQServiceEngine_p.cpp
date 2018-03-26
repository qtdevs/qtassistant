#include "CoolQServiceEngine.h"
#include "CoolQServiceEngine_p.h"

#include <process.h>
#include <QCoreApplication>

#include "CoolQApi/CoolQLib.h"
#include "CoolQServiceModule.h"
#include "CoolQServiceModule_p.h"

CQEVENT(const char *, AppInfo, 0)()
{
    return CQAPIVERTEXT "," TARGET;
}

void cqMsgHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg)
{
    qint32 priority = 0;

    if (msg.startsWith("QSslSocket")) {
        type = QtDebugMsg;
    }

    switch (type) {
    case QtDebugMsg:
        priority = CQLOG_DEBUG;
        break;
    case QtInfoMsg:
        priority = CQLOG_INFO;
        break;
    case QtWarningMsg:
        priority = CQLOG_WARNING;
        break;
    case QtCriticalMsg:
        priority = CQLOG_ERROR;
        break;
    case QtFatalMsg:
        priority = CQLOG_FATAL;
        break;
    }

    static char log[4096] = {0};
    QByteArray gbkMsg = CoolQ::trGbk(msg);
    sprintf(log, "%s (%s: %u)", gbkMsg.constData(), ctx.file, ctx.line);
    CQ_addLog(CoolQ::ServiceEnginePrivate::accessToken, priority, "Qt Logs", log);
    if (CQLOG_FATAL == priority)
        CQ_setFatal(CoolQ::ServiceEnginePrivate::accessToken, gbkMsg.constData());
}

CQEVENT(qint32, Initialize, 4)(qint32 ac)
{
    qInstallMessageHandler(cqMsgHandler);
    CoolQ::ServiceEnginePrivate::accessToken = ac;

    return 0;
}

int main(int argc, char *argv[]);
unsigned __stdcall cqMain(void *)
{
    char *argv[2] = { 0, 0 };
    return main(0, argv);
}

CQEVENT(qint32, __systemStartupEvent, 0)()
{
    if (0 == _beginthreadex(NULL, 0, cqMain, NULL, 0, NULL)) {
        qFatal("Create thread failed.");
        return 0;
    }

    return 0;
}

CQEVENT(qint32, __systemShutdownEvent, 0)()
{
    if (qApp)
        qApp->quit();

    return 0;
}

CQEVENT(qint32, __pluginEnableEvent, 0)()
{
    return 0;
}

CQEVENT(qint32, __pluginDisableEvent, 0)()
{
    return 0;
}

CQEVENT(qint32, __privateMessageEvent, 24)(qint32 type, qint32 time, qint64 from, const char *msg, qint32 font)
{
    if (auto engine = CoolQ::ServiceEngine::instance()) {
        CoolQ::MessageEvent event{ type, time, 0, font, from, msg };
        if (engine->privateMessageEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __groupMessageEvent, 36)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *, const char *msg, qint32 font)
{
    if (auto engine = CoolQ::ServiceEngine::instance()) {
        CoolQ::MessageEvent event{ type, time, from, font, sender, msg };
        if (engine->groupMessageEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __discussMessageEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *msg, qint32 font)
{
    if (auto engine = CoolQ::ServiceEngine::instance()) {
        CoolQ::MessageEvent event{ type, time, from, font, sender, msg };
        if (engine->discussMessageEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __masterChangeEvent, 24)(qint32 type, qint32 time, qint64 from, qint64 member)
{
    if (auto engine = CoolQ::ServiceEngine::instance()) {
        CoolQ::MasterChangeEvent event{ type, time, from, 0, member };
        if (engine->masterChangeEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __friendRequestEvent, 24)(qint32 type, qint32 time, qint64 from, const char *msg, const char *tag)
{
    if (auto engine = CoolQ::ServiceEngine::instance()) {
        CoolQ::FriendRequestEvent event{ type, time, from, msg, tag };
        if (engine->friendRequestEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __groupRequestEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 user, const char *msg, const char *tag)
{
    if (auto engine = CoolQ::ServiceEngine::instance()) {
        CoolQ::GroupRequestEvent event{ type, time, from, user, msg, tag };
        if (engine->groupRequestEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __friendAddEvent, 16)(qint32 type, qint32 time, qint64 from)
{
    if (auto engine = CoolQ::ServiceEngine::instance()) {
        CoolQ::FriendAddEvent event{ type, time, from };
        if (engine->friendAddEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __memberJoinEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    if (auto engine = CoolQ::ServiceEngine::instance()) {
        CoolQ::MemberJoinEvent event{ type, time, from, master, member };
        if (engine->memberJoinEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __memberLeaveEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    if (auto engine = CoolQ::ServiceEngine::instance()) {
        CoolQ::MemberLeaveEvent event{ type, time, from, master, member };
        if (engine->memberLeaveEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}
