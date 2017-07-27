#include "searchmodule.h"
#include "searchmodule_p.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include <QJsonArray>

#include <QtDebug>

Q_GLOBAL_STATIC(SearchModule, theSearchModule2)

// class SearchModule

SearchModule::SearchModule(CqEngine *engine)
    : CqModule(engine)
    , d_ptr(new SearchModulePrivate())
{
    d_ptr->q_ptr = this;

    d_ptr->apiKey = "56d5e6aae544f7bf27015ffd5f19c0df238b6e93e7309e3ec5e2883ee1b71317";
    d_ptr->apiUsername = "ericzh";

    d_ptr->network = new QNetworkAccessManager(this);
}

SearchModule::~SearchModule()
{
}

SearchModule *SearchModule::instance()
{
    return theSearchModule2();
}

bool SearchModule::privateMessageEvent(const MessageEvent &ev)
{
    Q_UNUSED(ev); return false;
}

bool SearchModule::groupMessageEvent(const MessageEvent &ev)
{
    if ((strlen(ev.gbkMsg) > 3)
            && ev.gbkMsg[0] == '!'
            && ev.gbkMsg[1] == 'q'
            && ev.gbkMsg[2] == ' ') {
        QString msg = convert(ev.gbkMsg);
        groupSearch(ev.from, ev.sender, msg.mid(2));

        return true;
    }
    Q_UNUSED(ev); return false;
}

bool SearchModule::discussMessageEvent(const MessageEvent &ev)
{
    Q_UNUSED(ev); return false;
}

void SearchModule::groupSearch(qint64 gid, qint64 uid, const QString &key)
{
    QMetaObject::invokeMethod(this, "_q_groupSearch", Qt::QueuedConnection,
                              Q_ARG(qint64, gid), Q_ARG(qint64, uid), Q_ARG(QString, key));
}

void SearchModule::_q_groupSearchFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qint64 gid = reply->property("gid").toLongLong();
    qint64 uid = reply->property("uid").toLongLong();
    QString key = reply->property("key").toString();

    qInfo() << gid << uid << key;

    if (reply->error() == QNetworkReply::NoError) {
        qInfo() << gid << uid << key;
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        groupSearchResult(gid, uid, key, doc.object());
    } else {
        qInfo() << reply->error() << reply->errorString();
    }
}

void SearchModule::_q_groupSearch(qint64 gid, qint64 uid, const QString &key)
{
    Q_D(SearchModule);

    QString searchUrl = d->searchBaseUrl.arg(d->apiKey, d->apiUsername, key);
    QNetworkReply *reply = d->network->get(QNetworkRequest(QUrl(searchUrl)));
    connect(reply, &QNetworkReply::finished,
            this, &SearchModule::_q_groupSearchFinished);

    reply->setProperty("gid", gid);
    reply->setProperty("uid", uid);
    reply->setProperty("key", key);
}

void SearchModule::groupSearchResult(qint64 gid, qint64 uid, const QString &key, const QJsonObject &result)
{
    QString msg;
    QTextStream ts(&msg);

    ts << cqAt(uid) << endl;

    if (result.contains("error_type")) {
        ;
    } else {
        QJsonArray topics = result.value("topics").toArray();
        if (topics.count() == 0) {
            ts << tr("Sorry, Not found about %1.").arg(key);
        } else {
            ts << tr("Keywords: ") << key << endl;
            ts << endl;

            for (int i = 0; i < topics.count() && i < 5; ++i) {
                if (i != 0) { ts << endl; }
                QJsonObject topic = topics.at(i).toObject();
                ts << tr("Topic: ") << topic.value("fancy_title").toString() << endl;
                auto lastPostedAt = QDateTime::fromString(topic.value("last_posted_at").toString(), Qt::ISODate);
                int year = lastPostedAt.date().year();
                int month = lastPostedAt.date().month();
                int day = lastPostedAt.date().day();
                ts << tr("%1y%2m%3d").arg(year).arg(month, 2, 10, QLatin1Char('0')).arg(day, 2, 10, QLatin1Char('0'));
                ts << "https://qtdevs.org/t/topic/" << topic.value("id").toInt();
            }
        }
    }

    ts.flush();

    sendGroupMessage(gid, msg);
}

// class SearchModulePrivate

SearchModulePrivate::SearchModulePrivate()
    : q_ptr(nullptr)
    , network(nullptr)
{
    searchBaseUrl = "https://qtdevs.org/search/query.json?api_key=%1&api_username=%2&term=%3";
}

SearchModulePrivate::~SearchModulePrivate()
{
}
