#ifndef SEARCHMODULE_H
#define SEARCHMODULE_H

#include "cqmodule.h"

class SearchModulePrivate;
class SearchModule : public CqModule
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SearchModule)

public:
    explicit SearchModule(CqEngine *engine = nullptr);
    virtual ~SearchModule();
protected:
    QScopedPointer<SearchModulePrivate> d_ptr;

public:
    static SearchModule *instance();

public:
    bool privateMessageEvent(const MessageEvent &ev);
    bool groupMessageEvent(const MessageEvent &ev);
    bool discussMessageEvent(const MessageEvent &ev);

public:
    void groupSearchResult(qint64 gid, qint64 uid, const QString &key, const QJsonObject &result);
public:
    void groupSearch(qint64 gid, qint64 uid, const QString &key);

private:
    void _q_groupSearchFinished();
private slots:
    void _q_groupSearch(qint64 gid, qint64 uid, const QString &key);
};

#endif // SEARCHMODULE_H
