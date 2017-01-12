#include "cqassistant.h"
#include "cqassistant_p.h"

#include <QPointer>
#include <QGuiApplication>
#include <QTranslator>

#include "cqapi/cqapi.h"

qint32 token = -1;
QPointer<CqAssistant> theAssistant;
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

    QByteArray gbkMsg = CqAssistant::conv(msg);

    static char log[4096] = {0};
    sprintf(log, "%s (%s:%u)", gbkMsg.constData(), ctx.file, ctx.line);
    CQ_addLog(token, priority, "Qt Assistant", log);
}

CQEVENT(const char *, AppInfo, 0)()
{
    return APPINFO;
}

CQEVENT(qint32, Initialize, 4)(qint32 authCode)
{
    qInstallMessageHandler(cqHandler);
    token = authCode;
    return 0;
}

CQEVENT(qint32, __systemStartupEvent, 0)()
{
    int argc = 0;
    QGuiApplication *app = new QGuiApplication(argc, (char **)Q_NULLPTR);
    theAssistant = new CqAssistant(token, app);

    QTranslator *translator = new QTranslator(app);
    if (translator->load("cqassistant_zh.qm", ":/translations")) {
        qDebug("Installing Translator...");
        app->installTranslator(translator);
    } else {
        qDebug("Failed Translator...");
    }

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
    return theAssistant->privateMessageEventFilter(MessageEvent{ type, time, from, font, from, msg }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __groupMessageEvent, 36)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *, const char *msg, qint32 font)
{
    return theAssistant->groupMessageEventFilter(MessageEvent{ type, time, from, font, sender, msg }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __discussMessageEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 sender, const char *msg, qint32 font)
{
    return theAssistant->discussMessageEventFilter(MessageEvent{ type, time, from, font, sender, msg }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __masterChangeEvent, 24)(qint32 type, qint32 time, qint64 from, qint64 member)
{
    return theAssistant->masterChangeEventFilter(MasterChangeEvent{ type, time, from, member }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __friendRequestEvent, 24)(qint32 type, qint32 time, qint64 from, const char *msg, const char *tag)
{
    return theAssistant->friendRequestEventFilter(FriendRequestEvent{ type, time, from, msg, tag }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __groupRequestEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 user, const char *msg, const char *tag)
{
    return theAssistant->groupRequestEventFilter(GroupRequestEvent{ type, time, from, user, msg, tag }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __friendAddEvent, 16)(qint32 type, qint32 time, qint64 from)
{
    return theAssistant->friendAddEventFilter(FriendAddEvent{ type, time, from }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __memberJoinEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    return theAssistant->memberJoinEventFilter(MemberJoinEvent{ type, time, from, master, member }) ? EVENT_BLOCK : EVENT_IGNORE;
}

CQEVENT(qint32, __memberLeaveEvent, 32)(qint32 type, qint32 time, qint64 from, qint64 master, qint64 member)
{
    return theAssistant->memberLeaveEventFilter(MemberLeaveEvent{ type, time, from, master, member }) ? EVENT_BLOCK : EVENT_IGNORE;
}
