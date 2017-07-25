#ifndef DONATEMODULE_H
#define DONATEMODULE_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include <QObject>
#include "cqportal.h"

class DonateModulePrivate;
class DonateModule : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DonateModule)

public:
    explicit DonateModule(QObject *parent = nullptr);
    virtual ~DonateModule();
protected:
    QScopedPointer<DonateModulePrivate> d_ptr;

public:
    static DonateModule *instance();

public:
    bool groupMessageEventFilter(CqPortal *portal, const MessageEvent &event);

public:
    void donateMember(CqPortal *portal, qint64 gid, qint64 uid);
};

#endif // DONATEMODULE_H
