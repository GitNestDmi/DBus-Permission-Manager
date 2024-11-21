#ifndef MYSERVER_H
#define MYSERVER_H

#include <QDBusContext>
#include <QDBusReply>
#include <QObject>
#include <memory>

#include "database.h"
#include "server_interface.h"

#define DATA_BASE_NAME "Permissions.db"

class PermissionService : public QObject, protected QDBusContext {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", PERNISSIONS_SERVICE_API)

public:
    explicit PermissionService(QObject* parent = nullptr);
    ~PermissionService() override;

public Q_SLOTS:
    Q_SCRIPTABLE void requestPermission(int permissionEnumCode) const;
    Q_SCRIPTABLE bool checkApplicationHasPermission(const QString applicationExecPath,
        const int permissionEnumCode) const;

    // Для проверки записей, нет в задании
    Q_SCRIPTABLE void showPermissions() const;

private:
    std::unique_ptr<IDataBase> database_;
};

QString getExecutablePath(const QDBusReply<uint> reply);

#endif // MYSERVER_H
