#ifndef SEARCHMODULE_H
#define SEARCHMODULE_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include "CqServiceModule.h"

class SearchModulePrivate;
class SearchModule : public CoolQ::ServiceModule
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SearchModule)

public:
    explicit SearchModule(CoolQ::ServiceEngine *parent = nullptr);
    virtual ~SearchModule();

public:
    bool privateMessageEvent(const CoolQ::MessageEvent &ev);
    bool groupMessageEvent(const CoolQ::MessageEvent &ev);
    bool discussMessageEvent(const CoolQ::MessageEvent &ev);

private:
    bool event(QEvent *event) final;
};

#endif // SEARCHMODULE_H
