#include "sqldata.h"
#include "sqldata_p.h"

#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QStringBuilder>

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(qlcSqlData, "SqlData")

// class SqlData

SqlData::SqlData(SqlDataPrivate &dd, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

SqlData::~SqlData()
{
}

void SqlData::initPath(const QString &path)
{
    SqlDataPrivate::basePath = path;
}

// class SqlDataPrivate

QString SqlDataPrivate::basePath;

SqlDataPrivate::SqlDataPrivate()
    : q_ptr(Q_NULLPTR)
{
}

SqlDataPrivate::~SqlDataPrivate()
{
}

void SqlDataPrivate::setFileName(const QString &fileName)
{
    this->fileName = fileName;
}

void SqlDataPrivate::prepare(const QString &s)
{
    prepareSqls.append(s);
}

bool SqlDataPrivate::openDatabase()
{
    qdb = QSqlDatabase::addDatabase("QSQLITE", fileName);
    QString fileFullName = QDir::cleanPath(basePath % "/" % fileName);
    qdb.setDatabaseName(fileFullName);
    if (!qdb.open()) {
        qCCritical(qlcSqlData, "%s: Open failed: %s",
                   qPrintable(fileFullName),
                   qPrintable(qdb.lastError().text()));
        return false;
    }

    for (const QString &sql: prepareSqls) {
        QSqlQuery result = qdb.exec(sql);
        if (result.lastError().isValid()) {
            qCCritical(qlcSqlData, "%s: Prepare failed: %s",
                       qPrintable(fileFullName),
                       qPrintable(result.lastError().text()));
            return false;
        }
    }

    qCInfo(qlcSqlData, "%s: Ready.",
           qPrintable(fileFullName));

    return true;
}

QSqlQuery SqlDataPrivate::query(const QByteArray &sql)
{
    return QSqlQuery(QString::fromLatin1(sql), qdb);
}

QSqlQuery SqlDataPrivate::query(const QString &sql)
{
    return QSqlQuery(sql, qdb);
}

QSqlQuery SqlDataPrivate::query(const char *sql)
{
    return QSqlQuery(QString::fromLatin1(sql), qdb);
}
