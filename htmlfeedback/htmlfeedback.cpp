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
    QString promptChar;

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
        promptChar = QChar(0xf0e6);
        backgroundColor.setNamedColor("#3071a9");
        htmlDoc.setDefaultStyleSheet(d->primarySheet);
        break;
    case Danger:
        promptChar = QChar(0xf00d);
        backgroundColor.setNamedColor("#c9302c");
        htmlDoc.setDefaultStyleSheet(d->dangerSheet);
        break;
    case Warning:
        promptChar = QChar(0xf12a);
        backgroundColor.setNamedColor("#ec971f");
        htmlDoc.setDefaultStyleSheet(d->warningSheet);
        break;
    case Prompt:
        promptChar = QChar(0xf129);
        backgroundColor.setNamedColor("#31b0d5");
        htmlDoc.setDefaultStyleSheet(d->promptSheet);
        break;
    case Success:
        promptChar = QChar(0xf00c);
        backgroundColor.setNamedColor("#449d44");
        htmlDoc.setDefaultStyleSheet(d->successSheet);
        break;
    }
    htmlDoc.setHtml(html);

    QSizeF size = htmlDoc.size();
    QPixmap pixmap(width, size.height() + cm * 2);
    pixmap.fill(backgroundColor);

    QPainter painter(&pixmap);
    // SideBar
    painter.setPen(Qt::white);
    painter.setFont(d->awesomeFont);
    QRect swRect(0, 0, sw, pixmap.height());
    painter.fillRect(swRect, backgroundColor.darker(120));
    // painter.drawText(swRect, Qt::AlignCenter, promptChar);
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

    awesomeFont = QFont("FontAwesome", 24);
}

HtmlFeedbackPrivate::~HtmlFeedbackPrivate()
{
}
