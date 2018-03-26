#include "HtmlDraw.h"
#include "HtmlDraw_p.h"

#include <QTextDocument>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <QtDebug>

// class HtmlDraw

HtmlDraw::HtmlDraw(const QString &path, QObject *parent)
    : QObject(parent)
    , d_ptr(new HtmlDrawPrivate())
{
    d_ptr->q_ptr = this;

    d_ptr->updateMaterialData(path);
}

HtmlDraw::~HtmlDraw()
{
}

QPixmap HtmlDraw::drawText(const QString &text, Style style, int width, qint64 theme)
{
    if (nullptr != HtmlDrawPrivate::instance)
        return HtmlDrawPrivate::instance->drawText(text, style, width, theme);

    return QPixmap();
}

QPixmap HtmlDraw::drawPrimaryText(const QString &text, int width, qint64 theme)
{
    return drawText(text, HtmlDraw::Primary, width, theme);
}

QPixmap HtmlDraw::drawDangerText(const QString &text, int width, qint64 theme)
{
    return drawText(text, HtmlDraw::Danger, width, theme);
}

QPixmap HtmlDraw::drawWarningText(const QString &text, int width, qint64 theme)
{
    return drawText(text, HtmlDraw::Warning, width, theme);
}

QPixmap HtmlDraw::drawPromptText(const QString &text, int width, qint64 theme)
{
    return drawText(text, HtmlDraw::Prompt, width, theme);
}

QPixmap HtmlDraw::drawSuccessText(const QString &text, int width, qint64 theme)
{
    return drawText(text, HtmlDraw::Success, width, theme);
}

// class HtmlDrawPrivate

HtmlDrawPrivate *HtmlDrawPrivate::instance = nullptr;

HtmlDrawPrivate::HtmlDrawPrivate()
    : q_ptr(Q_NULLPTR)
{
    Q_CHECK_PTR(nullptr == HtmlDrawPrivate::instance);
    HtmlDrawPrivate::instance = this;

    primarySheet = readCssFile(":/HtmlDraw/css/Primary.css");
    dangerSheet = readCssFile(":/HtmlDraw/css/Danger.css");
    warningSheet = readCssFile(":/HtmlDraw/css/Warning.css");
    promptSheet = readCssFile(":/HtmlDraw/css/Prompt.css");
    successSheet = readCssFile(":/HtmlDraw/css/Success.css");

    backgroundImage.load(":/HtmlDraw/img/Background.png");
    foregroundImage.load(":/HtmlDraw/img/Foreground.png");

    primaryImage.load(":/HtmlDraw/img/Primary.png");
    dangerImage.load(":/HtmlDraw/img/Danger.png");
    warningImage.load(":/HtmlDraw/img/Warning.png");
    promptImage.load(":/HtmlDraw/img/Prompt.png");
    successImage.load(":/HtmlDraw/img/Success.png");
}

HtmlDrawPrivate::~HtmlDrawPrivate()
{
    HtmlDrawPrivate::instance = nullptr;
}

void HtmlDrawPrivate::updateMaterialData(const QString &path)
{
    for (auto fileInfo : QDir(path).entryInfoList(QDir::Dirs)) {
        qint64  key = fileInfo.fileName().toLongLong();
        QString filePath = fileInfo.absoluteFilePath();

        QString primarySheetTemp = readCssFile(filePath + "/Primary.css");
        QString dangerSheetTemp = readCssFile(filePath + "/Danger.css");
        QString warningSheetTemp = readCssFile(filePath + "/Warning.css");
        QString promptSheetTemp = readCssFile(filePath + "/Prompt.css");
        QString successSheetTemp = readCssFile(filePath + "/Success.css");

        QPixmap backgroundImageTemp(filePath + "/Background.png");
        QPixmap foregroundImageTemp(filePath + "/Foreground.png");

        QPixmap primaryImageTemp(filePath + "/Primary.png");
        QPixmap dangerImageTemp(filePath + "/Danger.png");
        QPixmap warningImageTemp(filePath + "/Warning.png");
        QPixmap promptImageTemp(filePath + "/Prompt.png");
        QPixmap successImageTemp(filePath + "/Success.png");

        if (!primarySheetTemp.isEmpty())
            primarySheets.insert(key, primarySheet + "\n\n" + primarySheetTemp);
        if (!dangerSheetTemp.isEmpty())
            dangerSheets.insert(key, dangerSheet + "\n\n" + dangerSheetTemp);
        if (!warningSheetTemp.isEmpty())
            warningSheets.insert(key, warningSheet + "\n\n" + warningSheetTemp);
        if (!promptSheetTemp.isEmpty())
            promptSheets.insert(key, promptSheet + "\n\n" + promptSheetTemp);
        if (!successSheetTemp.isEmpty())
            successSheets.insert(key, successSheet + "\n\n" + successSheetTemp);

        if (!backgroundImageTemp.isNull())
            backgroundImages.insert(key, backgroundImageTemp);
        if (!foregroundImageTemp.isNull())
            foregroundImages.insert(key, foregroundImageTemp);

        if (!primaryImageTemp.isNull())
            primaryImages.insert(key, primaryImageTemp);
        if (!dangerImageTemp.isNull())
            dangerImages.insert(key, dangerImageTemp);
        if (!warningImageTemp.isNull())
            warningImages.insert(key, warningImageTemp);
        if (!promptImageTemp.isNull())
            promptImages.insert(key, promptImageTemp);
        if (!successImageTemp.isNull())
            successImages.insert(key, successImageTemp);
    }
}

