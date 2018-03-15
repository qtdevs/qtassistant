#ifndef MANAGEMODULE_H
#define MANAGEMODULE_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "cqmodule.h"

#include "sqldatas/masterlevels.h"
#include "htmlfeedback/htmlfeedback.h"

// class ManageModule

class ManageModulePrivate;
class ManageModule : public CqModule
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ManageModule)

public:
    explicit ManageModule(CqEngine *engine = nullptr);
    virtual ~ManageModule();

public:
    bool privateMessageEvent(const MessageEvent &ev) Q_DECL_FINAL;
    bool groupMessageEvent(const MessageEvent &ev) Q_DECL_FINAL;
    bool discussMessageEvent(const MessageEvent &ev) Q_DECL_FINAL;

    bool masterChangeEvent(const MasterChangeEvent &ev) Q_DECL_FINAL;
    bool friendRequestEvent(const FriendRequestEvent &ev) Q_DECL_FINAL;
    bool groupRequestEvent(const GroupRequestEvent &ev) Q_DECL_FINAL;

    bool friendAddEvent(const FriendAddEvent &ev) Q_DECL_FINAL;
    bool memberJoinEvent(const MemberJoinEvent &ev) Q_DECL_FINAL;
    bool memberLeaveEvent(const MemberLeaveEvent &ev) Q_DECL_FINAL;

private:
    void timerEvent(QTimerEvent *) Q_DECL_FINAL;

private:
    void groupHelp(const MessageEvent &ev, const QStringList &args);
    void groupLevel(const MessageEvent &ev, const QStringList &args);
    void groupRename(const MessageEvent &ev, const QStringList &args);
    void groupFormat(const MessageEvent &ev, const QStringList &args);
    void groupMember(const MessageEvent &ev, const QStringList &args);

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
    void groupFormatHelp(qint64 gid);
    void groupMemberHelp(qint64 gid);

    void groupBanHelp(qint64 gid);
    void groupKillHelp(qint64 gid);
    void groupPowerHelp(qint64 gid);

    void groupUnbanHelp(qint64 gid);
    void groupUnkillHelp(qint64 gid);
    void groupUnpowerHelp(qint64 gid);

    void groupWelcomeHelp(qint64 gid);
    void groupBlacklistHelp(qint64 gid);

private:
    void permissionDenied(qint64 gid, qint64 uid, MasterLevel level, const QString &reason = QString());

private:
    void showPrimary(qint64 gid, const QString &title, const QString &content);
    void showDanger(qint64 gid, const QString &title, const QString &content);
    void showWarning(qint64 gid, const QString &title, const QString &content);
    void showPrompt(qint64 gid, const QString &title, const QString &content);
    void showSuccess(qint64 gid, const QString &title, const QString &content);

    void feedback(qint64 gid, const QString &title, const QString &content, HtmlFeedback::Style style);

private:
    void showPrimaryList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showDangerList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showWarningList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showPromptList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showSuccessList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);

    void feedbackList(qint64 gid, const QString &title, const LevelInfoList &members, bool level, HtmlFeedback::Style style);

private:
    void showWelcomes(qint64 gid, qint64 uid);
    void saveWelcomes(qint64 gid, qint64 uid);
};

#endif // MANAGEMODULE_H
