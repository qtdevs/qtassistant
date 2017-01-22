#ifndef HTMLFEEDBACK_H
#define HTMLFEEDBACK_H

#if _MSC_VER >= 1600
#  pragma execution_character_set("utf-8")
#endif

#include <QObject>

class HtmlFeedbackPrivate;
class HtmlFeedback : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HtmlFeedback)

public:
    explicit HtmlFeedback(QObject *parent = Q_NULLPTR);
    virtual ~HtmlFeedback();
private:
    QScopedPointer<HtmlFeedbackPrivate> d_ptr;

public:
    QPixmap drawPrimary(const QString &html, int width) const;
    QPixmap drawDanger(const QString &html, int width) const;
    QPixmap drawWarning(const QString &html, int width) const;
    QPixmap drawPrompt(const QString &html, int width) const;
    QPixmap drawSuccess(const QString &html, int width) const;

public:
    enum Style {
        Primary,
        Danger,
        Warning,
        Prompt,
        Success,
    };

public:
    QPixmap draw(const QString &html, int width, Style style) const;
};

#endif // HTMLFEEDBACK_H
