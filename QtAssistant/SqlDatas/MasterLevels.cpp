#include "MasterLevels.h"
#include "MasterLevels_p.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QLoggingCategory>
#include <QReadWriteLock>

Q_LOGGING_CATEGORY(qlcMasterLevels, "MasterLevels")

// class MasterLevels

MasterLevels::MasterLevels(QObject *parent)
    : CoolQ::SqliteService(*new MasterLevelsPrivate(), parent)
{
    Q_D(MasterLevels);

    setFileName(QStringLiteral("Levels.db"));

    do {
        const char sql[] = "CREATE TABLE IF NOT EXISTS [Levels] ("
                           "[gid] INT8 NOT NULL, "
                           "[uid] INT8 NOT NULL, "
                           "[level] INT8 NOT NULL, "
                           "PRIMARY KEY ([gid], [uid]));";
        prepare(QString::fromLatin1(sql));
    } while (false);

    if (openDatabase()) {
        const char sql[] = "SELECT * FROM [Levels];";
        QSqlQuery query = this->query(sql);
        while (query.next()) {
            qint64 gid = query.value(0).toLongLong();
            qint64 uid = query.value(1).toLongLong();
            qint64 level = query.value(2).toLongLong();
            d->levels.insert(CoolQ::Member(gid, uid), levelCast(level));
        }
    }
}

MasterLevels::~MasterLevels()
{
}

void MasterLevels::init(const QSet<qint64> &superUsers)
{
    Q_D(MasterLevels);

    d->superUsers = superUsers;
}

MasterLevel MasterLevels::levelCast(int level)
{
    switch (level) {
    case -1:
        return MasterLevel::ATField;
    case 11:
        return MasterLevel::Master1;
    case 12:
        return MasterLevel::Master2;
    case 13:
        return MasterLevel::Master3;
    case 14:
        return MasterLevel::Master4;
    case 15:
        return MasterLevel::Master5;
    case 99:
        return MasterLevel::Unknown;
    default:
        Q_ASSERT(false);
    }

    return MasterLevel::Unknown;
}

QString MasterLevels::levelName(MasterLevel level)
{
    switch (level) {
    case MasterLevel::ATField:
        return QString(u8"绝对领域");
    case MasterLevel::Master1:
        return QString(u8"首席管理");
    case MasterLevel::Master2:
        return QString(u8"二级管理");
    case MasterLevel::Master3:
        return QString(u8"三级管理");
    case MasterLevel::Master4:
        return QString(u8"四级管理");
    case MasterLevel::Master5:
        return QString(u8"五级管理");
    case MasterLevel::Unknown:
        return QString(u8"普通成员");
    default:
        Q_ASSERT(false);
    }

    return QString(u8"普通成员");
}

int MasterLevels::levelCast(MasterLevel level)
{
    switch (level) {
    case MasterLevel::ATField:
        return -1;
    case MasterLevel::Master1:
        return 11;
    case MasterLevel::Master2:
        return 12;
    case MasterLevel::Master3:
        return 13;
    case MasterLevel::Master4:
        return 14;
    case MasterLevel::Master5:
        return 15;
    case MasterLevel::Unknown:
        return 99;
    default:
        Q_ASSERT(false);
    }

    return 99;
}

QHash<CoolQ::Member, MasterLevel> MasterLevels::levels() const
{
    Q_D(const MasterLevels);
    return d->levels;
}

bool MasterLevels::setLevel(qint64 gid, qint64 uid, MasterLevel level)
{
    Q_D(MasterLevels);

    if (MasterLevel::ATField == level) {
        return false;
    }

    CoolQ::Member member(gid, uid);
    QWriteLocker locker(&d->guard);

    if (MasterLevel::Unknown != level) {
        const char sql[] = "REPLACE INTO [Levels] VALUES(%1, %2, %3);";
        QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid).arg(levelCast(level));
        QSqlQuery query = this->query(qtSql);
        if (query.lastError().isValid()) {
            qCCritical(qlcMasterLevels, "Update error: %s",
                       qPrintable(query.lastError().text()));
            return false;
        }
        d->levels.insert(member, level);

        qCInfo(qlcMasterLevels,
               "Update: gid: %lld, uid: %lld, level: %d.",
               gid, uid, levelCast(level));
    } else {
        const char sql[] = "DELETE FROM [Levels] WHERE [gid] = %1 AND [uid] = %2;";
        QString qtSql = QString::fromLatin1(sql).arg(gid).arg(uid);
        QSqlQuery query = this->query(qtSql);
        if (query.lastError().isValid()) {
            qCCritical(qlcMasterLevels, "Delete error: %s",
                       qPrintable(query.lastError().text()));
            return false;
        }
        d->levels.remove(member);

        qCInfo(qlcMasterLevels,
               "Delete: gid: %lld, uid: %lld, level: %d.",
               gid, uid, levelCast(level));
    }

    return true;
}

MasterLevel MasterLevels::level(qint64 gid, qint64 uid) const
{
    Q_D(const MasterLevels);

    if (d->superUsers.contains(uid))
        return MasterLevel::ATField;

    QReadLocker locker(&d->guard);

    MasterLevel level = d->levels.value(CoolQ::Member(gid, uid), MasterLevel::Unknown);
    if (MasterLevel::Unknown != level) {
        return level;
    }
    return d->levels.value(CoolQ::Member(0, uid), MasterLevel::Unknown);
}

bool mlSortLevel(const LevelInfo &lhs, const LevelInfo &rhs)
{
    return lhs.level != rhs.level ? lhs.level < rhs.level : lhs.uid < rhs.uid;
}

LevelInfoList MasterLevels::levels(qint64 gid) const
{
    Q_D(const MasterLevels);

    LevelInfoList levels;

    QReadLocker locker(&d->guard);

    QHashIterator<CoolQ::Member, MasterLevel> i(d->levels);
    while (i.hasNext()) {
        i.next();
        if (i.key().first == gid) {
            levels << LevelInfo(i.key().second, i.value());
        }
    }
    std::sort(levels.begin(), levels.end(), mlSortLevel);

    return levels;
}

void MasterLevels::update(qint64 gid, LevelInfoList &l) const
{
    Q_D(const MasterLevels);

    QReadLocker locker(&d->guard);

    QMutableListIterator<LevelInfo> i(l);
    while (i.hasNext()) {
        LevelInfo &li = i.next();

        if (d->superUsers.contains(li.uid)) {
            li.level = MasterLevel::ATField;
        } else {
            li.level = d->levels.value(CoolQ::Member(gid, li.uid), MasterLevel::Unknown);
            if (MasterLevel::Unknown == li.level) {
                li.level = d->levels.value(CoolQ::Member(0, li.uid), MasterLevel::Unknown);
            }
        }
    }
}

// class MasterLevelsPrivate

MasterLevelsPrivate::MasterLevelsPrivate()
{
}

MasterLevelsPrivate::~MasterLevelsPrivate()
{
}
