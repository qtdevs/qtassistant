#ifndef DONATEMODULE_H
#define DONATEMODULE_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "cqmodule.h"

class DonateModulePrivate;
class DonateModule : public CqModule
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DonateModule)

public:
    explicit DonateModule(CqEngine *parent = nullptr);
    virtual ~DonateModule();

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
