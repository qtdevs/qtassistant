#ifndef QTASSISTANT_H
#define QTASSISTANT_H

#include "cqportal.h"

// class QtAssistant

class QtAssistantPrivate;
class QtAssistant : public CqPortal
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QtAssistant)

public:
    explicit QtAssistant(QObject *parent = Q_NULLPTR);
    virtual ~QtAssistant();

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

private:
    void timerEvent(QTimerEvent *) Q_DECL_FINAL;
};

#endif // QTASSISTANT_H
