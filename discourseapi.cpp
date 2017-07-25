#include "discourseapi.h"
#include "discourseapi_p.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include <QtDebug>

Q_GLOBAL_STATIC(DiscourseApi, theDiscourseApi2)

// class DiscourseApi

DiscourseApi::DiscourseApi(QObject *parent)
    : QObject(parent)
    , d_ptr(new DiscourseApiPrivate())
{
    d_ptr->q_ptr = this;

    d_ptr->apiKey = "56d5e6aae544f7bf27015ffd5f19c0df238b6e93e7309e3ec5e2883ee1b71317";
    d_ptr->apiUsername = "ericzh";

    d_ptr->network = new QNetworkAccessManager(this);
}

DiscourseApi::~DiscourseApi()
{
}

DiscourseApi *DiscourseApi::instance()
{
    return theDiscourseApi2();
}

void DiscourseApi::groupSearch(qint64 gid, qint64 uid, const QString &key)
{
    QMetaObject::invokeMethod(this, "_q_groupSearch", Qt::QueuedConnection,
                              Q_ARG(qint64, gid), Q_ARG(qint64, uid), Q_ARG(QString, key));
}

void DiscourseApi::_q_groupSearchFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qint64 gid = reply->property("gid").toLongLong();
    qint64 uid = reply->property("uid").toLongLong();
    QString key = reply->property("key").toString();

    qInfo() << gid << uid << key;

    if (reply->error() == QNetworkReply::NoError) {
        qInfo() << gid << uid << key;
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        emit groupSearchResult(gid, uid, key, doc.object());
    } else {
        qInfo() << reply->error() << reply->errorString();
    }
}

void DiscourseApi::_q_groupSearch(qint64 gid, qint64 uid, const QString &key)
{
    Q_D(DiscourseApi);

    QString searchUrl = d->searchBaseUrl.arg(d->apiKey, d->apiUsername, key);
    QNetworkReply *reply = d->network->get(QNetworkRequest(QUrl(searchUrl)));
    connect(reply, &QNetworkReply::finished,
            this, &DiscourseApi::_q_groupSearchFinished);

    reply->setProperty("gid", gid);
    reply->setProperty("uid", uid);
    reply->setProperty("key", key);
}

// class DiscourseApiPrivate

DiscourseApiPrivate::DiscourseApiPrivate()
    : q_ptr(nullptr)
    , network(nullptr)
{
    searchBaseUrl = "https://qtdevs.org/search/query.json?api_key=%1&api_username=%2&term=%3";
}

DiscourseApiPrivate::~DiscourseApiPrivate()
{
}
