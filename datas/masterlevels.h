#ifndef MASTERLEVELS_H
#define MASTERLEVELS_H

#include <QHash>
#include "sqldata.h"

enum class MasterLevel {
    ATField = -1,

    Master1 = 11,
    Master2 = 12,
    Master3 = 13,
    Master4 = 14,
    Master5 = 15,

    Unknown = 99,
};

class LevelInfo
{
public:
    LevelInfo(qint64 uid, MasterLevel level)
        : uid(uid), level(level)
    { }
public:
    qint64 uid;
    MasterLevel level;
};
typedef QList<LevelInfo> LevelInfoList;

class MasterLevelsPrivate;
class MasterLevels : public SqlData
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MasterLevels)

public:
    explicit MasterLevels(QObject *parent = Q_NULLPTR);
    virtual ~MasterLevels();

public:
    static MasterLevel levelCast(int level);
    static QString levelName(MasterLevel level);
    static int levelCast(MasterLevel level);

public:
    QHash<Member, MasterLevel> levels() const;

public:
    bool setLevel(qint64 gid, qint64 uid, MasterLevel level);
    MasterLevel level(qint64 gid, qint64 uid) const;
    LevelInfoList levels(qint64 gid) const;
    void update(qint64 gid, LevelInfoList &l) const;
};

#endif // MASTERLEVELS_H
