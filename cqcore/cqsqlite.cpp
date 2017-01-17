/*!
 * \class CqSqlite
 * \brief 数据存储类
 */

/*!
 * \enum CqSqlite::Result
 * \brief CqSqlite 执行结果
 *
 * 酷Q API 密切相关的执行函数都会返回此枚举作为结果表达。
 */

/*!
 * \var CqSqlite::NoChange
 * \brief 没有发生改变
 *
 * 如果数据库内容没有发生变化，返回此枚举值。
 */

/*!
 * \var CqSqlite::Done
 * \brief 已经完成
 *
 * 如果数据库操作执行成功，返回此枚举值。
 */

/*!
 * \var CqSqlite::SqlError
 * \brief 数据库错误
 *
 * 如果数据库操作执行失败，返回此枚举值。更多信息可以看酷Q的日志。
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

/*!
 * \brief 构造函数
 *
 * \internal
 */
CqSqlite::CqSqlite(CqSqlitePrivate &dd, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
 * \brief 析构函数
 *
 * \internal
 */
CqSqlite::~CqSqlite()
{
}

/*!
 * \brief 设置文件名
 *
 * 这里的文件名 \a fileName 不包含路径，只有文件名和扩展名；文件位置在插件目录内。
 */
void CqSqlite::setFileName(const QString &fileName)
{
    Q_D(CqSqlite);

    d->fileName = fileName;
}

/*!
 * \brief 设置预处理
 *
 * 添加预处理命令。每次文件加载后都将执行 \a s 语句。可以添加多条语句。
 */
void CqSqlite::prepare(const QString &s)
{
    Q_D(CqSqlite);

    d->prepareSqls.append(s);
}

/*!
 * \brief 打开数据库
 *
 * 此方法将打开数据库文件，如果成功返回 true；否则返回 false。
 */
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

    for (const auto &sql: d->prepareSqls) {
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

/*!
 * \brief 执行 Sql 语句
 *
 * 此方法将创建一个 QSqlQuery 并返回该对象，该对象将执行 \a sql 语句。
 */
QSqlQuery CqSqlite::query(const QString &sql)
{
    Q_D(CqSqlite);

    return QSqlQuery(sql, d->dbs);
}

/*!
 * \brief 执行 Sql 语句
 *
 * 此方法将创建一个 QSqlQuery 并返回该对象，该对象将执行 UTF-8 编码的 \a srcSql 语句。
 */
QSqlQuery CqSqlite::query(const char *srcSql)
{
    Q_D(CqSqlite);

    return QSqlQuery(QString::fromUtf8(srcSql), d->dbs);
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
