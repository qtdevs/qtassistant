#include "MemberWatchlist.h"
#include "MemberWatchlist_p.h"

#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QLoggingCategory>
#include <QReadWriteLock>

Q_LOGGING_CATEGORY(qlcMemberWatchlist, "Watchlist")

// class MemberWatchlist

MemberWatchlist::MemberWatchlist(QObject *parent)
    : CoolQ::SqliteService(*new MemberWatchlistPrivate(), parent)
{
    Q_D(MemberWatchlist);

    setFileName(QStringLiteral("Watchlist.db"));

    do {
        const char sql[] = "CREATE TABLE IF NOT EXISTS [Watchlist] ("
                           "[gid] INT8 NOT NULL, "
                           "[uid] INT8 NOT NULL, "
                           "[stamp] INT8 NOT NULL, "
                           "PRIMARY KEY ([gid], [uid]));";
        prepare(QString::fromLatin1(sql));
    } while (false);

    if (openDatabase()) {
        do {
            const char sql[] = "SELECT * FROM [Watchlist];";
            QSqlQuery query = this->query(sql);
            while (query.next()) {
                qint64 gid = query.value(0).toLongLong();
                qint64 uid = query.value(1).toLongLong();
                qint64 stamp = query.value(2).toLongLong();
                d->members.insert(CoolQ::Member(gid, uid), stamp);
            }
        } while (false);
    }
}

MemberWatchlist::~MemberWatchlist()
{
}

CoolQ::SqliteService::Result MemberWatchlist::addMember(qint64 gid, qint64 uid)
{
    Q_D(MemberWatchlist);
    QWriteLocker locker(&d->guard);

    CoolQ::Member member(gid, uid);
    if (!d->members.contains(member)) {
        qint64 stamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        const char sql[] = "REPLACE INTO [Watchlist] VALUES(%1, %2, %3);";
        QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid).arg(stamp);
        QSqlQuery query = this->query(qtSql);
        if (query.lastError().isValid()) {
            qCCritical(qlcMemberWatchlist, "Update error: %s",
                       qPrintable(query.lastError().text()));
            return SqlError;
        }
        d->members.insert(member, stamp);
        qCInfo(qlcMemberWatchlist, "Update: gid: %lld, uid: %lld.", gid, uid);

        return Done;
    }

    return NoChange;
}

CoolQ::SqliteService::Result MemberWatchlist::removeMember(qint64 gid, qint64 uid)
{
    Q_D(MemberWatchlist);
    QWriteLocker locker(&d->guard);

    CoolQ::Member member(gid, uid);
    if (d->members.contains(member)) {
        const char sql[] = "DELETE FROM [Watchlist] WHERE [gid] = %1 AND [uid] = %2;";
        QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid);
        QSqlQuery query = this->query(qtSql);
        if (query.lastError().isValid()) {
            qCCritical(qlcMemberWatchlist, "Delete error: %s",
                       qPrintable(query.lastError().text()));
            return SqlError;
        }
        d->members.remove(member);
        qCInfo(qlcMemberWatchlist, "Delete: gid: %lld, uid: %lld.", gid, uid);

        return Done;
    }

    return NoChange;
}

QHash<CoolQ::Member, qint64> MemberWatchlist::members() const
{
    Q_D(const MemberWatchlist);

    return d->members;
}

bool MemberWatchlist::contains(qint64 gid, qint64 uid) const
{
    Q_D(const MemberWatchlist);
    QReadLocker locker(&d->guard);

    if (d->members.contains(CoolQ::Member(0, uid))) {
        return true;
    }
    return d->members.contains(CoolQ::Member(gid, uid));
}

void MemberWatchlist::expiredMembers(CoolQ::MemberList &members)
{
    Q_D(MemberWatchlist);
    QWriteLocker locker(&d->guard);

    qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QMutableHashIterator<CoolQ::Member, qint64> iter(d->members);
    while (iter.hasNext()) {
        iter.next();
        if ((iter.value() + 1800000) < now) {
            members << iter.key();
            iter.remove();
        }
    }
}

// class MemberWatchlistPrivate

MemberWatchlistPrivate::MemberWatchlistPrivate()
{
}

MemberWatchlistPrivate::~MemberWatchlistPrivate()
{
}
