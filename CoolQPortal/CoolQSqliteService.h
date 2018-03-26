#ifndef COOLQSQLITESERVICE_H
#define COOLQSQLITESERVICE_H

#include <QSqlQuery>

#include "CoolQInterface.h"

namespace CoolQ {

class SqliteServicePrivate;
class SqliteService : public Interface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SqliteService)

protected:
    SqliteService(SqliteServicePrivate &dd, QObject *parent);
public:
    explicit SqliteService(QObject *parent);
public:
    virtual ~SqliteService();

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

} // namespace CoolQ

#endif // COOLQSQLITESERVICE_H
