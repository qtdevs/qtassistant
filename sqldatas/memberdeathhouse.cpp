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
    : CqSqlite(*new MemberDeathHousePrivate(), parent)
{
    Q_D(MemberDeathHouse);

    setFileName(QStringLiteral("DeathHouse.db"));

    do {
        const char sql[] = "CREATE TABLE IF NOT EXISTS [DeathHouse] ("
                           "[gid] INT8 NOT NULL, "
                           "[uid] INT8 NOT NULL, "
                           "[stamp] INT8 NOT NULL, "
                           "PRIMARY KEY ([gid], [uid]));";
        prepare(QString::fromLatin1(sql));
    } while (false);

    if (openDatabase()) {
        do {
            const char sql[] = "SELECT * FROM [DeathHouse];";
            QSqlQuery query = this->query(sql);
            while (query.next()) {
                qint64 gid = query.value(0).toLongLong();
                qint64 uid = query.value(1).toLongLong();
                qint64 stamp = query.value(2).toLongLong();
                d->members.insert(Member(gid, uid), stamp);
            }
        } while (false);
    }
}

MemberDeathHouse::~MemberDeathHouse()
{
}

CqSqlite::Result MemberDeathHouse::addMember(qint64 gid, qint64 uid)
{
    Q_D(MemberDeathHouse);
    QWriteLocker locker(&d->guard);

    Member member(gid, uid);
    if (!d->members.contains(member)) {
        qint64 stamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        const char sql[] = "REPLACE INTO [DeathHouse] VALUES(%1, %2, %3);";
        QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid).arg(stamp);
        QSqlQuery query = this->query(qtSql);
        if (query.lastError().isValid()) {
            qCCritical(qlcMemberDeathHouse, "Update error: %s",
                       qPrintable(query.lastError().text()));
            return SqlError;
        }
        d->members.insert(member, stamp);
        qCInfo(qlcMemberDeathHouse, "Update: gid: %lld, uid: %lld.", gid, uid);

        return Done;
    }

    return NoChange;
}

CqSqlite::Result MemberDeathHouse::removeMember(qint64 gid, qint64 uid)
{
    Q_D(MemberDeathHouse);
    QWriteLocker locker(&d->guard);

    Member member(gid, uid);
    if (d->members.contains(member)) {
        const char sql[] = "DELETE FROM [DeathHouse] WHERE [gid] = %1 AND [uid] = %2;";
        QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid);
        QSqlQuery query = this->query(qtSql);
        if (query.lastError().isValid()) {
            qCCritical(qlcMemberDeathHouse, "Delete error: %s",
                       qPrintable(query.lastError().text()));
            return SqlError;
        }
        d->members.remove(member);
        qCInfo(qlcMemberDeathHouse, "Delete: gid: %lld, uid: %lld.", gid, uid);

        return Done;
    }

    return NoChange;
}

QHash<Member, qint64> MemberDeathHouse::members() const
{
    Q_D(const MemberDeathHouse);
    return d->members;
}

void MemberDeathHouse::expiredMembers(MemberList &members)
{
    Q_D(MemberDeathHouse);
    QWriteLocker locker(&d->guard);

    qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QMutableHashIterator<Member, qint64> iter(d->members);
    while (iter.hasNext()) {
        iter.next();
        if ((iter.value() + 300000) < now) {
            members << iter.key();
            iter.remove();
        }
    }
}

// class MemberDeathHousePrivate

MemberDeathHousePrivate::MemberDeathHousePrivate()
{
}

MemberDeathHousePrivate::~MemberDeathHousePrivate()
{
}
