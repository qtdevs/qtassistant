#ifndef HTMLDRAW_P_H
#define HTMLDRAW_P_H

#include <QHash>
#include <QPixmap>
#include "HtmlDraw.h"

class HtmlDrawPrivate
{
    Q_DECLARE_PUBLIC(HtmlDraw)

public:
    HtmlDrawPrivate();
    virtual ~HtmlDrawPrivate();
protected:
    HtmlDraw *q_ptr;

public:
    static HtmlDrawPrivate *instance;

public:
    void updateMaterialData(const QString &path);
    QPixmap drawText(const QString &text, HtmlDraw::Style style,
                     int width, qint64 theme = 0) const;
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

public:

};

#endif // HTMLDRAW_P_H
