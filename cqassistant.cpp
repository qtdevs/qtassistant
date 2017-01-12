#include "cqassistant.h"
#include "cqassistant_p.h"

#include <QDir>
#include <QThread>
#include <QPixmap>
#include <QStringBuilder>
#include <QTextStream>
#include <QUuid>

#include "cqapi/cqapi.h"

#include "datas/masterlevels.h"
#include "datas/memberwelcome.h"
#include "datas/memberblacklist.h"
#include "datas/memberdeathhouse.h"

#include "htmlfeedback/htmlfeedback.h"

CqCode cqCode(qint32 code)
{
    switch (code) {
    case 1:
        break;
    default:
        return CqCode::Unknown;
    }

    return CqCode::Unknown;
}

// class CqEncoder

QString CqEncoder::at(qint64 uid)
{
    return QString::fromLatin1("[CQ:at,qq=%1]").arg(uid);
}

QString CqEncoder::image(const QString &name)
{
    return QString::fromLatin1("[CQ:image,file=%1]").arg(name);
}

// class CqAssistant

CqAssistant::CqAssistant(qint32 token, QObject *parent)
    : QObject(parent)
    , d_ptr(new CqAssistantPrivate())
{
    d_ptr->q_ptr = this;
    d_ptr->token = token;
    d_ptr->initThread();

    d_ptr->currentId = CQ_getLoginQQ(token);
    QString path = conv(CQ_getAppDirectory(token));
    d_ptr->basePath = QDir::cleanPath(path);
}

CqAssistant::~CqAssistant()
{
    Q_D(CqAssistant);

    if (d->agent->isRunning()) {
        d->agent->quit();
        d->agent->wait();
    }
}

QByteArray CqAssistant::conv(const QString &str)
{
    return str.toLocal8Bit();
}

QString CqAssistant::conv(const char *gbkStr)
{
    return QString::fromLocal8Bit(gbkStr);
}

QString CqAssistant::conv(const QByteArray &str)
{
    return QString::fromLocal8Bit(str);
}

qint64 CqAssistant::currentId() const
{
    return d_ptr->currentId;
}

QString CqAssistant::basePath() const
{
    return d_ptr->basePath;
}

QString CqAssistant::appFilePath(const char *srcName) const
{
    if ((srcName[0] == '/') || (srcName[0] == '\\')) {
        return QDir::cleanPath(d_ptr->basePath % QString::fromUtf8(srcName));
    }

    return QDir::cleanPath(d_ptr->basePath % QStringLiteral("/") % QString::fromUtf8(srcName));
}

QString CqAssistant::appFilePath(const QString &name) const
{
    if (name.startsWith(QLatin1Char('/')) || name.startsWith(QLatin1Char('\\'))) {
        return QDir::cleanPath(d_ptr->basePath % name);
    }

    return QDir::cleanPath(d_ptr->basePath % QStringLiteral("/") % name);
}

QString CqAssistant::usrFilePath(const char *srcName) const
{
    QString currentId = QString::number(d_ptr->currentId);

    if ((srcName[0] == '/') || (srcName[0] == '\\')) {
        return QDir::cleanPath(d_ptr->basePath % "/" % currentId % QString::fromUtf8(srcName));
    }

    return QDir::cleanPath(d_ptr->basePath % "/" % currentId % "/" % QString::fromUtf8(srcName));
}

QString CqAssistant::usrFilePath(const QString &name) const
{
    QString currentId = QString::number(d_ptr->currentId);

    if (name.startsWith(QLatin1Char('/')) || name.startsWith(QLatin1Char('\\'))) {
        return QDir::cleanPath(d_ptr->basePath % "/" % currentId % name);
    }

    return QDir::cleanPath(d_ptr->basePath % "/" % currentId % "/" % name);
}

CqCode CqAssistant::sendPrivateMessage(qint64 uid, const char *gbkMsg) const
{
    return cqCode(CQ_sendPrivateMsg(d_ptr->token, uid, gbkMsg));
}

CqCode CqAssistant::sendGroupMessage(qint64 gid, const char *gbkMsg) const
{
    return cqCode(CQ_sendGroupMsg(d_ptr->token, gid, gbkMsg));
}

CqCode CqAssistant::sendDiscussMessage(qint64 did, const char *gbkMsg) const
{
    return cqCode(CQ_sendDiscussMsg(d_ptr->token, did, gbkMsg));
}

CqCode CqAssistant::sendPrivateMessage(qint64 uid, const QString &msg) const
{
    return sendPrivateMessage(uid, conv(msg).constData());
}

