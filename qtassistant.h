#ifndef QTASSISTANT_H
#define QTASSISTANT_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "cqportal.h"

// class QtAssistant

class QtAssistantPrivate;
class QtAssistant : public CqPortal
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QtAssistant)

public:
    explicit QtAssistant(QObject *parent = Q_NULLPTR);
    virtual ~QtAssistant();

public:
    bool privateMessageEventFilter(const MessageEvent &ev) Q_DECL_FINAL;
    bool groupMessageEventFilter(const MessageEvent &ev) Q_DECL_FINAL;
    bool discussMessageEventFilter(const MessageEvent &ev) Q_DECL_FINAL;

    bool masterChangeEventFilter(const MasterChangeEvent &ev) Q_DECL_FINAL;
    bool friendRequestEventFilter(const FriendRequestEvent &ev) Q_DECL_FINAL;
    bool groupRequestEventFilter(const GroupRequestEvent &ev) Q_DECL_FINAL;

    bool friendAddEventFilter(const FriendAddEvent &ev) Q_DECL_FINAL;
    bool memberJoinEventFilter(const MemberJoinEvent &ev) Q_DECL_FINAL;
    bool memberLeaveEventFilter(const MemberLeaveEvent &ev) Q_DECL_FINAL;

private:
    void timerEvent(QTimerEvent *) Q_DECL_FINAL;

private:
    void groupHelp(const MessageEvent &ev, const QStringList &args);
    void groupLevel(const MessageEvent &ev, const QStringList &args);
    void groupRename(const MessageEvent &ev, const QStringList &args);

    void groupBan(const MessageEvent &ev, const QStringList &args);
    void groupKill(const MessageEvent &ev, const QStringList &args);
    void groupPower(const MessageEvent &ev, const QStringList &args);

    void groupUnban(const MessageEvent &ev, const QStringList &args);
    void groupUnkill(const MessageEvent &ev, const QStringList &args);
    void groupUnpower(const MessageEvent &ev, const QStringList &args);

    void groupWelcome(const MessageEvent &ev, const QStringList &args);
    void groupBlacklist(const MessageEvent &ev, const QStringList &args);

private:
    void groupHelpHelp(qint64 gid);
    void groupLevelHelp(qint64 gid);
    void groupRenameHelp(qint64 gid);

    void groupBanHelp(qint64 gid);
    void groupKillHelp(qint64 gid);
    void groupPowerHelp(qint64 gid);

    void groupUnbanHelp(qint64 gid);
    void groupUnkillHelp(qint64 gid);
    void groupUnpowerHelp(qint64 gid);

    void groupWelcomeHelp(qint64 gid);
    void groupBlacklistHelp(qint64 gid);
};

#endif // QTASSISTANT_H
