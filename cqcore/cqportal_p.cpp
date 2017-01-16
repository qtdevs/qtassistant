#include "cqportal.h"
#include "cqportal_p.h"

#include <QPointer>
#include <QGuiApplication>
#include <QTranslator>

#include "cqapi/cqapi.h"

QPointer<CqPortal> thePortal;
#define APPINFO CQAPIVERTEXT ",org.qtdevs.assistant"

void cqHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg)
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

    QByteArray gbkMsg = CqPortal::convert(msg);

    static char log[4096] = {0};
    sprintf(log, "%s (%s:%u)", gbkMsg.constData(), ctx.file, ctx.line);
    CQ_addLog(CqPortalPrivate::accessToken, priority, "Qt Assistant", log);
}

CQEVENT(const char *, AppInfo, 0)()
{
    return APPINFO;
}

CQEVENT(qint32, Initialize, 4)(qint32 ac)
{
    qInstallMessageHandler(cqHandler);
    CqPortalPrivate::accessToken = ac;

    return 0;
}

CQEVENT(qint32, __systemStartupEvent, 0)()
{
    int argc = 0;
    QGuiApplication *app = new QGuiApplication(argc, (char **)Q_NULLPTR);
    thePortal = _q_CreateInstance(app);

    return 0;
}

CQEVENT(qint32, __systemShutdownEvent, 0)()
{
    delete QGuiApplication::instance();

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
    return thePortal->privateMessageEventFilter(MessageEvent{ type, time, from, font, from, msg }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __groupMessageEvent, 36)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *, const char *msg, qint32 font)
{
    return thePortal->groupMessageEventFilter(MessageEvent{ type, time, from, font, sender, msg }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __discussMessageEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *msg, qint32 font)
{
    return thePortal->discussMessageEventFilter(MessageEvent{ type, time, from, font, sender, msg }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __masterChangeEvent, 24)(qint32 type, qint32 time, qint64 from, qint64 member)
{
    return thePortal->masterChangeEventFilter(MasterChangeEvent{ type, time, from, member }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __friendRequestEvent, 24)(qint32 type, qint32 time, qint64 from, const char *msg, const char *tag)
{
    return thePortal->friendRequestEventFilter(FriendRequestEvent{ type, time, from, msg, tag }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __groupRequestEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 user, const char *msg, const char *tag)
{
    return thePortal->groupRequestEventFilter(GroupRequestEvent{ type, time, from, user, msg, tag }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __friendAddEvent, 16)(qint32 type, qint32 time, qint64 from)
{
    return thePortal->friendAddEventFilter(FriendAddEvent{ type, time, from }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __memberJoinEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    return thePortal->memberJoinEventFilter(MemberJoinEvent{ type, time, from, master, member }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __memberLeaveEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    return thePortal->memberLeaveEventFilter(MemberLeaveEvent{ type, time, from, master, member }) ? EVENT_BLOCK : EVENT_IGNORE;
}
