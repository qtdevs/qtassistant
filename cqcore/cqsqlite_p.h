#ifndef CQSQLITE_P_H
#define CQSQLITE_P_H

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

public:
    void setFileName(const QString &fileName);
    void prepare(const QString &s);
    bool openDatabase();

public:
    QSqlQuery query(const QByteArray &sql);
    QSqlQuery query(const QString &sql);
    QSqlQuery query(const char *sql);

public:
    mutable QReadWriteLock guard;

private:
    QString fileName;
    QStringList prepareSqls;
    QSqlDatabase dbs;
};

#endif // CQSQLITE_P_H
