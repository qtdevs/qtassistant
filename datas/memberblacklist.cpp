#include "memberblacklist.h"
#include "memberblacklist_p.h"

#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QLoggingCategory>
#include <QReadWriteLock>

Q_LOGGING_CATEGORY(qlcMemberBlacklist, "MemberBlacklist")

// class MemberBlacklist

MemberBlacklist::MemberBlacklist(QObject *parent)
    : SqlData(*new MemberBlacklistPrivate(), parent)
{
    Q_D(MemberBlacklist);

    d->setFileName(QStringLiteral("Blacklist.db"));

    do {
        const char sql[] = "CREATE TABLE IF NOT EXISTS [Blacklist] ("
                           "[gid] INT8 NOT NULL, "
                           "[uid] INT8 NOT NULL, "
                           "[stamp] INT8 NOT NULL, "
                           "PRIMARY KEY ([gid], [uid]));";
        d->prepare(QString::fromLatin1(sql));
    } while (false);

    if (d->openDatabase()) {
        do {
            const char sql[] = "SELECT * FROM [Blacklist];";
            QSqlQuery query = d->query(sql);
            while (query.next()) {
                qint64 gid = query.value(0).toLongLong();
                qint64 uid = query.value(1).toLongLong();
                qint64 stamp = query.value(2).toLongLong();
                d->blacklist.insert(Member(gid, uid), stamp);
            }
        } while (false);
    }
}

MemberBlacklist::~MemberBlacklist()
{
}

SqlData::Result MemberBlacklist::addMember(qint64 gid, qint64 uid)
{
    Q_D(MemberBlacklist);
    QWriteLocker locker(&d->guard);

    Member member(gid, uid);
    if (!d->blacklist.contains(member)) {
        qint64 stamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        const char sql[] = "REPLACE INTO [Blacklist] VALUES(%1, %2, %3);";
        QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid).arg(stamp);
        QSqlQuery query = d->query(qtSql);
        if (query.lastError().isValid()) {
            qCCritical(qlcMemberBlacklist, "Update error: %s",
                       qPrintable(query.lastError().text()));
            return SqlError;
        }
        d->blacklist.insert(member, stamp);
        qCInfo(qlcMemberBlacklist, "Update: gid: %lld, uid: %lld.", gid, uid);

        return Done;
    }

    return NoChange;
}

SqlData::Result MemberBlacklist::removeMember(qint64 gid, qint64 uid)
{
    Q_D(MemberBlacklist);
    QWriteLocker locker(&d->guard);

    Member member(gid, uid);
    if (d->blacklist.contains(member)) {
        const char sql[] = "DELETE FROM [Blacklist] WHERE [gid] = %1 AND [uid] = %2;";
        QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid);
        QSqlQuery query = d->query(qtSql);
        if (query.lastError().isValid()) {
            qCCritical(qlcMemberBlacklist, "Delete error: %s",
                       qPrintable(query.lastError().text()));
            return SqlError;
        }
        d->blacklist.remove(member);
        qCInfo(qlcMemberBlacklist, "Delete: gid: %lld, uid: %lld.", gid, uid);

        return Done;
    }

    return NoChange;
}

QHash<Member, qint64> MemberBlacklist::blacklist() const
{
    Q_D(const MemberBlacklist);
    return d->blacklist;
}

bool MemberBlacklist::contains(qint64 gid, qint64 uid) const
{
    Q_D(const MemberBlacklist);
    QReadLocker locker(&d->guard);

    if (d->blacklist.contains(Member(0, uid))) {
        return true;
    }
    return d->blacklist.contains(Member(gid, uid));
}

// class MemberBlacklistPrivate

MemberBlacklistPrivate::MemberBlacklistPrivate()
{
}

MemberBlacklistPrivate::~MemberBlacklistPrivate()
{
}
