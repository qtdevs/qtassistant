#include "searchmodule.h"
#include "searchmodule_p.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include <QJsonArray>
#include <QRegularExpression>
#include <QtDebug>
#include <QCoreApplication>

Q_GLOBAL_STATIC(SearchModule, theSearchModule2)

// class SearchModule

SearchModule::SearchModule(CoolQ::ServicePortal *parent)
    : CoolQ::ServiceModule(*new SearchModulePrivate(), parent)
{
    Q_D(SearchModule);

    d->network = new QNetworkAccessManager(this);
}

SearchModule::~SearchModule()
{
}

bool SearchModule::privateMessageEvent(const CoolQ::MessageEvent &ev)
{
    Q_D(SearchModule);

    if (d->search(0, 0, ev)) {
        return true;
    }

    return false;
}

bool SearchModule::groupMessageEvent(const CoolQ::MessageEvent &ev)
{
    Q_D(SearchModule);

    if (d->search(0, ev.from, ev)) {
        return true;
    }

    return false;
}

bool SearchModule::discussMessageEvent(const CoolQ::MessageEvent &ev)
{
    Q_D(SearchModule);

    if (d->search(ev.from, 0, ev)) {
        return true;
    }

    return false;
}

bool SearchModule::event(QEvent *event)
{
    Q_D(SearchModule);

    if (event->type() == SearchEvent::Search) {
        SearchEvent *sev = reinterpret_cast<SearchEvent *>(event);

        int from = 1;
        int number = 3;

        if ((sev->did != 0) || (sev->gid != 0)) {
        } else {
            number = 3;
        }

        QString keywords = sev->keywords.join('+');

        QString url = d->searchUrl.arg(keywords).arg(from).arg(number);

        if (sev->langSpec == SearchEvent::EnglishOnly)
            url += "&lr=lang_en";
        else if (sev->langSpec == SearchEvent::ChineseOnly)
            url += "&lr=lang_zh-CN";
        if (!sev->siteSpec.isEmpty())
            url += "&siteSearch=" + sev->siteSpec;

        QNetworkReply *reply = d->network->get(QNetworkRequest(QUrl(url)));
        connect(reply, &QNetworkReply::finished,
                this, [=] { d->parseSearchResult(reply); });

        reply->setProperty("did", sev->did);
        reply->setProperty("gid", sev->gid);
        reply->setProperty("uid", sev->uid);
        reply->setProperty("keywords", sev->keywords.join(' '));
        reply->setProperty("siteSpec", sev->siteSpec);

        // qInfo() << sev->did << sev->gid << sev->uid << sev->keywords;
    }

    return ServiceModule::event(event);
}

// class SearchModulePrivate

SearchModulePrivate::SearchModulePrivate()
    : network(nullptr)
{
    searchUrl = "https://www.googleapis.com/customsearch/v1?key=AIzaSyA1LRDb7Y2JnuQA_2wbXxm4zxsL1F2N1T0&cx=014406374443882396406:gkfrjqgu-08" \
                "&c2coff=1&hl=zh-CN&gl=countryCN&googlehost=google.com.hk&safe=high" \
                "&q=%1&start=%2&num=%3";
}

SearchModulePrivate::~SearchModulePrivate()
{
}

