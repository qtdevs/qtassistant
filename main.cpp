#include <QtCore>
#include <QtWidgets>

#include "managemodule.h"
#include "cqengine.h"
#include "donatemodule.h"
#include "searchmodule.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    ManageModule assistant;
    DonateModule donateModule;
    SearchModule searchModule;

    CqEngine engine;
    engine.installModule(&assistant);
    engine.installModule(&donateModule);
    engine.installModule(&searchModule);

    QWidget mainWindow;
    mainWindow.show();

    return app.exec();
}
