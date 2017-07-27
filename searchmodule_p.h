#ifndef SEARCHMODULE_P_H
#define SEARCHMODULE_P_H

#include <QNetworkAccessManager>

#include "searchmodule.h"

class SearchModulePrivate
{
    Q_DECLARE_PUBLIC(SearchModule)

public:
    SearchModulePrivate();
    virtual ~SearchModulePrivate();
protected:
    SearchModule *q_ptr;

protected:
    QString apiKey;
    QString apiUsername;
    QNetworkAccessManager *network;

protected:
    QString searchBaseUrl;
};

#endif // SEARCHMODULE_P_H