bool SearchModulePrivate::search(qint64 did, qint64 gid, const CoolQ::MessageEvent &ev)
{
    Q_Q(SearchModule);

    QStringList keywords;
    SearchEvent::LangSpec langSpec = SearchEvent::NoLangSpec;
    QString     siteSpec;

    bool banned = false;

    if (ev.equals(6, 0x20)
            && (ev.equals(0, 0x73) || ev.equals(0, 0x53))    // s S
            && (ev.equals(0, 0x73) || ev.equals(0, 0x53))) { // s S
        keywords = CoolQ::trGbk(&ev.gbkMsg[7]).split(' ', QString::SkipEmptyParts);
        banned = true;
    } else if (ev.equals(4, 0x20)
               && ev.equals(0, 0xcb) && ev.equals(1, 0xd1)   // Sou
               && ev.equals(2, 0xcb) && ev.equals(3, 0xf7)) {// Suo
        keywords = CoolQ::trGbk(&ev.gbkMsg[5]).split(' ', QString::SkipEmptyParts);
        banned = true;
    } else if (ev.equals(2, 0x20)
               && (ev.equals(0, 0x65) || ev.equals(0, 0x45))    // e E
               && (ev.equals(1, 0x73) || ev.equals(1, 0x53))) { // s S
        keywords = CoolQ::trGbk(&ev.gbkMsg[3]).split(' ', QString::SkipEmptyParts);
        langSpec = SearchEvent::EnglishOnly;
        banned = true;
    } else if (ev.equals(8, 0x20)
               && ev.equals(0, 0xd3) && ev.equals(1, 0xa2)   // Ying
               && ev.equals(2, 0xce) && ev.equals(3, 0xc4)   // Wen
               && ev.equals(4, 0xcb) && ev.equals(5, 0xd1)   // Sou
               && ev.equals(6, 0xcb) && ev.equals(7, 0xf7)) {// Suo
        keywords = CoolQ::trGbk(&ev.gbkMsg[9]).split(' ', QString::SkipEmptyParts);
        langSpec = SearchEvent::EnglishOnly;
        banned = true;
    } else if (ev.equals(2, 0x20)
               && (ev.equals(0, 0x63) || ev.equals(0, 0x43))    // c C
               && (ev.equals(1, 0x73) || ev.equals(1, 0x53))) { // s S
        keywords = CoolQ::trGbk(&ev.gbkMsg[3]).split(' ', QString::SkipEmptyParts);
        langSpec = SearchEvent::ChineseOnly;
        banned = true;
    } else if (ev.equals(8, 0x20)
               && ev.equals(0, 0xd6) && ev.equals(1, 0xd0)   // Zhong
               && ev.equals(2, 0xce) && ev.equals(3, 0xc4)   // Wen
               && ev.equals(4, 0xcb) && ev.equals(5, 0xd1)   // Sou
               && ev.equals(6, 0xcb) && ev.equals(7, 0xf7)) {// Suo
        keywords = CoolQ::trGbk(&ev.gbkMsg[9]).split(' ', QString::SkipEmptyParts);
        langSpec = SearchEvent::ChineseOnly;
        banned = true;
    } else if (ev.equals(2, 0x20)
               && (ev.equals(0, 0x71) || ev.equals(0, 0x51))    // q Q
               && (ev.equals(1, 0x73) || ev.equals(1, 0x53))) { // s S
        keywords = CoolQ::trGbk(&ev.gbkMsg[3]).split(' ', QString::SkipEmptyParts);
        siteSpec = "qtdevs.org";
    } else if (ev.equals(8, 0x20)
               && ev.equals(0, 0xc9) && ev.equals(1, 0xe7)   // She
               && ev.equals(2, 0xc7) && ev.equals(3, 0xf8)   // Qu
               && ev.equals(4, 0xcb) && ev.equals(5, 0xd1)   // Sou
               && ev.equals(6, 0xcb) && ev.equals(7, 0xf7)) {// Suo
        keywords = CoolQ::trGbk(&ev.gbkMsg[9]).split(' ', QString::SkipEmptyParts);
        siteSpec = "qtdevs.org";
    } else if (ev.equals(2, 0x20)
               && (ev.equals(0, 0x67) || ev.equals(0, 0x47))    // g G
               && (ev.equals(1, 0x73) || ev.equals(1, 0x53))) { // s S
        keywords = CoolQ::trGbk(&ev.gbkMsg[3]).split(' ', QString::SkipEmptyParts);
        siteSpec = QLatin1Literal("qtdebug.com");
    } else if (ev.equals(8, 0x20)
               && ev.equals(0, 0xb6) && ev.equals(1, 0xfe)   // Er
               && ev.equals(2, 0xb9) && ev.equals(3, 0xb7)   // Gou
               && ev.equals(4, 0xcb) && ev.equals(5, 0xd1)   // Sou
               && ev.equals(6, 0xcb) && ev.equals(7, 0xf7)) {// Suo
        keywords = CoolQ::trGbk(&ev.gbkMsg[9]).split(' ', QString::SkipEmptyParts);
        siteSpec = QLatin1Literal("qtdebug.com");
    }

    if (banned) {
        QString text = SearchModule::tr("Banned in law.");
        if (did != 0) {
            q->sendDiscussMessage(did, q->at(ev.sender) + " " + text);
        } else if (gid != 0) {
            q->sendGroupMessage(gid, q->at(ev.sender) + " " + text);
        } else {
            q->sendPrivateMessage(ev.sender, text);
        }
        return true;
    }

    if (!keywords.isEmpty()) {
        qApp->postEvent(q, new SearchEvent(did, gid, ev.sender,
                                           keywords, langSpec, siteSpec));
        return true;
    }

    return false;
}

void SearchModulePrivate::parseSearchResult(QNetworkReply *reply)
{
    Q_Q(SearchModule);

    qint64 did = reply->property("did").toLongLong();
    qint64 gid = reply->property("gid").toLongLong();
    qint64 uid = reply->property("uid").toLongLong();
    QString keywords = reply->property("keywords").toString();
    QString siteSpec = reply->property("siteSpec").toString();

    QString msg;
    QTextStream s(&msg);

    if (reply->error() != QNetworkReply::NoError) {
        s << SearchModule::tr("Sorry, Google already banned by GFW.");
        qInfo() << reply->error() << reply->errorString();
    } else {
        auto doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject result = doc.object();

        if (result.contains("error")) {
            s << SearchModule::tr("Sorry, Error via %1.").arg(keywords);
        } else {
            QJsonArray topics = result.value("items").toArray();
            if (topics.count() == 0) {
                s << SearchModule::tr("Sorry, Not found via %1.").arg(keywords);
            } else {
                s << SearchModule::tr("Keywords: ") << keywords << endl << endl;

                for (int i = 0; i < topics.count(); ++i) {
                    QJsonObject item = topics.at(i).toObject();
                    s << q->cqEmoji(128640) << item.value("title").toString() << endl;
                    QString snippet = item.value("snippet").toString();
                    if (!snippet.isEmpty()) {
                        snippet.remove("\n");
                        s << snippet << endl;
                    }
                    s << item.value("link").toString() << endl;
                }

                /*
                if (siteSpec == QLatin1Literal("qtdebug.com")) {
                    s << SearchModule::tr("Donate ErGou.") << q->cqEmoji(128054);
                }
                */
            }
        }
    }

    s.flush();

    if (did != 0) {
        q->sendDiscussMessage(did, q->at(uid) + " " + SearchModule::tr("The result already sent in private."));
    } else if (gid != 0) {
        q->sendGroupMessage(gid, q->at(uid) + " " + SearchModule::tr("The result already sent in private."));
    }

    q->sendPrivateMessage(uid, msg);
}
