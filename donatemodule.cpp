#include "donatemodule.h"
#include "donatemodule_p.h"

#include <QFileInfo>
#include <QTextStream>

#include "managemodule_p.h"

// class DonateModule

DonateModule::DonateModule(CqEngine *parent)
    : CqModule(parent)
    , d_ptr(new DonateModulePrivate())
{
    d_ptr->q_ptr = this;

    Q_ASSERT(DonateModulePrivate::instance == nullptr);
    DonateModulePrivate::instance = this;

    d_ptr->ergouString = tr("Donate ErGou");
    d_ptr->gougeString = tr("Donate GouGe");
    d_ptr->maodaString = tr("Donate MaoDa");
}

DonateModule::~DonateModule()
{
    DonateModulePrivate::instance = nullptr;
}

DonateModule *DonateModule::instance()
{
    return DonateModulePrivate::instance;
}

bool DonateModule::privateMessageEvent(const MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool DonateModule::groupMessageEvent(const MessageEvent &ev)
{
    Q_D(DonateModule);

    if (strlen(ev.gbkMsg) < 30) {
        QString msgs = convert(ev.gbkMsg);
        QStringList args = msgs.split(' ', QString::SkipEmptyParts);

        QList<qint64> uids;

        QString cmd = args[0];
        if (cmd.startsWith(tr("Donate"))) {
            if (msgs == d->ergouString || msgs == d->gougeString) {
                donateMember(ev.from, 26664141);
                return true;
            } else if (msgs == d->maodaString) {
                donateMember(ev.from, 215688650);
                return true;
            }

            if (cmd.startsWith(tr("DonateMe"))) {
                uids.append(ev.sender);
            } else {
                args = msgs.mid(tr("Donate").length()).split(' ', QString::SkipEmptyParts);
                LevelInfoList ll = ManageModulePrivate::findUsers(args);
                for (auto li : ll)
                    uids.append(li.uid);
            }

            if (!uids.isEmpty()) {
                for (auto uid : uids) {
                    donateMember(ev.from, uid);
                }
            } else {

            }

            return true;
        }
    }
    return false;

    Q_UNUSED(ev);
    return false;
}

bool DonateModule::discussMessageEvent(const MessageEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

void DonateModule::donateMember(qint64 gid, qint64 uid)
{
    QDateTime now = QDateTime::currentDateTime();

    CqMemberInfo mi = memberInfo(gid, uid);
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
            ts << "\r\n" << tr("The Below Pay Info ") << mi.nameCard() << tr(".");
            ts << "\r\n" << tr("Other members contact iMaodA.");
        }
        ts.flush();

        sendGroupMessage(gid, outMsg);
    } else {
        sendGroupMessage(gid, cqAt(uid) + tr("JoinTime >= 90."));
    }
}

// class DonateModulePrivate

DonateModule *DonateModulePrivate::instance = nullptr;

DonateModulePrivate::DonateModulePrivate()
    : q_ptr(nullptr)
{
}

DonateModulePrivate::~DonateModulePrivate()
{
}
