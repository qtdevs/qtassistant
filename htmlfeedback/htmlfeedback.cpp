#include "htmlfeedback.h"
#include "htmlfeedback_p.h"

#include <QTextDocument>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <QtDebug>

// class HtmlFeedback

HtmlFeedback::HtmlFeedback(QObject *parent)
    : QObject(parent)
    , d_ptr(new HtmlFeedbackPrivate())
{
    d_ptr->q_ptr = this;
}

HtmlFeedback::~HtmlFeedback()
{
}

void HtmlFeedback::updateMaterials(const QString &path)
{
    Q_D(HtmlFeedback);

    for (auto fileInfo : QDir(path).entryInfoList(QDir::Dirs)) {
        qint64  key = fileInfo.fileName().toLongLong();
        QString filePath = fileInfo.absoluteFilePath();

        QString primarySheet = d->readCssFile(filePath + "/Primary.css");
        QString dangerSheet = d->readCssFile(filePath + "/Danger.css");
        QString warningSheet = d->readCssFile(filePath + "/Warning.css");
        QString promptSheet = d->readCssFile(filePath + "/Prompt.css");
        QString successSheet = d->readCssFile(filePath + "/Success.css");

        QPixmap backgroundImage(filePath + "/Background.png");
        QPixmap foregroundImage(filePath + "/Foreground.png");

        QPixmap primaryImage(filePath + "/Primary.png");
        QPixmap dangerImage(filePath + "/Danger.png");
        QPixmap warningImage(filePath + "/Warning.png");
        QPixmap promptImage(filePath + "/Prompt.png");
        QPixmap successImage(filePath + "/Success.png");

        if (!primarySheet.isEmpty())
            d->primarySheets.insert(key, d->primarySheet + "\n\n" + primarySheet);
        if (!dangerSheet.isEmpty())
            d->dangerSheets.insert(key, d->dangerSheet + "\n\n" + dangerSheet);
        if (!warningSheet.isEmpty())
            d->warningSheets.insert(key, d->warningSheet + "\n\n" + warningSheet);
        if (!promptSheet.isEmpty())
            d->promptSheets.insert(key, d->promptSheet + "\n\n" + promptSheet);
        if (!successSheet.isEmpty())
            d->successSheets.insert(key, d->successSheet + "\n\n" + successSheet);

        if (!backgroundImage.isNull())
            d->backgroundImages.insert(key, backgroundImage);
        if (!foregroundImage.isNull())
            d->foregroundImages.insert(key, foregroundImage);

        if (!primaryImage.isNull())
            d->primaryImages.insert(key, primaryImage);
        if (!dangerImage.isNull())
            d->dangerImages.insert(key, dangerImage);
        if (!warningImage.isNull())
            d->warningImages.insert(key, warningImage);
        if (!promptImage.isNull())
            d->promptImages.insert(key, promptImage);
        if (!successImage.isNull())
            d->successImages.insert(key, successImage);
    }
}

QPixmap HtmlFeedback::drawText(const QString &text, int width, Style style, qint64 theme) const
{
    Q_D(const HtmlFeedback);

    QPixmap source;

    qreal contentMargins = 20;
    qreal bw = 12;
    qreal cm = contentMargins;

    QTextDocument htmlDoc;
    htmlDoc.setUndoRedoEnabled(false);
    htmlDoc.setUseDesignMetrics(true);
    htmlDoc.setTextWidth(width - bw - cm * 2);
    switch (style) {
    case Primary:
        htmlDoc.setDefaultStyleSheet(d->primarySheets.value(theme, d->primarySheet));
        source = d->primaryImages.value(theme, d->primaryImage);
        break;
    case Danger:
        htmlDoc.setDefaultStyleSheet(d->dangerSheets.value(theme, d->dangerSheet));
        source = d->dangerImages.value(theme, d->dangerImage);
        break;
    case Warning:
        htmlDoc.setDefaultStyleSheet(d->warningSheets.value(theme, d->warningSheet));
        source = d->warningImages.value(theme, d->warningImage);
        break;
    case Prompt:
        htmlDoc.setDefaultStyleSheet(d->promptSheets.value(theme, d->promptSheet));
        source = d->promptImages.value(theme, d->promptImage);
        break;
    case Success:
        htmlDoc.setDefaultStyleSheet(d->successSheets.value(theme, d->successSheet));
        source = d->successImages.value(theme, d->successImage);
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

    QPixmap backgroundImage = d->backgroundImages.value(theme, d->backgroundImage);
    if (!backgroundImage.isNull()) {
        painter.save();
        painter.translate(qrand() % 10, qrand() % 10);
        painter.drawTiledPixmap(-tw, -th, tw * 3, th * 3, backgroundImage);
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

    QPixmap foregroundImage = d->foregroundImages.value(theme, d->foregroundImage);
    if (!foregroundImage.isNull()) {
        painter.save();
        painter.translate(qrand() % 10, qrand() % 10);
        painter.drawTiledPixmap(-tw, -th, tw * 3, th * 3, foregroundImage);
        painter.restore();
    }

    return target;
}

QPixmap HtmlFeedback::drawPrimaryText(const QString &text, int width, qint64 theme) const
{
    return drawText(text, width, HtmlFeedback::Primary, theme);
}

QPixmap HtmlFeedback::drawDangerText(const QString &text, int width, qint64 theme) const
{
    return drawText(text, width, HtmlFeedback::Danger, theme);
}

QPixmap HtmlFeedback::drawWarningText(const QString &text, int width, qint64 theme) const
{
    return drawText(text, width, HtmlFeedback::Warning, theme);
}

QPixmap HtmlFeedback::drawPromptText(const QString &text, int width, qint64 theme) const
{
    return drawText(text, width, HtmlFeedback::Prompt, theme);
}

QPixmap HtmlFeedback::drawSuccessText(const QString &text, int width, qint64 theme) const
{
    return drawText(text, width, HtmlFeedback::Success, theme);
}

// class HtmlFeedbackPrivate

HtmlFeedbackPrivate::HtmlFeedbackPrivate()
    : q_ptr(Q_NULLPTR)
{
    primarySheet = readCssFile(":/htmlfeedback/css/Primary.css");
    dangerSheet = readCssFile(":/htmlfeedback/css/Danger.css");
    warningSheet = readCssFile(":/htmlfeedback/css/Warning.css");
    promptSheet = readCssFile(":/htmlfeedback/css/Prompt.css");
    successSheet = readCssFile(":/htmlfeedback/css/Success.css");

    backgroundImage.load(":/htmlfeedback/img/Background.png");
    foregroundImage.load(":/htmlfeedback/img/Foreground.png");

    primaryImage.load(":/htmlfeedback/img/Primary.png");
    dangerImage.load(":/htmlfeedback/img/Danger.png");
    warningImage.load(":/htmlfeedback/img/Warning.png");
    promptImage.load(":/htmlfeedback/img/Prompt.png");
    successImage.load(":/htmlfeedback/img/Success.png");
}

HtmlFeedbackPrivate::~HtmlFeedbackPrivate()
{
}

QString HtmlFeedbackPrivate::readCssFile(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        return QString::fromUtf8(file.readAll());
    }

    return QString();
}
