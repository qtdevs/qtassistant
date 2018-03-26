#ifndef DONATEMODULE_H
#define DONATEMODULE_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "CqServiceModule.h"

class DonateModulePrivate;
class DonateModule : public CoolQ::ServiceModule
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DonateModule)

public:
    explicit DonateModule(CoolQ::ServiceEngine *parent = nullptr);
    virtual ~DonateModule();

public:
    static DonateModule *instance();

public:
    bool privateMessageEvent(const CoolQ::MessageEvent &ev);
    bool groupMessageEvent(const CoolQ::MessageEvent &ev);
    bool discussMessageEvent(const CoolQ::MessageEvent &ev);

public:
    void donateMember(qint64 gid, qint64 uid);
};

#endif // DONATEMODULE_H
