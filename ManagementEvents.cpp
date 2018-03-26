#include "ManagementModule.h"
#include "ManagementModule_p.h"

#include <QStringBuilder>
#include <QTextStream>
#include <QFileInfo>
#include <QMetaEnum>
#include <QPixmap>
#include <QUuid>
#include <QDir>

#include "sqldatas/MasterLevels.h"
#include "sqldatas/MemberBlacklist.h"
#include "sqldatas/MemberWatchlist.h"

#include "htmlfeedback.h"

// class ManagementModule

bool ManagementModule::masterChangeEvent(const CoolQ::MasterChangeEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool ManagementModule::friendRequestEvent(const CoolQ::FriendRequestEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool ManagementModule::groupRequestEvent(const CoolQ::GroupRequestEvent &ev)
{
    Q_D(ManagementModule);

    if (!d->managedGroups.contains(ev.from)) {
        return false;
    }

    // 黑名单检查，如果发现匹配，直接拒绝。
    if (d->blacklist->contains(ev.from, ev.user)) {
        rejectRequest(ev.type, ev.gbkTag);
        return false;
    }

    return false;
}

bool ManagementModule::friendAddEvent(const CoolQ::FriendAddEvent &ev)
{
    Q_UNUSED(ev);
    return false;
}

bool ManagementModule::memberJoinEvent(const CoolQ::MemberJoinEvent &ev)
{
    Q_D(ManagementModule);

    if (!d->managedGroups.contains(ev.from)) {
        return false;
    }

    // 黑名单检查，如果发现匹配，直接踢出。
    if (d->blacklist->contains(ev.from, ev.member)) {
        kickGroupMember(ev.from, ev.member, false);
        return false;
    }

    // 加入新手监控，监视首次发言。
    d->watchlist->addMember(ev.from, ev.member);

    // 尝试修改昵称。
    QString nameCard;
    bool unknownLocation = false;
    CoolQ::MemberInfo mi = memberInfo(ev.from, ev.member, false);
    if (mi.isValid()) {
        QString nickName = mi.nickName().trimmed();
        QString location = mi.location().trimmed();
        d->safetyNameCard(nickName);

        if (!nickName.isEmpty()) {
            if (location.isEmpty()) {
                unknownLocation = true;
                location = tr("所在地");
            }
            nameCard = '[' % location % ']' % nickName;
            renameGroupMember(ev.from, ev.member, nameCard);
        }
    }

    QString msg;
    QTextStream ts(&msg);

    QFileInfo rootInfo(imgFilePath("Welcomes"));
    if (rootInfo.isDir()) {
        int i = 0;
        QDir root(rootInfo.absoluteFilePath());
        for (auto fileInfo : root.entryInfoList(QDir::Files, QDir::Name)) {
            if (fileInfo.suffix() == "txt") {
                QStringList nameParts = fileInfo.fileName().split('.');
                if (nameParts.count() == 3) {
                    QFile file(fileInfo.absoluteFilePath());
                    if (file.open(QFile::ReadOnly)) {
                        auto html = QString::fromUtf8(file.readAll());
                        auto styleEnum = QMetaEnum::fromType<HtmlFeedback::Style>();
                        auto style = styleEnum.keysToValue(nameParts.at(1).toLatin1());
                        auto image = d->htmlFeedback->drawText(html, 400, (HtmlFeedback::Style)style, ev.from);
                        if (++i > 1) ts << '\n';
                        ts << cqImage(saveImage(image));
                    }
                }
            }
        }
    }

    ts.flush();

    sendGroupMessage(ev.from, at(ev.member));
    sendGroupMessage(ev.from, msg);

    return false;
}

bool ManagementModule::memberLeaveEvent(const CoolQ::MemberLeaveEvent &ev)
{
    Q_D(ManagementModule);

    if (!d->managedGroups.contains(ev.from)) {
        return false;
    }

    // 移出新手监控，不再进行监视。
    d->watchlist->removeMember(ev.from, ev.member);

    return false;
}
