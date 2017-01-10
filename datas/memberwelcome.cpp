#include "memberwelcome.h"
#include "memberwelcome_p.h"

// class MemberWelcome

MemberWelcome::MemberWelcome(QObject *parent)
    : SqlData(*new MemberWelcomePrivate(), parent)
{
    Q_D(MemberWelcome);

    d->setFileName(QStringLiteral("Welcome.db"));

    d->openDatabase();
}

MemberWelcome::~MemberWelcome()
{
}

// class MemberWelcomePrivate

MemberWelcomePrivate::MemberWelcomePrivate()
{
}

MemberWelcomePrivate::~MemberWelcomePrivate()
{
}
