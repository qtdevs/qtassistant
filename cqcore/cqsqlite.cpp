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

// class CqSqlitePrivate

QString CqSqlitePrivate::basePath;

CqSqlitePrivate::CqSqlitePrivate()
    : q_ptr(Q_NULLPTR)
{
}

CqSqlitePrivate::~CqSqlitePrivate()
{
}

void CqSqlitePrivate::setFileName(const QString &fileName)
{
    this->fileName = fileName;
}

void CqSqlitePrivate::prepare(const QString &s)
{
    prepareSqls.append(s);
}

bool CqSqlitePrivate::openDatabase()
{
    dbs = QSqlDatabase::addDatabase("QSQLITE", fileName);
    QString fileFullName = QDir::cleanPath(basePath % "/" % fileName);
    dbs.setDatabaseName(fileFullName);
    if (!dbs.open()) {
        qCCritical(qlcCqSqlite, "%s: Open failed: %s",
                   qPrintable(fileFullName),
                   qPrintable(dbs.lastError().text()));
        return false;
    }

    for (const QString &sql: prepareSqls) {
        QSqlQuery result = dbs.exec(sql);
        if (result.lastError().isValid()) {
            qCCritical(qlcCqSqlite, "%s: Prepare failed: %s",
                       qPrintable(fileFullName),
                       qPrintable(result.lastError().text()));
            return false;
        }
    }

    qCInfo(qlcCqSqlite, "%s: Ready.",
           qPrintable(fileFullName));

    return true;
}

QSqlQuery CqSqlitePrivate::query(const QByteArray &sql)
{
    return QSqlQuery(QString::fromLatin1(sql), dbs);
}

QSqlQuery CqSqlitePrivate::query(const QString &sql)
{
    return QSqlQuery(sql, dbs);
}

QSqlQuery CqSqlitePrivate::query(const char *sql)
{
    return QSqlQuery(QString::fromLatin1(sql), dbs);
}
