#include <QtCore>
#include <QtWidgets>

#include "CqServicePortal.h"

#include "managemodule.h"
#include "donatemodule.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    auto portal = new CoolQ::ServicePortal(&app);

    new ManageModule(portal);
    new DonateModule(portal);

    if (!portal->initialize()) {
        return false;
    }

    QWidget mainWindow;
    mainWindow.show();

    return app.exec();
}
