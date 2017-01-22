#ifndef CQPORTAL_P_H
#define CQPORTAL_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "cqportal.h"

class CqPortalPrivate
{
    Q_DECLARE_PUBLIC(CqPortal)

public:
    CqPortalPrivate();
    virtual ~CqPortalPrivate();
protected:
    CqPortal *q_ptr;

public:
    static CqPortal::Result result(qint32 r);
public:
    static qint32 accessToken;
    static CqPortal *instance;

private:
    qint64  currentId;
    QString basePath;
    QString imagePath;
};

#endif // CQPORTAL_P_H
