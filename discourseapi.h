#ifndef DISCOURSEAPI_H
#define DISCOURSEAPI_H

#include <QObject>

class DiscourseApiPrivate;
class DiscourseApi : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DiscourseApi)

public:
    explicit DiscourseApi(QObject *parent = nullptr);
    virtual ~DiscourseApi();
protected:
    QScopedPointer<DiscourseApiPrivate> d_ptr;

public:
    static DiscourseApi *instance();

signals:
    void groupSearchResult(qint64 gid, qint64 uid, const QString &key, const QJsonObject &result);
public:
    void groupSearch(qint64 gid, qint64 uid, const QString &key);

private:
    void _q_groupSearchFinished();
private slots:
    void _q_groupSearch(qint64 gid, qint64 uid, const QString &key);
};

#endif // DISCOURSEAPI_H
