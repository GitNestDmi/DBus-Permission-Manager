#ifndef MYSERVER_H
#define MYSERVER_H

#include <QDBusContext>
#include <QDBusReply>
#include <QObject>
#include <memory>

#include "database.h"
#include "serverInterface.h"

class PermissionService : public QObject, protected QDBusContext {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", PERNISSIONS_SERVICE_API)

 public:
  explicit PermissionService(QObject* parent = nullptr);
  ~PermissionService() override;

 public Q_SLOTS:
  Q_SCRIPTABLE void RequestPermission(int permissionEnumCode);
  Q_SCRIPTABLE bool CheckApplicationHasPermission(QString applicationExecPath,
                                                  int permissionEnumCode);

  Q_SCRIPTABLE void showPermissions();  // Для проверки записей, нет в задании

 private:
  std::unique_ptr<IDataBase> database;
};

QString getExecutablePath(QDBusReply<uint> reply);

#endif  // MYSERVER_H