CqCode CqAssistant::sendGroupMessage(qint64 gid, const QString &msg) const
{
    return sendGroupMessage(gid, conv(msg).constData());
}

CqCode CqAssistant::sendDiscussMessage(qint64 did, const QString &msg) const
{
    return sendDiscussMessage(did, conv(msg).constData());
}

CqCode CqAssistant::banGroupMember(qint64 gid, qint64 uid, int duration)
{
    return cqCode(CQ_setGroupBan(d_ptr->token, gid, uid, duration));
}

CqCode CqAssistant::kickGroupMember(qint64 gid, qint64 uid, bool lasting)
{
    return cqCode(CQ_setGroupKick(d_ptr->token, gid, uid, lasting));
}

CqCode CqAssistant::adminGroupMember(qint64 gid, qint64 uid, bool enabled)
{
    return cqCode(CQ_setGroupAdmin(d_ptr->token, gid, uid, enabled));
}

CqCode CqAssistant::renameGroupMember(qint64 gid, qint64 uid, const char *gbkNewNickName)
{
    return cqCode(CQ_setGroupCard(d_ptr->token, gid, uid, gbkNewNickName));
}

CqCode CqAssistant::renameGroupMember(qint64 gid, qint64 uid, const QString &newNameCard)
{
    return cqCode(CQ_setGroupCard(d_ptr->token, gid, uid, conv(newNameCard).constData()));
}

CqCode CqAssistant::acceptRequest(const char *gbkTag)
{
    return cqCode(CQ_setFriendAddRequest(d_ptr->token, gbkTag, REQUEST_ALLOW, ""));
}

CqCode CqAssistant::rejectRequest(const char *gbkTag)
{
    return cqCode(CQ_setFriendAddRequest(d_ptr->token, gbkTag, REQUEST_DENY, ""));
}

CqCode CqAssistant::acceptRequest(qint32 type, const char *gbkTag)
{
    if (1 == type) {
        return cqCode(CQ_setGroupAddRequestV2(d_ptr->token, gbkTag, REQUEST_GROUPADD, REQUEST_ALLOW, ""));
    } else if (2 == type) {
        return cqCode(CQ_setGroupAddRequestV2(d_ptr->token, gbkTag, REQUEST_GROUPINVITE, REQUEST_ALLOW, ""));
    }

    return CqCode::Unknown;
}

CqCode CqAssistant::rejectRequest(qint32 type, const char *gbkTag)
{
    if (1 == type) {
        return cqCode(CQ_setGroupAddRequestV2(d_ptr->token, gbkTag, REQUEST_GROUPADD, REQUEST_DENY, ""));
    } else if (2 == type) {
        return cqCode(CQ_setGroupAddRequestV2(d_ptr->token, gbkTag, REQUEST_GROUPINVITE, REQUEST_DENY, ""));
    }

    return CqCode::Unknown;
}

CqCode CqAssistant::leaveGroup(qint64 gid)
{
    return cqCode(CQ_setGroupLeave(d_ptr->token, gid, false));
}

CqCode CqAssistant::leaveDiscuss(qint64 did)
{
    return cqCode(CQ_setDiscussLeave(d_ptr->token, did));
}

CqCode CqAssistant::mute(qint64 gid, bool muted)
{
    return cqCode(CQ_setGroupWholeBan(d_ptr->token, gid, muted));
}

MemberInfo CqAssistant::memberInfo(qint64 gid, qint64 uid, bool cache)
{
    return MemberInfo(CQ_getGroupMemberInfoV2(d_ptr->token, gid, uid, !cache));
}

PersonInfo CqAssistant::personInfo(qint64 uid, bool cache)
{
    return PersonInfo(CQ_getStrangerInfo(d_ptr->token, uid, !cache));
}

QString CqAssistant::saveImage(const QImage &p) const
{
    Q_D(const CqAssistant);

    QString uuid = QString::fromLatin1(QUuid::createUuid().toRfc4122().toHex());
    QString name = QString("%1/%2.png").arg(d->imagePath, uuid);
    if (p.save(name, "PNG")) {
        return uuid % ".png";
    }

    return QString();
}

QImage CqAssistant::loadImage(const QString &f) const
{
    Q_UNUSED(f);
    return QImage();
}

// class CqAssistantPrivate

CqAssistantPrivate::CqAssistantPrivate()
    : q_ptr(Q_NULLPTR)
    , agent(Q_NULLPTR)
    , token(-1)
    , currentId(0)
    , levels(Q_NULLPTR)
    , welcome(Q_NULLPTR)
    , blacklist(Q_NULLPTR)
    , deathHouse(Q_NULLPTR)
    , htmlFeedback(Q_NULLPTR)
{
}

