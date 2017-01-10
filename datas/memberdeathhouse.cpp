#include "memberdeathhouse.h"
#include "memberdeathhouse_p.h"

#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QLoggingCategory>
#include <QReadWriteLock>

Q_LOGGING_CATEGORY(qlcMemberDeathHouse, "MemberDeathHouse")

// class MemberDeathHouse

MemberDeathHouse::MemberDeathHouse(QObject *parent)
    : SqlData(*new MemberDeathHousePrivate(), parent)
{
    Q_D(MemberDeathHouse);

    d->setFileName(QStringLiteral("DeathHouse.db"));

    do {
        const char sql[] = "CREATE TABLE IF NOT EXISTS [DeathHouse] ("
                           "[gid] INT8 NOT NULL, "
                           "[uid] INT8 NOT NULL, "
                           "[stamp] INT8 NOT NULL, "
                           "PRIMARY KEY ([gid], [uid]));";
        d->prepare(QString::fromLatin1(sql));
    } while (false);

    if (d->openDatabase()) {
        do {
            const char sql[] = "SELECT * FROM [DeathHouse];";
            QSqlQuery query = d->query(sql);
            while (query.next()) {
                qint64 gid = query.value(0).toLongLong();
                qint64 uid = query.value(1).toLongLong();
                qint64 stamp = query.value(2).toLongLong();
                d->deaths.insert(Member(gid, uid), stamp);
            }
        } while (false);
    }
}

MemberDeathHouse::~MemberDeathHouse()
{
}

void MemberDeathHouse::killMember(qint64 gid, qint64 uid)
{
    Q_D(MemberDeathHouse);

    Member member(gid, uid);
    QWriteLocker locker(&d->guard);

    if (d->deaths.contains(member)) {
        return;
    }

    qint64 stamp = QDateTime::currentDateTime().toMSecsSinceEpoch();

    const char sql[] = "REPLACE INTO [DeathHouse] VALUES(%1, %2, %3);";
    QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid).arg(stamp);
    QSqlQuery query = d->query(qtSql);
    if (query.lastError().isValid()) {
        qCCritical(qlcMemberDeathHouse, "Update error: %s",
                   qPrintable(query.lastError().text()));
        return;
    }
    d->deaths.insert(member, stamp);

    qCInfo(qlcMemberDeathHouse, "Update: gid: %lld, uid: %lld.", gid, uid);
}

void MemberDeathHouse::freeMember(qint64 gid, qint64 uid)
{
    Q_D(MemberDeathHouse);

    Member member(gid, uid);
    QWriteLocker locker(&d->guard);

    if (!d->deaths.contains(member)) {
        return;
    }

    const char sql[] = "DELETE FROM [DeathHouse] WHERE [gid] = %1 AND [uid] = %2;";
    QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid);
    QSqlQuery query = d->query(qtSql);
    if (query.lastError().isValid()) {
        qCCritical(qlcMemberDeathHouse, "Delete error: %s",
                   qPrintable(query.lastError().text()));
        return;
    }
    d->deaths.remove(member);

    qCInfo(qlcMemberDeathHouse, "Delete: gid: %lld, uid: %lld.", gid, uid);
}

QHash<Member, qint64> MemberDeathHouse::deaths() const
{
    Q_D(const MemberDeathHouse);
    return d->deaths;
}

// class MemberDeathHousePrivate

MemberDeathHousePrivate::MemberDeathHousePrivate()
{
}

MemberDeathHousePrivate::~MemberDeathHousePrivate()
{
}
