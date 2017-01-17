/*! \class CqSqlite
 * \brief 数据存储类
 */

#include "cqsqlite.h"
#include "cqsqlite_p.h"

#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QStringBuilder>

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(qlcCqSqlite, "CqSqlite")

// class CqSqlite

CqSqlite::CqSqlite(CqSqlitePrivate &dd, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

CqSqlite::~CqSqlite()
{
}

void CqSqlite::setFileName(const QString &fileName)
{
    Q_D(CqSqlite);

    d->fileName = fileName;
}

void CqSqlite::prepare(const QString &s)
{
    Q_D(CqSqlite);

    d->prepareSqls.append(s);
}

bool CqSqlite::openDatabase()
{
    Q_D(CqSqlite);

    if (d->fileName.isEmpty()) {
        return false;
    }

    d->dbs = QSqlDatabase::addDatabase("QSQLITE", d->fileName);
    QString sqliteFileName = QDir::cleanPath(d->basePath % "/" % d->fileName);
    d->dbs.setDatabaseName(sqliteFileName);
    if (!d->dbs.open()) {
        qCCritical(qlcCqSqlite, "%s: Open failed: %s",
                   qPrintable(sqliteFileName),
                   qPrintable(d->dbs.lastError().text()));
        return false;
    }

    for (const QString &sql: d->prepareSqls) {
        QSqlQuery result = d->dbs.exec(sql);
        if (result.lastError().isValid()) {
            qCCritical(qlcCqSqlite, "%s: Prepare failed: %s",
                       qPrintable(sqliteFileName),
                       qPrintable(result.lastError().text()));
            return false;
        }
    }

    qCInfo(qlcCqSqlite, "%s: Ready.",
           qPrintable(sqliteFileName));

    return true;
}

QSqlQuery CqSqlite::query(const QByteArray &sql)
{
    Q_D(CqSqlite);

    return QSqlQuery(QString::fromLatin1(sql), d->dbs);
}

QSqlQuery CqSqlite::query(const QString &sql)
{
    Q_D(CqSqlite);

    return QSqlQuery(sql, d->dbs);
}

QSqlQuery CqSqlite::query(const char *sql)
{
    Q_D(CqSqlite);

    return QSqlQuery(QString::fromLatin1(sql), d->dbs);
}

// class CqSqlitePrivate

QString CqSqlitePrivate::basePath;

CqSqlitePrivate::CqSqlitePrivate()
    : q_ptr(Q_NULLPTR)
{
}

CqSqlitePrivate::~CqSqlitePrivate()
{
}
