#ifndef ASSISTANTMODULE_H
#define ASSISTANTMODULE_H

#include "CoolQServiceModule.h"

#include "SqlDatas/MasterLevels.h"
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

public:
    void groupHelpAction(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupLevel(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupRename(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupFormat(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupMember(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupBanAction(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupKickAction(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupRaiseAction(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupUnbanAction(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupLowerAction(const CoolQ::MessageEvent &ev, const QStringList &args);

    void groupWatchlistAction(const CoolQ::MessageEvent &ev, const QStringList &args);
    void groupBlacklistAction(const CoolQ::MessageEvent &ev, const QStringList &args);

public:
    void groupLevelHelp(qint64 gid);
    void groupRenameHelp(qint64 gid);
    void groupFormatHelp(qint64 gid);
    void groupMemberHelp(qint64 gid);

    void groupBanHelpAction(qint64 gid);
    void groupKickHelpAction(qint64 gid);
    void groupRaiseHelpAction(qint64 gid);

    void groupUnbanHelpAction(qint64 gid);
    void groupLowerHelpAction(qint64 gid);

    void groupWatchlistHelpAction(qint64 gid);
    void groupBlacklistHelpAction(qint64 gid);

public:
    void permissionDenied(qint64 gid, qint64 uid, MasterLevel level, const QString &reason = QString());

public:
    void showPrimary(qint64 gid, const QString &title, const QString &content);
    void showDanger(qint64 gid, const QString &title, const QString &content);
    void showWarning(qint64 gid, const QString &title, const QString &content);
    void showPrompt(qint64 gid, const QString &title, const QString &content);
    void showSuccess(qint64 gid, const QString &title, const QString &content);

    void feedback(qint64 gid, const QString &title, const QString &content, HtmlDraw::Style style);

public:
    void showPrimaryList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showDangerList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showWarningList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showPromptList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showSuccessList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);

    void feedbackList(qint64 gid, const QString &title, const LevelInfoList &members, bool level, HtmlDraw::Style style);

public:
    void showWelcomes(qint64 gid, qint64 uid);
    void saveWelcomes(qint64 gid, qint64 uid);
};

#endif // ASSISTANTMODULE_H
