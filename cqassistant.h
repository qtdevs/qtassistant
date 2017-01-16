#ifndef CQASSISTANT_H
#define CQASSISTANT_H

#include "cqportal.h"

// class CqAssistant

class CqAssistantPrivate;
class CqAssistant : public CqPortal
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CqAssistant)

public:
    explicit CqAssistant(QObject *parent = Q_NULLPTR);
    virtual ~CqAssistant();

public:
    void initialize() Q_DECL_FINAL;
    void cleanup() Q_DECL_FINAL;

public:
    bool privateMessageEventFilter(const MessageEvent &ev) Q_DECL_OVERRIDE;
    bool groupMessageEventFilter(const MessageEvent &ev) Q_DECL_OVERRIDE;
    bool discussMessageEventFilter(const MessageEvent &ev) Q_DECL_OVERRIDE;

    bool masterChangeEventFilter(const MasterChangeEvent &ev) Q_DECL_OVERRIDE;
    bool friendRequestEventFilter(const FriendRequestEvent &ev) Q_DECL_OVERRIDE;
    bool groupRequestEventFilter(const GroupRequestEvent &ev) Q_DECL_OVERRIDE;

    bool friendAddEventFilter(const FriendAddEvent &ev) Q_DECL_OVERRIDE;
    bool memberJoinEventFilter(const MemberJoinEvent &ev) Q_DECL_OVERRIDE;
    bool memberLeaveEventFilter(const MemberLeaveEvent &ev) Q_DECL_OVERRIDE;
};

#endif // CQASSISTANT_H
