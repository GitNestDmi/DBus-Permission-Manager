#include <QCoreApplication>
#include <QDBusConnection>
#include <QObject>

#include "permission_service.h"
#include "server_interface.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    PermissionService service(&app);
    //qDebug() << service.checkApplicationHasPermission("ass", 0);
    //service.showPermissions();
    //service.requestPermission(0);
    //qDebug() << service.checkApplicationHasPermission("ass", 0);
    service.showPermissions();

    //Регистрируем сервис и объект на сесссионной шине
    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.registerService(PERNISSIONS_SERVICE_NAME)) {
        qWarning() << "Failed to register service " << PERNISSIONS_SERVICE_NAME;
        return 1;
    }
    if (!connection.registerObject(PERNISSIONS_SERVICE_PATH, &service,
            QDBusConnection::ExportScriptableSlots)) {
        qWarning() << "Failed to register object " << PERNISSIONS_SERVICE_PATH
                   << " " << &service;
        return 1;
    }

    return app.exec();
}
