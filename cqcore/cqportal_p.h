#ifndef CQPORTAL_P_H
#define CQPORTAL_P_H

#include <QThread>

#include "cqportal.h"

// class CqThread

class CqThread : public QThread
{
public:
    explicit CqThread(CqPortal *portal);
private:
    void run() Q_DECL_OVERRIDE;
private:
    CqPortal *portal;
};

// class CqPortalPrivate

class CqPortalPrivate : public QObject
{
    Q_OBJECT
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

public Q_SLOTS:
    void initThread();
    void cleanup();
private:
    QThread *agent;

private:
    qint64  currentId;
    QString basePath;
    QString imagePath;
};

#endif // CQPORTAL_P_H
