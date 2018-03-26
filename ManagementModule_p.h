#ifndef MANAGEMENTMODULE_P_H
#define MANAGEMENTMODULE_P_H

#include <QSet>

#include "CqServiceModule_p.h"
#include "ManagementModule.h"

class MasterLevels;
class MemberWatchlist;
class MemberBlacklist;

class ManagementModulePrivate : public CoolQ::ServiceModulePrivate
{
    Q_DECLARE_PUBLIC(ManagementModule)

public:
    ManagementModulePrivate();
    virtual ~ManagementModulePrivate();

protected:
    static ManagementModule *instance;

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
    MemberWatchlist *watchlist;
    MemberBlacklist *blacklist;

    HtmlFeedback *htmlFeedback;

    int checkTimerId;
};

#endif // MANAGEMENTMODULE_P_H
