#ifndef HTMLFEEDBACK_H
#define HTMLFEEDBACK_H

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
    QImage drawPrimary(const QString &html, int width) const;
    QImage drawDanger(const QString &html, int width) const;
    QImage drawWarning(const QString &html, int width) const;
    QImage drawPrompt(const QString &html, int width) const;
    QImage drawSuccess(const QString &html, int width) const;
};

#endif // HTMLFEEDBACK_H
