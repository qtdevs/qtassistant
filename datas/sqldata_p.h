#ifndef SQLDATA_P_H
#define SQLDATA_P_H

#include <QReadWriteLock>
#include <QSqlDatabase>

#include "sqldata.h"

class SqlDataPrivate
{
    Q_DECLARE_PUBLIC(SqlData)

public:
    SqlDataPrivate();
    virtual ~SqlDataPrivate();
protected:
    SqlData *q_ptr;

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
    QSqlDatabase qdb;
};

#endif // SQLDATA_P_H
