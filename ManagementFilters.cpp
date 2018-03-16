#include "ManagementFilters.h"

#include "managemodule.h"

/*

ManagementFilters::ManagementFilters(CoolQ::ServiceModule *parent)
    : CoolQ::MessageFilter(parent)
{
}

CoolQ::MessageFilter::Filters ManagementFilters::filters() const
{
    return PrivateFilter | GroupFilter | DiscussFilter;
}

QStringList ManagementFilters::keywords() const
{
    QStringList keywords;

    keywords << tr("xxxx");

    return keywords;
}

bool ManagementFilters::privateMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

bool ManagementFilters::groupMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

bool ManagementFilters::discussMessageFilter(int i, const CoolQ::MessageEvent &ev)
{
    Q_UNUSED(i);
    Q_UNUSED(ev);

    return false;
}

*/

// class RestartComputer

RestartComputer::RestartComputer(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters RestartComputer::filters() const
{
    return PrivateFilter;
}

QStringList RestartComputer::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"重启系统");
    keywords << QString(u8"系统重启");
    keywords << QString(u8"重启电脑");
    keywords << QString(u8"电脑重启");

    return keywords;
}

bool RestartComputer::privateMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManageModule *>(module())) {
        if (MasterLevel::ATField == mm->level(0, ev.sender)) {
            mm->sendPrivateMessage(ev.sender, QString(u8"系统即将重启..."));
        }
    }

    return true;
}

// class CreateStartupShortcut

CreateStartupShortcut::CreateStartupShortcut(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters CreateStartupShortcut::filters() const
{
    return PrivateFilter;
}

QStringList CreateStartupShortcut::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"启动登录启动");
    keywords << QString(u8"启动自动启动");
    keywords << QString(u8"开启登录启动");
    keywords << QString(u8"开启自动启动");
    keywords << QString(u8"禁用登录启动");
    keywords << QString(u8"禁用自动启动");

    return keywords;
}

bool CreateStartupShortcut::privateMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManageModule *>(module())) {
        if (MasterLevel::ATField == mm->level(0, ev.sender)) {
            mm->sendPrivateMessage(ev.sender, QString(u8"已经开启登录启动"));
        }
    }

    return true;
}

// class DeleteStartupShortcut

DeleteStartupShortcut::DeleteStartupShortcut(ServiceModule *parent)
    : MessageFilter(parent)
{
}

MessageFilter::Filters DeleteStartupShortcut::filters() const
{
    return PrivateFilter;
}

QStringList DeleteStartupShortcut::keywords() const
{
    QStringList keywords;

    keywords << QString(u8"关闭登录启动");
    keywords << QString(u8"关闭自动启动");
    keywords << QString(u8"禁用登录启动");
    keywords << QString(u8"禁用自动启动");

    return keywords;
}

bool DeleteStartupShortcut::privateMessageFilter(int i, const MessageEvent &ev)
{
    Q_UNUSED(i);

    if (auto mm = qobject_cast<ManageModule *>(module())) {
        if (MasterLevel::ATField == mm->level(0, ev.sender)) {
            mm->sendPrivateMessage(ev.sender, QString(u8"已经关闭登录启动"));
        }
    }

    return true;
}
