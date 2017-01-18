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

    QColor backgroundColor;
    const QPixmap *siderBarImage = Q_NULLPTR;

    qreal sideBarWidth = 50;
    qreal contentMargins = 4;

    qreal sw = sideBarWidth;
    qreal cm = contentMargins;

    QTextDocument htmlDoc;
    htmlDoc.setUndoRedoEnabled(false);
    htmlDoc.setUseDesignMetrics(true);
    htmlDoc.setTextWidth(width - sw - cm * 2);
    switch (style) {
    case Primary:
        htmlDoc.setDefaultStyleSheet(d->primarySheet);
        backgroundColor.setNamedColor("#3071a9");
        siderBarImage = &d->primaryImage;
        break;
    case Danger:
        htmlDoc.setDefaultStyleSheet(d->dangerSheet);
        backgroundColor.setNamedColor("#c9302c");
        siderBarImage = &d->dangerImage;
        break;
    case Warning:
        htmlDoc.setDefaultStyleSheet(d->warningSheet);
        backgroundColor.setNamedColor("#ec971f");
        siderBarImage = &d->warningImage;
        break;
    case Prompt:
        htmlDoc.setDefaultStyleSheet(d->promptSheet);
        backgroundColor.setNamedColor("#31b0d5");
        siderBarImage = &d->promptImage;
        break;
    case Success:
        htmlDoc.setDefaultStyleSheet(d->successSheet);
        backgroundColor.setNamedColor("#449d44");
        siderBarImage = &d->successImage;
        break;
    }
    htmlDoc.setHtml(html);

    QSizeF size = htmlDoc.size();
    QPixmap pixmap(width, size.height() + cm * 2);
    pixmap.fill(backgroundColor);

    QPainter painter(&pixmap);
    // SideBar
    painter.setPen(Qt::white);
    QRect swRect(0, 0, sw, pixmap.height());
    painter.fillRect(swRect, backgroundColor.darker(120));
    if (siderBarImage) {
        QPoint c = swRect.center();
        int cy = c.y() - siderBarImage->height() / 2;
        int cx = c.x() - siderBarImage->width() / 2;
        painter.drawPixmap(cx, cy, *siderBarImage);
    }
    // Content
    painter.translate(sw + cm, cm);
    htmlDoc.drawContents(&painter);

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
}

HtmlFeedbackPrivate::~HtmlFeedbackPrivate()
{
}
