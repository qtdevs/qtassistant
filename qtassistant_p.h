#ifndef QtAssistant_P_H
#define QtAssistant_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "cqportal_p.h"
#include "qtassistant.h"

class MasterLevels;
class MemberWelcome;
class MemberBlacklist;
class MemberDeathHouse;

#include "sqldatas/masterlevels.h"
#include "htmlfeedback/htmlfeedback.h"

class QtAssistantPrivate : public CqPortalPrivate
{
    Q_DECLARE_PUBLIC(QtAssistant)

public:
    QtAssistantPrivate();
    virtual ~QtAssistantPrivate();

private:
    static LevelInfoList findUsers(const QStringList &args);

private:
    MasterLevels *levels;
    MemberWelcome *welcome;
    MemberBlacklist *blacklist;
    MemberDeathHouse *deathHouse;

    int checkTimerId;

private:
    void permissionDenied(qint64 gid, qint64 uid, MasterLevel level, const QString &reason = QString());

    //void showPrimaryList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    //void showDangerList(qint64 gid, const QString &title, const LevelInfoList &members, bool level);
    void showWarning(qint64 gid, const QString &title, const QString &content);
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

#endif // QtAssistant_P_H
