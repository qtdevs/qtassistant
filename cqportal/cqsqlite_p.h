#ifndef CQSQLITE_P_H
#define CQSQLITE_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include <QReadWriteLock>
#include <QSqlDatabase>

#include "cqsqlite.h"

class CqSqlitePrivate
{
    Q_DECLARE_PUBLIC(CqSqlite)

public:
    CqSqlitePrivate();
    virtual ~CqSqlitePrivate();
protected:
    CqSqlite *q_ptr;

public:
    static QString basePath;
    mutable QReadWriteLock guard;

private:
    QString fileName;
    QStringList prepareSqls;
    QSqlDatabase dbs;
};

#endif // CQSQLITE_P_H
