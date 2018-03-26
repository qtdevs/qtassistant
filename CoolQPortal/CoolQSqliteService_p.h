#ifndef CQSQLITESERVICE_P_H
#define CQSQLITESERVICE_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include <QReadWriteLock>
#include <QSqlDatabase>

#include "CoolQInterface_p.h"
#include "CoolQSqliteService.h"

namespace CoolQ {

class SqliteServicePrivate : public InterfacePrivate
{
    Q_DECLARE_PUBLIC(SqliteService)

public:
    SqliteServicePrivate();
    virtual ~SqliteServicePrivate();

public:
    static QString basePath;
    mutable QReadWriteLock guard;

private:
    QString fileName;
    QStringList prepareSqls;
    QSqlDatabase dbs;
};

} // namespace CoolQ

#endif // CQSQLITESERVICE_P_H
