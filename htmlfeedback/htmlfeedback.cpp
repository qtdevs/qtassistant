#include "htmlfeedback.h"
#include "htmlfeedback_p.h"

#include <QTextDocument>
#include <QPainter>
#include <QFile>

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

QPixmap HtmlFeedback::drawPrimary(const QString &html, int width) const
{
    return draw(html, width, HtmlFeedback::Primary);
}

QPixmap HtmlFeedback::drawDanger(const QString &html, int width) const
{
    return draw(html, width, HtmlFeedback::Danger);
}

QPixmap HtmlFeedback::drawWarning(const QString &html, int width) const
{
    return draw(html, width, HtmlFeedback::Warning);
}

QPixmap HtmlFeedback::drawPrompt(const QString &html, int width) const
{
    return draw(html, width, HtmlFeedback::Prompt);
}

QPixmap HtmlFeedback::drawSuccess(const QString &html, int width) const
{
    return draw(html, width, HtmlFeedback::Success);
}

QPixmap HtmlFeedback::draw(const QString &html, int width, Style style) const
{
    Q_D(const HtmlFeedback);

    qreal contentMargins = 20;
    qreal bw = 12;
    qreal cm = contentMargins;

    const QPixmap *frameImage = nullptr;

    QTextDocument htmlDoc;
    htmlDoc.setUndoRedoEnabled(false);
    htmlDoc.setUseDesignMetrics(true);
    htmlDoc.setTextWidth(width - bw - cm * 2);
    switch (style) {
    case Primary:
        htmlDoc.setDefaultStyleSheet(d->primarySheet);
        frameImage = &d->primaryImage;
        break;
    case Danger:
        htmlDoc.setDefaultStyleSheet(d->dangerSheet);
        frameImage = &d->dangerImage;
        break;
    case Warning:
        htmlDoc.setDefaultStyleSheet(d->warningSheet);
        frameImage = &d->warningImage;
        break;
    case Prompt:
        htmlDoc.setDefaultStyleSheet(d->promptSheet);
        frameImage = &d->promptImage;
        break;
    case Success:
        htmlDoc.setDefaultStyleSheet(d->successSheet);
        frameImage = &d->successImage;
        break;
    }
    htmlDoc.setHtml(html);

    QSizeF size = htmlDoc.size();
    QPixmap pixmap(width, size.height() + cm * 2);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Background

    int tw = pixmap.width();
    int th = pixmap.height();

    int sw = frameImage->width();
    int sh = frameImage->height();

    if (!d->backgroundImage.isNull())
        painter.drawTiledPixmap(0, 0, tw, th, d->backgroundImage);

    painter.drawPixmap(0, 0, 40, 40, *frameImage, 0, 0, 40, 40);
    painter.drawPixmap(40, 0, tw - 80, 40, *frameImage, 40, 0, sw - 80, 40);
    painter.drawPixmap(tw - 40, 0, 40, 40, *frameImage, sw - 40, 0, 40, 40);
    painter.drawPixmap(tw - 40, 40, 40, th - 80, *frameImage, sw - 40, 40, 40, sh - 80);
    painter.drawPixmap(tw - 40, th - 40, 40, 40, *frameImage, sw - 40, sh - 40, 40, 40);
    painter.drawPixmap(40, th - 40, tw - 80, 40, *frameImage, 40, sh - 40, sw - 80, 40);
    painter.drawPixmap(0, th - 40, 40, 40, *frameImage, 0, sh - 40, 40, 40);
    painter.drawPixmap(0, 40, 40, th - 80, *frameImage, 0, 40, 40, sh - 80);
    painter.drawPixmap(40, 40, tw - 80, th - 80, *frameImage, 40, 40, sw - 80, sh - 80);

    // Content
    painter.translate(bw + cm, cm);
    htmlDoc.drawContents(&painter);

    if (!d->foregroundImage.isNull())
        painter.drawTiledPixmap(0, 0, tw, th, d->foregroundImage);

    return pixmap;
}

// class HtmlFeedbackPrivate

HtmlFeedbackPrivate::HtmlFeedbackPrivate()
    : q_ptr(Q_NULLPTR)
{
    do {
        QFile file(":/htmlfeedback/css/primary.css");
        if (file.open(QFile::ReadOnly)) {
            primarySheet = QString::fromUtf8(file.readAll());
        }
    } while (false);

    do {
        QFile file(":/htmlfeedback/css/danger.css");
        if (file.open(QFile::ReadOnly)) {
            dangerSheet = QString::fromUtf8(file.readAll());
        }
    } while (false);

    do {
        QFile file(":/htmlfeedback/css/warning.css");
        if (file.open(QFile::ReadOnly)) {
            warningSheet = QString::fromUtf8(file.readAll());
        }
    } while (false);

    do {
        QFile file(":/htmlfeedback/css/prompt.css");
        if (file.open(QFile::ReadOnly)) {
            promptSheet = QString::fromUtf8(file.readAll());
        }
    } while (false);

    do {
        QFile file(":/htmlfeedback/css/success.css");
        if (file.open(QFile::ReadOnly)) {
            successSheet = QString::fromUtf8(file.readAll());
        }
    } while (false);

    primaryImage.load(":/htmlfeedback/img/primary.png");
    dangerImage.load(":/htmlfeedback/img/danger.png");
    warningImage.load(":/htmlfeedback/img/warning.png");
    promptImage.load(":/htmlfeedback/img/prompt.png");
    successImage.load(":/htmlfeedback/img/success.png");

    backgroundImage.load(":/htmlfeedback/img/background.png");
    foregroundImage.load(":/htmlfeedback/img/foreground.png");
}

HtmlFeedbackPrivate::~HtmlFeedbackPrivate()
{
}
