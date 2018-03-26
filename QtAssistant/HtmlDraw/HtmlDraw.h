#ifndef HTMLDRAW_H
#define HTMLDRAW_H

#include <QObject>

class HtmlDrawPrivate;
class HtmlDraw : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HtmlDraw)

public:
    explicit HtmlDraw(const QString &path, QObject *parent = nullptr);
protected:
    QScopedPointer<HtmlDrawPrivate> d_ptr;
public:
    virtual ~HtmlDraw();



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
    static QPixmap drawText(const QString &text, Style style, int width = 400, qint64 theme = 0);

public:
    static QPixmap drawPrimaryText(const QString &text, int width = 400, qint64 theme = 0);
    static QPixmap drawDangerText(const QString &text, int width = 400, qint64 theme = 0);
    static QPixmap drawWarningText(const QString &text, int width = 400, qint64 theme = 0);
    static QPixmap drawPromptText(const QString &text, int width = 400, qint64 theme = 0);
    static QPixmap drawSuccessText(const QString &text, int width = 400, qint64 theme = 0);


};

#endif // HTMLDRAW_H
