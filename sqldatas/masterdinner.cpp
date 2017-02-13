#include "masterdinner.h"
#include "masterdinner_p.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QLoggingCategory>
#include <QReadWriteLock>
#include <QMessageBox>

Q_LOGGING_CATEGORY(qlcMasterDinner, "MasterDinner")

// class MasterDinner

MasterDinner::MasterDinner(QObject *parent)
    : CqSqlite(*new MasterDinnerPrivate(), parent)
{
    Q_D(MasterDinner);

    setFileName(QStringLiteral("ydcd.db"));

    do {
        const char sql[] = "CREATE TABLE IF NOT EXISTS [YDCD] ("
                           "[DinnerId] INTEGER NOT NULL, "
                           "[name] varchar(255),"
                           "PRIMARY KEY ([DinnerId]));";
        prepare(QString::fromLatin1(sql));
    } while (false);

    if (openDatabase()) {
        do {
            const char sql[] = "SELECT * FROM [YDCD];";
            QSqlQuery query = this->query(sql);
            while (query.next()) {
                qint64 dinnerId = query.value(0).toLongLong();
                QString name = query.value(1).toString();
                //QMessageBox::information(NULL, QString::number(dinnerId, 10), name, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                d->members.insert(MemberDinner(dinnerId, name),d->members.size() + 1);
            }
        } while (false);
    }
    QMessageBox::information(NULL, QString::number(d->members.count(), 10), tr("个数"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

MasterDinner::~MasterDinner()
{
}

CqSqlite::Result MasterDinner::addMember(qint64 dinnerId, QString name)
{
    Q_D(MasterDinner);
    QWriteLocker locker(&d->guard);

    /*MemberDinner member(dinnerId, name);
    if (!d->members.contains(member)) {
        const char sql[] = "REPLACE INTO [YDCD] VALUES(%1, %2);";
        QString qtSql = QString::fromLatin1(sql).arg(dinnerId).arg(name);
        QSqlQuery query = this->query(qtSql);
        if (query.lastError().isValid()) {
            qCCritical(qlcMasterDinner, "Update error: %s",
                       qPrintable(query.lastError().text()));
            return SqlError;
        }
        d->members.insert(member, 1);
        qCInfo(qlcMasterDinner, "Update: dinnerId: %lld, name: %s.", dinnerId, name);

        return Done;
    }*/

    return NoChange;
}

CqSqlite::Result MasterDinner::removeMember(qint64 dinnerId, QString name)
{
    Q_D(MasterDinner);
    QWriteLocker locker(&d->guard);

    MemberDinner member(dinnerId, name);
   /* if (d->members.contains(member)) {
        const char sql[] = "DELETE FROM [Blacklist] WHERE [gid] = %1 AND [uid] = %2;";
        QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid);
        QSqlQuery query = this->query(qtSql);
        if (query.lastError().isValid()) {
            qCCritical(qlcMemberBlacklist, "Delete error: %s",
                       qPrintable(query.lastError().text()));
            return SqlError;
        }
        d->members.remove(member);
        qCInfo(qlcMemberBlacklist, "Delete: gid: %lld, uid: %lld.", gid, uid);

        return Done;
    }*/

    return NoChange;
}

QHash<MemberDinner, qint64> MasterDinner::members() const
{
    Q_D(const MasterDinner);
    return d->members;
}

bool MasterDinner::contains(qint64 dinnerId, QString name) const
{
    Q_D(const MasterDinner);
    QReadLocker locker(&d->guard);

    if (d->members.contains(MemberDinner(0, name))) {
        return true;
    }
    return d->members.contains(MemberDinner(dinnerId, name));
}

MasterDinnerPrivate::MasterDinnerPrivate()
{
}

MasterDinnerPrivate::~MasterDinnerPrivate()
{
}
