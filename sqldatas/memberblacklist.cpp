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
    : CqSqlite(*new MemberBlacklistPrivate(), parent)
{
    Q_D(MemberBlacklist);

    setFileName(QStringLiteral("Blacklist.db"));

    do {
        const char sql[] = "CREATE TABLE IF NOT EXISTS [Blacklist] ("
                           "[gid] INT8 NOT NULL, "
                           "[uid] INT8 NOT NULL, "
                           "[stamp] INT8 NOT NULL, "
                           "PRIMARY KEY ([gid], [uid]));";
        prepare(QString::fromLatin1(sql));
    } while (false);

    if (openDatabase()) {
        do {
            const char sql[] = "SELECT * FROM [Blacklist];";
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

MemberBlacklist::~MemberBlacklist()
{
}

CqSqlite::Result MemberBlacklist::addMember(qint64 gid, qint64 uid)
{
    Q_D(MemberBlacklist);
    QWriteLocker locker(&d->guard);

    Member member(gid, uid);
    if (!d->members.contains(member)) {
        qint64 stamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        const char sql[] = "REPLACE INTO [Blacklist] VALUES(%1, %2, %3);";
        QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid).arg(stamp);
        QSqlQuery query = this->query(qtSql);
        if (query.lastError().isValid()) {
            qCCritical(qlcMemberBlacklist, "Update error: %s",
                       qPrintable(query.lastError().text()));
            return SqlError;
        }
        d->members.insert(member, stamp);
        qCInfo(qlcMemberBlacklist, "Update: gid: %lld, uid: %lld.", gid, uid);

        return Done;
    }

    return NoChange;
}

CqSqlite::Result MemberBlacklist::removeMember(qint64 gid, qint64 uid)
{
    Q_D(MemberBlacklist);
    QWriteLocker locker(&d->guard);

    Member member(gid, uid);
    if (d->members.contains(member)) {
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
    }

    return NoChange;
}

QHash<Member, qint64> MemberBlacklist::members() const
{
    Q_D(const MemberBlacklist);
    return d->members;
}

bool MemberBlacklist::contains(qint64 gid, qint64 uid) const
{
    Q_D(const MemberBlacklist);
    QReadLocker locker(&d->guard);

    if (d->members.contains(Member(0, uid))) {
        return true;
    }
    return d->members.contains(Member(gid, uid));
}

// class MemberBlacklistPrivate

MemberBlacklistPrivate::MemberBlacklistPrivate()
{
}

MemberBlacklistPrivate::~MemberBlacklistPrivate()
{
}
