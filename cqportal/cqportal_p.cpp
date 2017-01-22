#include "cqportal.h"
#include "cqportal_p.h"

#include <QCoreApplication>
#include <process.h>
#include "cqapi/cqapi.h"

CQEVENT(const char *, AppInfo, 0)()
{
    return CQAPIVERTEXT "," TARGET;
}

void qtHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg)
{
    qint32 priority = 0;

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
    QByteArray gbkMsg = CqPortal::convert(msg);
    sprintf(log, "%s (%s: %u)", gbkMsg.constData(), ctx.file, ctx.line);
    CQ_addLog(CqPortalPrivate::accessToken, priority, "Qt Portal", log);
    if (CQLOG_FATAL == priority) {
        CQ_setFatal(CqPortalPrivate::accessToken, gbkMsg.constData());
    }
}

CQEVENT(qint32, Initialize, 4)(qint32 ac)
{
    qInstallMessageHandler(qtHandler);
    CqPortalPrivate::accessToken = ac;

    return 0;
}

int main(int argc, char *argv[]);
unsigned __stdcall qtMain(void *)
{
    char *argv[2];
    return main(0, argv);
}

CQEVENT(qint32, __systemStartupEvent, 0)()
{
    if (0 == _beginthreadex(NULL, 0, qtMain, NULL, 0, NULL)) {
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
    if (CqPortalPrivate::instance) {
        MessageEvent ev{ type, time, from, font, from, msg };
        if (CqPortalPrivate::instance->privateMessageEventFilter(ev)) {
            return EVENT_BLOCK;
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __groupMessageEvent, 36)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *, const char *msg, qint32 font)
{
    if (CqPortalPrivate::instance) {
        MessageEvent ev{ type, time, from, font, sender, msg };
        if (CqPortalPrivate::instance->groupMessageEventFilter(ev)) {
            return EVENT_BLOCK;
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __discussMessageEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *msg, qint32 font)
{
    if (CqPortalPrivate::instance) {
        MessageEvent ev{ type, time, from, font, sender, msg };
        if (CqPortalPrivate::instance->discussMessageEventFilter(ev)) {
            return EVENT_BLOCK;
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __masterChangeEvent, 24)(qint32 type, qint32 time, qint64 from, qint64 member)
{
    if (CqPortalPrivate::instance) {
        MasterChangeEvent ev{ type, time, from, member };
        if (CqPortalPrivate::instance->masterChangeEventFilter(ev)) {
            return EVENT_BLOCK;
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __friendRequestEvent, 24)(qint32 type, qint32 time, qint64 from, const char *msg, const char *tag)
{
    if (CqPortalPrivate::instance) {
        FriendRequestEvent ev{ type, time, from, msg, tag };
        if (CqPortalPrivate::instance->friendRequestEventFilter(ev)) {
            return EVENT_BLOCK;
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __groupRequestEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 user, const char *msg, const char *tag)
{
    if (CqPortalPrivate::instance) {
        GroupRequestEvent ev{ type, time, from, user, msg, tag };
        if (CqPortalPrivate::instance->groupRequestEventFilter(ev)) {
            return EVENT_BLOCK;
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __friendAddEvent, 16)(qint32 type, qint32 time, qint64 from)
{
    if (CqPortalPrivate::instance) {
        FriendAddEvent ev{ type, time, from };
        if (CqPortalPrivate::instance->friendAddEventFilter(ev)) {
            return EVENT_BLOCK;
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __memberJoinEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    if (CqPortalPrivate::instance) {
        MemberJoinEvent ev{ type, time, from, master, member };
        if (CqPortalPrivate::instance->memberJoinEventFilter(ev)) {
            return EVENT_BLOCK;
        }
    }

    return EVENT_IGNORE;
}

CQEVENT(qint32, __memberLeaveEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    if (CqPortalPrivate::instance) {
        MemberLeaveEvent ev{ type, time, from, master, member };
        if (CqPortalPrivate::instance->memberLeaveEventFilter(ev)) {
            return EVENT_BLOCK;
        }
    }

    return EVENT_IGNORE;
}
