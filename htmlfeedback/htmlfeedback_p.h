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
