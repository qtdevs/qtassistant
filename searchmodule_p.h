#ifndef SEARCHMODULE_P_H
#define SEARCHMODULE_P_H

#include <QEvent>
#include <QMetaMethod>
#include <QNetworkAccessManager>

#include "searchmodule.h"

class SearchEvent : public QEvent
{
public:
    enum LangSpec {
        NoLangSpec,
        EnglishOnly,
        ChineseOnly,
    };

    enum SearchEventType {
        Search = QEvent::User + 1,
    };

public:
    SearchEvent(qint64 did, qint64 gid, qint64 uid,
                const QStringList &keywords, LangSpec langSpec,
                const QString &siteSpec)
        : QEvent(QEvent::Type(Search))
        , did(did), gid(gid), uid(uid)
        , keywords(keywords), langSpec(langSpec), siteSpec(siteSpec)
    { }

public:
    qint64 did;
    qint64 gid;
    qint64 uid;
public:
    QStringList keywords;
    LangSpec    langSpec;
    QString     siteSpec;
};

class SearchModulePrivate
{
    Q_DECLARE_PUBLIC(SearchModule)

public:
    SearchModulePrivate();
    virtual ~SearchModulePrivate();
protected:
    SearchModule *q_ptr;

public:
    bool search(qint64 did, qint64 gid, const MessageEvent &ev);
    void parseSearchResult(QNetworkReply *reply);

protected:
    QNetworkAccessManager *network;
    QString                searchUrl;
};

#endif // SEARCHMODULE_P_H
