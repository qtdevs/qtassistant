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

QImage HtmlFeedback::drawPrimary(const QString &html, int width) const
{
    return d_ptr->draw(html, width, HtmlFeedbackPrivate::Primary);
}

QImage HtmlFeedback::drawDanger(const QString &html, int width) const
{
    return d_ptr->draw(html, width, HtmlFeedbackPrivate::Danger);
}

QImage HtmlFeedback::drawWarning(const QString &html, int width) const
{
    return d_ptr->draw(html, width, HtmlFeedbackPrivate::Warning);
}

QImage HtmlFeedback::drawPrompt(const QString &html, int width) const
{
    return d_ptr->draw(html, width, HtmlFeedbackPrivate::Prompt);
}

QImage HtmlFeedback::drawSuccess(const QString &html, int width) const
{
    return d_ptr->draw(html, width, HtmlFeedbackPrivate::Success);
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

QImage HtmlFeedbackPrivate::draw(const QString &html, int width, Style style) const
{
    QColor backgroundColor;
    QString promptChar;

    qreal sideBarWidth = 50;
    qreal contentMargins = 10;

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
        htmlDoc.setDefaultStyleSheet(primarySheet);
        break;
    case Danger:
        promptChar = QChar(0xf00d);
        backgroundColor.setNamedColor("#c9302c");
        htmlDoc.setDefaultStyleSheet(dangerSheet);
        break;
    case Warning:
        promptChar = QChar(0xf12a);
        backgroundColor.setNamedColor("#ec971f");
        htmlDoc.setDefaultStyleSheet(warningSheet);
        break;
    case Prompt:
        promptChar = QChar(0xf129);
        backgroundColor.setNamedColor("#31b0d5");
        htmlDoc.setDefaultStyleSheet(promptSheet);
        break;
    case Success:
        promptChar = QChar(0xf00c);
        backgroundColor.setNamedColor("#449d44");
        htmlDoc.setDefaultStyleSheet(successSheet);
        break;
    }
    htmlDoc.setHtml(html);

    QSizeF size = htmlDoc.size();
    QImage pixmap(width, size.height() + cm * 2, QImage::Format_RGB32);
    pixmap.fill(backgroundColor);

    QPainter painter(&pixmap);
    // SideBar
    painter.setPen(Qt::white);
    painter.setFont(awesomeFont);
    QRect swRect(0, 0, sw, pixmap.height());
    painter.fillRect(swRect, backgroundColor.darker(120));
    // painter.drawText(swRect, Qt::AlignCenter, promptChar);
    // Content
    painter.translate(sw + cm, cm);
    htmlDoc.drawContents(&painter);

    return pixmap;
}
