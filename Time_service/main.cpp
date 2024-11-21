#include <QCoreApplication>
#include <QtDBus>

#include "server_interface.h"
#include "time_service.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    TimeService service(&app);

    //Регистрируем сервис и объект на сесссионной шине
    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.registerService(TIME_SERVICE_NAME)) {
        qWarning() << "Failed to register service " << TIME_SERVICE_NAME;
        return 1;
    }
    if (!connection.registerObject(TIME_SERVICE_PATH, &service,
            QDBusConnection::ExportScriptableSlots)) {
        qWarning() << "Failed to register object " << TIME_SERVICE_PATH << " "
                   << &service;
        return 1;
    }

    return app.exec();
}
