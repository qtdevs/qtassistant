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
protected:
    QScopedPointer<HtmlFeedbackPrivate> d_ptr;

public:
    void updateMaterials(const QString &path);

public:
    enum Style {
        Primary,
        Danger,
        Warning,
        Prompt,
        Success,
    };
    Q_ENUM(Style)

public:
    QPixmap drawText(const QString &text, int width, Style style, qint64 theme = 0) const;

public:
    QPixmap drawPrimaryText(const QString &text, int width, qint64 theme = 0) const;
    QPixmap drawDangerText(const QString &text, int width, qint64 theme = 0) const;
    QPixmap drawWarningText(const QString &text, int width, qint64 theme = 0) const;
    QPixmap drawPromptText(const QString &text, int width, qint64 theme = 0) const;
    QPixmap drawSuccessText(const QString &text, int width, qint64 theme = 0) const;
};

#endif // HTMLFEEDBACK_H
