#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include <QFileInfo>
#include <QTextStream>

#include "donatemodule.h"
#include "donatemodule_p.h"

#include "qtassistant_p.h"

// class DonateModule

DonateModule::DonateModule(QObject *parent)
    : QObject(parent)
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

bool DonateModule::groupMessageEventFilter(CqPortal *portal, const MessageEvent &event)
{
    Q_D(DonateModule);

    if (strlen(event.gbkMsg) < 30) {
        QString msgs = portal->convert(event.gbkMsg);
        QStringList args = msgs.split(' ', QString::SkipEmptyParts);

        QList<qint64> uids;

        QString cmd = args[0];
        if (cmd.startsWith(tr("Donate"))) {
            if (msgs == d->ergouString || msgs == d->gougeString) {
                donateMember(portal, event.from, 26664141);
                return true;
            } else if (msgs == d->maodaString) {
                donateMember(portal, event.from, 215688650);
                return true;
            }

            if (cmd.startsWith(tr("DonateMe"))) {
                uids.append(event.sender);
            } else {
                args = msgs.mid(tr("Donate").length()).split(' ', QString::SkipEmptyParts);
                LevelInfoList ll = QtAssistantPrivate::findUsers(args);
                for (auto li : ll)
                    uids.append(li.uid);
            }

            if (!uids.isEmpty()) {
                for (auto uid : uids) {
                    donateMember(portal, event.from, uid);
                }
            } else {

            }

            return true;
        }
    }
    return false;
}

void DonateModule::donateMember(CqPortal *portal, qint64 gid, qint64 uid)
{
    QDateTime now = QDateTime::currentDateTime();

    CqMemberInfo mi = portal->memberInfo(gid, uid);
    int joinTime = mi.joinTime().daysTo(now);
    if (joinTime >= 90) {
        QString alipay = portal->resFilePath(QString::fromLatin1("image/donates/%1-alipay.png").arg(uid));
        QString weixin = portal->resFilePath(QString::fromLatin1("image/donates/%1-weixin.png").arg(uid));

        bool hasAlipayFile = QFileInfo::exists(alipay);
        bool hasWeixinFile = QFileInfo::exists(weixin);

        QString outMsg;
        QTextStream ts(&outMsg);
        if (!hasAlipayFile && !hasWeixinFile) {
            ts << tr("No Resource Files.");
        } else {
            if (hasAlipayFile)
                ts << portal->cqImage(QString::fromLatin1("donates\\%1-alipay.png").arg(uid));
            if (hasWeixinFile)
                ts << portal->cqImage(QString::fromLatin1("donates\\%1-weixin.png").arg(uid));
            ts << "\r\n" << tr("The Below Pay Info ") << mi.nameCard() << tr(".");
            ts << "\r\n" << tr("Other members contact iMaodA.");
        }
        ts.flush();

        portal->sendGroupMessage(gid, outMsg);
    } else {
        portal->sendGroupMessage(gid, portal->cqAt(uid) + tr("JoinTime >= 90."));
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
