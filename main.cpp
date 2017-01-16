#include <QtCore>
#include <QtWidgets>

#include "cqassistant.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    CqAssistant assistant;
    Q_UNUSED(assistant);

    QWidget mainWindow;
    mainWindow.show();

    return app.exec();
}
