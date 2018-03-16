#ifndef MANAGEMODULE_H
#define MANAGEMODULE_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "CqServiceModule.h"

#include "sqldatas/masterlevels.h"
#include "htmlfeedback/htmlfeedback.h"

// class ManageModule

class ManageModulePrivate;
class ManageModule : public CoolQ::ServiceModule
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ManageModule)

public:
    explicit ManageModule(CoolQ::ServicePortal *engine = nullptr);
    virtual ~ManageModule();
public:
    static ManageModule *instance();

public:
    MasterLevel level(qint64 gid, qint64 uid) const;

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

private:
    void groupHelp(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupLevel(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupRename(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupFormat(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupMember(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupBan(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupKill(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupPower(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupUnban(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupUnkill(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupUnpower(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupWelcome(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupBlacklist(const CoolQ::MessageEvent &ev, const QStringList &args);

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
