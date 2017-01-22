#ifndef HTMLFEEDBACK_P_H
#define HTMLFEEDBACK_P_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include <QPixmap>

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

    QPixmap primaryImage;
    QPixmap dangerImage;
    QPixmap warningImage;
    QPixmap promptImage;
    QPixmap successImage;
};

#endif // HTMLFEEDBACK_P_H
