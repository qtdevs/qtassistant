#ifndef ASSISTANTMODULE_P_H
#define ASSISTANTMODULE_P_H

#include <QSet>

#include "CoolQServiceModule_p.h"
#include "AssistantModule.h"

class MemberWatchlist;
class MemberBlacklist;

class AssistantModulePrivate : public CoolQ::ServiceModulePrivate
{
    Q_DECLARE_PUBLIC(AssistantModule)

public:
    AssistantModulePrivate();
    virtual ~AssistantModulePrivate();

protected:
    static AssistantModule *instance;

public:
    static QList<qint64> findUsers(const QStringList &args);

    static void safetyNameCard(QString &nameCard);
    static void formatNameCard(QString &nameCard);

protected:
    void init(const QJsonObject &o);

private:
    QSet<qint64> superUsers;
    QSet<qint64> managedGroups;
    QSet<qint64> banHongbaoGroups;

protected:
    void saveWelcomes(const QString &id, HtmlDraw::Style style);

private:
    MemberWatchlist *watchlist;
    MemberBlacklist *blacklist;

    HtmlDraw *htmlDraw;

    int checkTimerId;
};

#endif // ASSISTANTMODULE_P_H
