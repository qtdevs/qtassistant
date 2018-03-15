#ifndef MANAGEMODULE_P_H
#define MANAGEMODULE_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include <QSet>

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

public:
    static LevelInfoList findUsers(const QStringList &args);

    static void safetyNameCard(QString &nameCard);
    static void formatNameCard(QString &nameCard);

protected:
    void init(const QJsonObject &o);

private:
    QSet<qint64> superUsers;
    QSet<qint64> managedGroups;
    QSet<qint64> banHongbaoGroups;

    QStringList  welcomeImages;

protected:
    void saveWelcomes(const QString &id, HtmlFeedback::Style style);

private:
    MasterLevels *levels;
    MemberWelcome *welcome;
    MemberBlacklist *blacklist;
    MemberDeathHouse *deathHouse;

    HtmlFeedback *htmlFeedback;

    int checkTimerId;
};

#endif // MANAGEMODULE_P_H
