#include "donatemodule.h"
#include "donatemodule_p.h"

#include <QFileInfo>
#include <QTextStream>

#include "ManagementModule_p.h"

// class DonateModule

DonateModule::DonateModule(CoolQ::ServiceEngine *parent)
    : CoolQ::ServiceModule(*new DonateModulePrivate(), parent)
{
    Q_D(DonateModule);

    Q_ASSERT(DonateModulePrivate::instance == nullptr);
    DonateModulePrivate::instance = this;

    d->ergouString = tr("Donate ErGou");
    d->gougeString = tr("Donate GouGe");
    d->maodaString = tr("Donate MaoDa");
}

DonateModule::~DonateModule()
{
    DonateModulePrivate::instance = nullptr;
}

DonateModule *DonateModule::instance()
{
    return DonateModulePrivate::instance;
}

bool DonateModule::privateMessageEvent(const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool DonateModule::groupMessageEvent(const CoolQ::MessageEvent &ev)
{
    if (ev.equals(0, 0xb4) && ev.equals(1, 0xf2) // DaShang
            && ev.equals(2, 0xc9) && ev.equals(3, 0xcd)) {

        if (ev.equals(4, 0xc3) && ev.equals(5, 0xa8) // MaoDa
                && ev.equals(6, 0xb4) && ev.equals(7, 0xf3)) {
            donateMember(ev.from, 215688650);
            return true;
        }
        if (ev.equals(4, 0xb6) && ev.equals(5, 0xfe) // ErGou
                && ev.equals(6, 0xb9) && ev.equals(7, 0xb7)) {
            donateMember(ev.from,  26664141);
            return true;
        }

        QList<qint64> uids;

        if (ev.equals(4, 0xce) && ev.equals(5, 0xd2)) { // DaShangWo
            donateMember(ev.from, ev.sender);
            return true;
        } else {
            QStringList args = CoolQ::trGbk(&ev.gbkMsg[4]).split(' ', QString::SkipEmptyParts);
            for (auto li : ManagementModulePrivate::findUsers(args)) {
                donateMember(ev.from, li.uid);
            }
            return true;
        }
    }

    return false;
}

bool DonateModule::discussMessageEvent(const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

void DonateModule::donateMember(qint64 gid, qint64 uid)
{
    Q_D(DonateModule);

    QMutexLocker locker(&d->safeGuard);
    QDateTime lastSent = d->lastSents.value(uid);
    QDateTime now = QDateTime::currentDateTime();
    if (lastSent.isValid() && (lastSent.secsTo(now) < 60)) {
        if (++d->retryCounters[uid] > 3) {
            banGroupMember(gid, uid, 600);
        }
        return;
    } else {
        d->lastSents.insert(uid, now);
        d->retryCounters.remove(uid);
    }
    locker.unlock();

    CoolQ::MemberInfo mi = memberInfo(gid, uid);
    int joinTime = mi.joinTime().daysTo(now);
    if (joinTime >= 90) {
        QString alipay = resFilePath(QString::fromLatin1("image/donates/%1-alipay.png").arg(uid));
        QString weixin = resFilePath(QString::fromLatin1("image/donates/%1-weixin.png").arg(uid));

        bool hasAlipayFile = QFileInfo::exists(alipay);
        bool hasWeixinFile = QFileInfo::exists(weixin);

        QString outMsg;
        QTextStream ts(&outMsg);
        if (!hasAlipayFile && !hasWeixinFile) {
            ts << tr("No Resource Files.");
        } else {
            if (hasAlipayFile)
                ts << cqImage(QString::fromLatin1("donates\\%1-alipay.png").arg(uid));
            if (hasWeixinFile)
                ts << cqImage(QString::fromLatin1("donates\\%1-weixin.png").arg(uid));
            ts << "\n" << tr("The Below Pay Info ") << mi.nameCard() << tr(".");
            ts << "\n" << tr("Other members contact iMaodA.");
        }
        ts.flush();

        sendGroupMessage(gid, outMsg);
    } else {
        sendGroupMessage(gid, at(uid) + tr("JoinTime >= 90."));
    }
}

// class DonateModulePrivate

DonateModule *DonateModulePrivate::instance = nullptr;

DonateModulePrivate::DonateModulePrivate()
{
}

DonateModulePrivate::~DonateModulePrivate()
{
}
