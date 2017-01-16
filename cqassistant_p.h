#ifndef CQASSISTANT_P_H
#define CQASSISTANT_P_H

#include "cqportal_p.h"
#include "cqassistant.h"

class MasterLevels;
class MemberWelcome;
class MemberBlacklist;
class MemberDeathHouse;

#include "datas/masterlevels.h"
#include "htmlfeedback/htmlfeedback.h"

class CqAssistantPrivate : public CqPortalPrivate
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(CqAssistant)

public:
    CqAssistantPrivate();
    virtual ~CqAssistantPrivate();

private:
    MasterLevels *levels;
    MemberWelcome *welcome;
    MemberBlacklist *blacklist;
    MemberDeathHouse *deathHouse;

private:
    void timerEvent(QTimerEvent *) Q_DECL_FINAL;
private:
    int timerId;

private:
    LevelInfoList findUsers(const QStringList &args) const;

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

private:
    void permissionDenied(qint64 gid, qint64 uid, MasterLevel level, const QString &reason = QString());

    //void showPrimaryList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    //void showDangerList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    //void showWarningList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showPrompt(qint64 gid, const QString &title, const QString &content);
    void showSuccess(qint64 gid, const QString &title, const QString &content);

    void showPrimaryList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showDangerList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showWarningList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showPromptList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showSuccessList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);

    void feedbackList(qint64 gid, const QString &title, const LevelInfoList &members, bool level, HtmlFeedback::Style style);

private:
    HtmlFeedback *htmlFeedback;
};

#endif // CQASSISTANT_P_H
