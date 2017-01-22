#ifndef CQSQLITE_H
#define CQSQLITE_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include <QSqlQuery>

#include "cqglobalevents.h"

class CqSqlitePrivate;
class CqSqlite : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CqSqlite)

protected:
    CqSqlite(CqSqlitePrivate &dd, QObject *parent = Q_NULLPTR);
protected:
    QScopedPointer<CqSqlitePrivate> d_ptr;
public:
    virtual ~CqSqlite();

public:
    enum Result { NoChange, Done, SqlError };

protected:
    void setFileName(const QString &fileName);
    void prepare(const QString &s);
    bool openDatabase();

protected:
    QSqlQuery query(const QString &sql);
    QSqlQuery query(const char *srcSql);
};

#endif // CQSQLITE_H
