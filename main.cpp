#include <QtCore>
#include <QtWidgets>

#include "CoolQServiceEngine.h"
#include "AssistantModule.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    auto engine = new CoolQ::ServiceEngine(&app);

    new AssistantModule(engine);
    if (!engine->initialize()) {
        return false;
    }

    QWidget mainWindow;
    mainWindow.show();

    return app.exec();
}
