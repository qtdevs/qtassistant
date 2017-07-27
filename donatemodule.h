#ifndef DONATEMODULE_H
#define DONATEMODULE_H

#include "cqmodule.h"

class DonateModulePrivate;
class DonateModule : public CqModule
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DonateModule)

public:
    explicit DonateModule(CqEngine *parent = nullptr);
    virtual ~DonateModule();
protected:
    QScopedPointer<DonateModulePrivate> d_ptr;

public:
    static DonateModule *instance();

public:
    bool privateMessageEvent(const MessageEvent &ev);
    bool groupMessageEvent(const MessageEvent &ev);
    bool discussMessageEvent(const MessageEvent &ev);

public:
    void donateMember(qint64 gid, qint64 uid);
};

#endif // DONATEMODULE_H