CqAssistantPrivate::~CqAssistantPrivate()
{
}

void CqAssistantPrivate::initThread()
{
    agent = new QThread(this);

    moveToThread(agent);
    connect(agent, &QThread::started,
            this, &CqAssistantPrivate::initialize);

    agent->start();
}

void CqAssistantPrivate::initialize()
{
    if (currentId == 0) {
        return;
    }

    QString path = basePath % "/" % QString::number(currentId);
    SqlData::initPath(QDir::cleanPath(path));
    QDir().mkpath(path);

    levels = new MasterLevels(this);
    welcome = new MemberWelcome(this);
    blacklist = new MemberBlacklist(this);
    deathHouse = new MemberDeathHouse(this);

    htmlFeedback = new HtmlFeedback(this);
    imagePath = QDir::cleanPath(basePath % "/../../data/image");

    startTimer(10000);
}

#include <QDateTime>

void CqAssistantPrivate::timerEvent(QTimerEvent *)
{
    Q_Q(CqAssistant);

    auto welcome = this->welcome->welcome();
    if (!welcome.isEmpty()) {
        qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
        QHashIterator<Member, qint64> i(welcome);
        while (i.hasNext()) {
            i.next();
            if ((i.value() + 1800000) < now) {
                this->welcome->removeMember(i.key().first, i.key().second);
                q->kickGroupMember(i.key().first, i.key().second, false);
                QString msg = "Killed: " + QString::number(i.key().second);
                q->sendGroupMessage(i.key().first, msg);
            }
        }
    }

    auto deaths = deathHouse->deathHouse();
    if (!deaths.isEmpty()) {
        qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
        QHashIterator<Member, qint64> i(deaths);
        while (i.hasNext()) {
            i.next();
            if ((i.value() + 360000) < now) {
                deathHouse->removeMember(i.key().first, i.key().second);
                q->kickGroupMember(i.key().first, i.key().second, false);
                QString msg = "Killed: " + QString::number(i.key().second);
                q->sendGroupMessage(i.key().first, msg);
            }
        }
    }
}

void CqAssistantPrivate::permissionDenied(qint64 gid, qint64 uid, MasterLevel level, const QString &reason)
{
    Q_UNUSED(uid);

    Q_Q(CqAssistant);

    QString content = reason.isEmpty() ? tr("As %1, you have no rights.").arg(MasterLevels::levelName(level)) : reason;
    QString html = QString("<html><body><span class=\"t\">%1</span><p class=\"c\">%2</p></body></html>").arg(tr("Permission Denied"), content);

    QImage feedback = htmlFeedback->drawDanger(html, 400);
    QString fileName = q->saveImage(feedback);
    q->sendGroupMessage(gid, image(fileName));
}

void CqAssistantPrivate::showPrimaryList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Primary);
}

void CqAssistantPrivate::showDangerList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Danger);
}

void CqAssistantPrivate::showWarningList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Warning);
}

void CqAssistantPrivate::showPromptList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Prompt);
}

void CqAssistantPrivate::showSuccessList(qint64 gid, const QString &title, const LevelInfoList &members, bool level)
{
    feedbackList(gid, title, members, level, HtmlFeedback::Success);
}

void CqAssistantPrivate::feedbackList(qint64 gid, const QString &title, const LevelInfoList &members, bool level, HtmlFeedback::Style style)
{
    Q_Q(CqAssistant);

    for (int i = 0, part = 0; i < members.count();) {
        QString html;
        do {
            int cc = i + 4;
            QTextStream ds(&html);
            ds << "<html><body><span class=\"t\">" << title;
            if (members.count() > 5) {
                ds << tr("(Part %1)").arg(++part);
            }
            ds << "</span><div>";
            for (; i < members.count(); ++i) {
                const LevelInfo &li = members.at(i);
                MemberInfo mi = q->memberInfo(gid, li.uid);
                ds << "<p class=\"c\">";
                if (level) {
                    ds << MasterLevels::levelName(li.level) << tr(": ");
                }
                if (!mi.nameCard().isEmpty()) {
                    ds << mi.nameCard() << "</p>";
                } else {
                    ds << mi.nickName() << "</p>";
                }
                if (i == cc) {
                    break;
                }
            }
            ds << "</div></body></html>";
        } while (false);

        QImage feedback = htmlFeedback->draw(html, 400, style);
        QString fileName = q->saveImage(feedback);
        q->sendGroupMessage(gid, image(fileName));
    }
}
