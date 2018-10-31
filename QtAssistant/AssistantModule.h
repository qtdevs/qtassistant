#ifndef ASSISTANTMODULE_H
#define ASSISTANTMODULE_H

#include "CoolQServiceModule.h"

#include "HtmlDraw/HtmlDraw.h"

// class AssistantModule

class AssistantModulePrivate;
class AssistantModule : public CoolQ::ServiceModule
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AssistantModule)

public:
    explicit AssistantModule(CoolQ::ServiceEngine *engine = nullptr);
    virtual ~AssistantModule();
public:
    static AssistantModule *instance();

public:
    bool privateMessageEvent(const CoolQ::MessageEvent &ev) Q_DECL_FINAL;
    bool groupMessageEvent(const CoolQ::MessageEvent &ev) Q_DECL_FINAL;
    bool discussMessageEvent(const CoolQ::MessageEvent &ev) Q_DECL_FINAL;

    bool masterChangeEvent(const CoolQ::MasterChangeEvent &ev) Q_DECL_FINAL;
    bool friendRequestEvent(const CoolQ::FriendRequestEvent &ev) Q_DECL_FINAL;
    bool groupRequestEvent(const CoolQ::GroupRequestEvent &ev) Q_DECL_FINAL;

    bool friendAddEvent(const CoolQ::FriendAddEvent &ev) Q_DECL_FINAL;
    bool memberJoinEvent(const CoolQ::MemberJoinEvent &ev) Q_DECL_FINAL;
    bool memberLeaveEvent(const CoolQ::MemberLeaveEvent &ev) Q_DECL_FINAL;

private:
    void timerEvent(QTimerEvent *) Q_DECL_FINAL;

public:
    void groupHelpAction(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupRenameAction(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupFormatAction(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupBanAction(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupKickAction(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupUnbanAction(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupWatchlistAction(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupAddWatchlistAction(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupRemoveWatchlistAction(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupBlacklistAction(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupAddBlacklistAction(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupRemoveBlacklistAction(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupMemberAction(const CoolQ::MessageEvent &ev, const QStringList &args);

public:
    void groupRenameHelpAction(qint64 gid);
    void groupFormatHelpAction(qint64 gid);

    void groupBanHelpAction(qint64 gid);
    void groupKickHelpAction(qint64 gid);
    void groupUnbanHelpAction(qint64 gid);

    void groupWatchlistHelpAction(qint64 gid);
    void groupBlacklistHelpAction(qint64 gid);

    void groupMemberHelpAction(qint64 gid);

public:
    void showPrimary(qint64 gid, const QString &title, const QString &content);
    void showDanger(qint64 gid, const QString &title, const QString &content);
    void showWarning(qint64 gid, const QString &title, const QString &content);
    void showPrompt(qint64 gid, const QString &title, const QString &content);
    void showSuccess(qint64 gid, const QString &title, const QString &content);

    void feedback(qint64 gid, const QString &title, const QString &content, HtmlDraw::Style style);

public:
    void showPrimaryList(qint64 gid, const QString &title, const QList<qint64> &members);
    void showDangerList(qint64 gid, const QString &title, const QList<qint64> &members);
    void showWarningList(qint64 gid, const QString &title, const QList<qint64> &members);
    void showPromptList(qint64 gid, const QString &title, const QList<qint64> &members);
    void showSuccessList(qint64 gid, const QString &title, const QList<qint64> &members);

    void feedbackList(qint64 gid, const QString &title, const QList<qint64> &members, HtmlDraw::Style style);

public:
    void showWelcomes(qint64 gid, qint64 uid);
    void saveWelcomes(qint64 gid, qint64 uid);

public:
    bool isSuperUser(qint64 uid) const;
};

#endif // ASSISTANTMODULE_H
