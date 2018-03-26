#ifndef HTMLFEEDBACK_P_H
#define HTMLFEEDBACK_P_H

#include <QHash>
#include <QPixmap>

#include "htmlfeedback.h"

class HtmlFeedbackPrivate
{
    Q_DECLARE_PUBLIC(HtmlFeedback)

public:
    HtmlFeedbackPrivate();
    virtual ~HtmlFeedbackPrivate();
protected:
    HtmlFeedback *q_ptr;

public:
    QString readCssFile(const QString &fileName);

private:
    QString primarySheet;
    QString dangerSheet;
    QString warningSheet;
    QString promptSheet;
    QString successSheet;

private:
    QPixmap backgroundImage;
    QPixmap foregroundImage;

private:
    QPixmap primaryImage;
    QPixmap dangerImage;
    QPixmap warningImage;
    QPixmap promptImage;
    QPixmap successImage;

private:
    QHash<qint64, QString> primarySheets;
    QHash<qint64, QString> dangerSheets;
    QHash<qint64, QString> warningSheets;
    QHash<qint64, QString> promptSheets;
    QHash<qint64, QString> successSheets;

private:
    QHash<qint64, QPixmap> backgroundImages;
    QHash<qint64, QPixmap> foregroundImages;

private:
    QHash<qint64, QPixmap> primaryImages;
    QHash<qint64, QPixmap> dangerImages;
    QHash<qint64, QPixmap> warningImages;
    QHash<qint64, QPixmap> promptImages;
    QHash<qint64, QPixmap> successImages;
};

#endif // HTMLFEEDBACK_P_H
