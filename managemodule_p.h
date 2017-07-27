#ifndef MANAGEMODULE_P_H
#define MANAGEMODULE_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "cqmodule_p.h"
#include "managemodule.h"

class MasterLevels;
class MemberWelcome;
class MemberBlacklist;
class MemberDeathHouse;

class ManageModulePrivate : public CqModulePrivate
{
    Q_DECLARE_PUBLIC(ManageModule)

public:
    ManageModulePrivate();
    virtual ~ManageModulePrivate();
protected:
    ManageModule *q_ptr;

public:
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

#endif // MANAGEMODULE_P_H
