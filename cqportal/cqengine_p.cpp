#include "cqengine.h"
#include "cqengine_p.h"

#include <QCoreApplication>
#include <process.h>
#include "cqapi/cqapi.h"

#include "cqmodule.h"
#include "cqmodule_p.h"

CQEVENT(const char *, AppInfo, 0)()
{
    return CQAPIVERTEXT "," TARGET;
}

void cqHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg)
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
    QByteArray gbkMsg = CqModule::convert(msg);
    sprintf(log, "%s (%s: %u)", gbkMsg.constData(), ctx.file, ctx.line);
    CQ_addLog(CqEnginePrivate::accessToken, priority, "Cq Engine", log);
    if (CQLOG_FATAL == priority) {
        CQ_setFatal(CqEnginePrivate::accessToken, gbkMsg.constData());
    }
}

CQEVENT(qint32, Initialize, 4)(qint32 ac)
{
    qInstallMessageHandler(cqHandler);
    CqEnginePrivate::accessToken = ac;

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
    if (qApp) {
        qApp->quit();
    }

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
    if (CqEnginePrivate *enginePriv = CqEnginePrivate::self()) {
        MessageEvent event{ type, time, 0, font, from, msg };
        for (CqModule *module : enginePriv->privateMessageModules) {
            if (module->privateMessageEvent(event)) {
                return EVENT_BLOCK;
            }
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __groupMessageEvent, 36)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *, const char *msg, qint32 font)
{
    if (CqEnginePrivate *enginePriv = CqEnginePrivate::self()) {
        MessageEvent event{ type, time, from, font, sender, msg };
        for (CqModule *module : enginePriv->groupMessageModules) {
            if (module->groupMessageEvent(event)) {
                return EVENT_BLOCK;
            }
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __discussMessageEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *msg, qint32 font)
{
    if (CqEnginePrivate *enginePriv = CqEnginePrivate::self()) {
        MessageEvent event{ type, time, from, font, sender, msg };
        for (CqModule *module : enginePriv->discussMessageModules) {
            if (module->discussMessageEvent(event)) {
                return EVENT_BLOCK;
            }
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __masterChangeEvent, 24)(qint32 type, qint32 time, qint64 from, qint64 member)
{
    if (CqEnginePrivate *enginePriv = CqEnginePrivate::self()) {
        MasterChangeEvent event{ type, time, from, member };
        for (CqModule *module : enginePriv->masterChangeModules) {
            if (module->masterChangeEvent(event)) {
                return EVENT_BLOCK;
            }
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __friendRequestEvent, 24)(qint32 type, qint32 time, qint64 from, const char *msg, const char *tag)
{
    if (CqEnginePrivate *enginePriv = CqEnginePrivate::self()) {
        FriendRequestEvent event{ type, time, from, msg, tag };
        for (CqModule *module : enginePriv->friendRequestModules) {
            if (module->friendRequestEvent(event)) {
                return EVENT_BLOCK;
            }
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __groupRequestEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 user, const char *msg, const char *tag)
{
    if (CqEnginePrivate *enginePriv = CqEnginePrivate::self()) {
        GroupRequestEvent event{ type, time, from, user, msg, tag };
        for (CqModule *module : enginePriv->groupRequestModules) {
            if (module->groupRequestEvent(event)) {
                return EVENT_BLOCK;
            }
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __friendAddEvent, 16)(qint32 type, qint32 time, qint64 from)
{
    if (CqEnginePrivate *enginePriv = CqEnginePrivate::self()) {
        FriendAddEvent event{ type, time, from };
        for (CqModule *module : enginePriv->friendAddModules) {
            if (module->friendAddEvent(event)) {
                return EVENT_BLOCK;
            }
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __memberJoinEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    if (CqEnginePrivate *enginePriv = CqEnginePrivate::self()) {
        MemberJoinEvent event{ type, time, from, master, member };
        for (CqModule *module : enginePriv->memberJoinModules) {
            if (module->memberJoinEvent(event)) {
                return EVENT_BLOCK;
            }
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __memberLeaveEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    if (CqEnginePrivate *enginePriv = CqEnginePrivate::self()) {
        MemberLeaveEvent event{ type, time, from, master, member };
        for (CqModule *module : enginePriv->memberLeaveModules) {
            if (module->memberLeaveEvent(event)) {
                return EVENT_BLOCK;
            }
        }
    }

    return EVENT_IGNORE;
}
