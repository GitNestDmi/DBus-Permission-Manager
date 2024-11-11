#ifndef MYSERVER_H
#define MYSERVER_H

#include <database.h>
#include <QDBusContext>
#include <QDBusReply>
#include <QObject>

namespace Permissions {
enum class permType : int { SystemTime = 0 };

}

class MyServer : public QObject, protected QDBusContext {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "com.system.permissionsAPI")

 public:
  explicit MyServer(QObject* parent = nullptr);
  ~MyServer() override;

 public Q_SLOTS:
  Q_SCRIPTABLE void RequestPermission(int permissionEnumCode);
  Q_SCRIPTABLE bool CheckApplicationHasPermission(QString applicationExecPath,
                                                  int permissionEnumCode);
  Q_SCRIPTABLE void showPermissions();  // Для проверки записей, нет в задании

 private:
  Database database;

  QString getExecutablePath(QDBusReply<uint> reply);
};

#endif  // MYSERVER_H
