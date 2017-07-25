#ifndef DISCOURSEAPI_P_H
#define DISCOURSEAPI_P_H

#include <QNetworkAccessManager>

#include "discourseapi.h"

class DiscourseApiPrivate
{
    Q_DECLARE_PUBLIC(DiscourseApi)

public:
    DiscourseApiPrivate();
    virtual ~DiscourseApiPrivate();
protected:
    DiscourseApi *q_ptr;

protected:
    QString apiKey;
    QString apiUsername;
    QNetworkAccessManager *network;

protected:
    QString searchBaseUrl;
};

#endif // DISCOURSEAPI_P_H
