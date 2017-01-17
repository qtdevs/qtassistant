#include <QtCore>
#include <QtWidgets>

#include "qtassistant.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    QtAssistant assistant;
    Q_UNUSED(assistant);

    QWidget mainWindow;
    mainWindow.show();

    return app.exec();
}
