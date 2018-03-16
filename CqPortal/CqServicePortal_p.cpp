#include "CqServicePortal.h"
#include "CqServicePortal_p.h"

#include <QCoreApplication>
#include <process.h>
#include "CqApi/CqLib.h"

#include "CqServiceModule.h"
#include "CqServiceModule_p.h"

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
    QByteArray gbkMsg = CoolQ::trGbk(msg);
    sprintf(log, "%s (%s: %u)", gbkMsg.constData(), ctx.file, ctx.line);
    CQ_addLog(CoolQ::ServicePortalPrivate::accessToken, priority, "Cq Portal", log);
    if (CQLOG_FATAL == priority) {
        CQ_setFatal(CoolQ::ServicePortalPrivate::accessToken, gbkMsg.constData());
    }
}

CQEVENT(qint32, Initialize, 4)(qint32 ac)
{
    qInstallMessageHandler(cqHandler);
    CoolQ::ServicePortalPrivate::accessToken = ac;

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
    if (auto portal = CoolQ::ServicePortal::instance()) {
        CoolQ::MessageEvent event{ type, time, 0, font, from, msg };
        if (portal->privateMessageEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __groupMessageEvent, 36)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *, const char *msg, qint32 font)
{
    if (auto portal = CoolQ::ServicePortal::instance()) {
        CoolQ::MessageEvent event{ type, time, from, font, sender, msg };
        if (portal->groupMessageEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __discussMessageEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *msg, qint32 font)
{
    if (auto portal = CoolQ::ServicePortal::instance()) {
        CoolQ::MessageEvent event{ type, time, from, font, sender, msg };
        if (portal->discussMessageEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __masterChangeEvent, 24)(qint32 type, qint32 time, qint64 from, qint64 member)
{
    if (auto portal = CoolQ::ServicePortal::instance()) {
        CoolQ::MasterChangeEvent event{ type, time, from, 0, member };
        if (portal->masterChangeEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __friendRequestEvent, 24)(qint32 type, qint32 time, qint64 from, const char *msg, const char *tag)
{
    if (auto portal = CoolQ::ServicePortal::instance()) {
        CoolQ::FriendRequestEvent event{ type, time, from, msg, tag };
        if (portal->friendRequestEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __groupRequestEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 user, const char *msg, const char *tag)
{
    if (auto portal = CoolQ::ServicePortal::instance()) {
        CoolQ::GroupRequestEvent event{ type, time, from, user, msg, tag };
        if (portal->groupRequestEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __friendAddEvent, 16)(qint32 type, qint32 time, qint64 from)
{
    if (auto portal = CoolQ::ServicePortal::instance()) {
        CoolQ::FriendAddEvent event{ type, time, from };
        if (portal->friendAddEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __memberJoinEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    if (auto portal = CoolQ::ServicePortal::instance()) {
        CoolQ::MemberJoinEvent event{ type, time, from, master, member };
        if (portal->memberJoinEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __memberLeaveEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    if (auto portal = CoolQ::ServicePortal::instance()) {
        CoolQ::MemberLeaveEvent event{ type, time, from, master, member };
        if (portal->memberLeaveEvent(event))
            return EVENT_BLOCK;
    }

    return EVENT_IGNORE;
}
