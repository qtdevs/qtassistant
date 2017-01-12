#ifndef HTMLFEEDBACK_P_H
#define HTMLFEEDBACK_P_H

#include <QFont>

#include "htmlfeedback.h"

class HtmlFeedbackPrivate
{
    Q_DECLARE_PUBLIC(HtmlFeedback)

public:
    HtmlFeedbackPrivate();
    virtual ~HtmlFeedbackPrivate();
private:
    HtmlFeedback *q_ptr;

public:
    enum Style {
        Primary,
        Danger,
        Warning,
        Prompt,
        Success,
    };

public:
    QImage draw(const QString &html, int width, Style style) const;

private:
    QString primarySheet;
    QString dangerSheet;
    QString warningSheet;
    QString promptSheet;
    QString successSheet;

private:
    QFont awesomeFont;
};

#endif // HTMLFEEDBACK_P_H
