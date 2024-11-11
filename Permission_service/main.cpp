#include <QCoreApplication>
#include <QObject>
#include <QtDBus/QDBusConnection>
#include "myserver.h"
#include "serverInterface.h"

int main(int argc, char* argv[]) {
  QCoreApplication app(argc, argv);
  MyServer server(&app);

  //Регистрируем сервис и объект на сесссионной шине
  QDBusConnection connection = QDBusConnection::sessionBus();
  if (!connection.registerService(SERVICE_NAME)) {
    qWarning() << "Failed to register service.";
    return 1;
  }
  if (!connection.registerObject(SERVICE_PATH, &server,
                                 QDBusConnection::ExportScriptableSlots)) {
    qWarning() << "Failed to register object.";
    return 1;
  }

  return app.exec();
}