QPixmap HtmlDrawPrivate::drawText(const QString &text, HtmlDraw::Style style, int width, qint64 theme) const
{
    QPixmap source;

    qreal contentMargins = 20;
    qreal bw = 12;
    qreal cm = contentMargins;

    QTextDocument htmlDoc;
    htmlDoc.setUndoRedoEnabled(false);
    htmlDoc.setUseDesignMetrics(true);
    htmlDoc.setTextWidth(width - bw - cm * 2);
    switch (style) {
    case HtmlDraw::Primary:
        htmlDoc.setDefaultStyleSheet(primarySheets.value(theme, primarySheet));
        source = primaryImages.value(theme, primaryImage);
        break;
    case HtmlDraw::Danger:
        htmlDoc.setDefaultStyleSheet(dangerSheets.value(theme, dangerSheet));
        source = dangerImages.value(theme, dangerImage);
        break;
    case HtmlDraw::Warning:
        htmlDoc.setDefaultStyleSheet(warningSheets.value(theme, warningSheet));
        source = warningImages.value(theme, warningImage);
        break;
    case HtmlDraw::Prompt:
        htmlDoc.setDefaultStyleSheet(promptSheets.value(theme, promptSheet));
        source = promptImages.value(theme, promptImage);
        break;
    case HtmlDraw::Success:
        htmlDoc.setDefaultStyleSheet(successSheets.value(theme, successSheet));
        source = successImages.value(theme, successImage);
        break;
    }
    htmlDoc.setHtml(text);

    QSizeF size = htmlDoc.size();
    QPixmap target(width, size.height() + cm * 2);
    target.fill(Qt::transparent);

    QPainter painter(&target);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    int tw = target.width();
    int th = target.height();
    int sw = source.width();
    int sh = source.height();

    // Background

    QPixmap bgImage = backgroundImages.value(theme, backgroundImage);
    if (!bgImage.isNull()) {
        painter.save();
        painter.translate(qrand() % 10, qrand() % 10);
        painter.drawTiledPixmap(-tw, -th, tw * 3, th * 3, bgImage);
        painter.restore();
    }

    if (!source.isNull()) {
        painter.drawPixmap(0, 0, 40, 40, source, 0, 0, 40, 40);
        painter.drawPixmap(40, 0, tw - 80, 40, source, 40, 0, sw - 80, 40);
        painter.drawPixmap(tw - 40, 0, 40, 40, source, sw - 40, 0, 40, 40);
        painter.drawPixmap(tw - 40, 40, 40, th - 80, source, sw - 40, 40, 40, sh - 80);
        painter.drawPixmap(tw - 40, th - 40, 40, 40, source, sw - 40, sh - 40, 40, 40);
        painter.drawPixmap(40, th - 40, tw - 80, 40, source, 40, sh - 40, sw - 80, 40);
        painter.drawPixmap(0, th - 40, 40, 40, source, 0, sh - 40, 40, 40);
        painter.drawPixmap(0, 40, 40, th - 80, source, 0, 40, 40, sh - 80);
        painter.drawPixmap(40, 40, tw - 80, th - 80, source, 40, 40, sw - 80, sh - 80);
    }

    painter.translate(bw + cm, cm);
    htmlDoc.drawContents(&painter);

    QPixmap fgImage = foregroundImages.value(theme, foregroundImage);
    if (!fgImage.isNull()) {
        painter.save();
        painter.translate(qrand() % 10, qrand() % 10);
        painter.drawTiledPixmap(-tw, -th, tw * 3, th * 3, fgImage);
        painter.restore();
    }

    return target;
}

QString HtmlDrawPrivate::readCssFile(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        return QString::fromUtf8(file.readAll());
    }

    return QString();
}
