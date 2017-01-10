#ifndef SQLDATA_H
#define SQLDATA_H

#include <QObject>
#include <QVector>

typedef QPair<qint64, qint64> Member;
typedef QVector<Member>       MemberList;

class SqlDataPrivate;
class SqlData : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SqlData)

protected:
    SqlData(SqlDataPrivate &dd, QObject *parent = Q_NULLPTR);
protected:
    QScopedPointer<SqlDataPrivate> d_ptr;
public:
    virtual ~SqlData();

public:
    enum Result { NoChange, Done, SqlError };

public:
    static void initPath(const QString &path);
};

#endif // SQLDATA_H
