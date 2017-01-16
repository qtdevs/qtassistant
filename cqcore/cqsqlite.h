#ifndef CQSQLITE_H
#define CQSQLITE_H

#include "cqglobaldefs.h"

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
};

#endif // CQSQLITE_H
