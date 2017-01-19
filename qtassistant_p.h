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

class QtAssistantPrivate : public CqPortalPrivate
{
    Q_DECLARE_PUBLIC(QtAssistant)

public:
    QtAssistantPrivate();
    virtual ~QtAssistantPrivate();

private:
    static LevelInfoList findUsers(const QStringList &args);

    static void safetyNameCard(QString &nameCard);
    static void formatNameCard(QString &nameCard);

private:
    MasterLevels *levels;
    MemberWelcome *welcome;
    MemberBlacklist *blacklist;
    MemberDeathHouse *deathHouse;

    HtmlFeedback *htmlFeedback;

    int checkTimerId;
};

#endif // QtAssistant_P_H
