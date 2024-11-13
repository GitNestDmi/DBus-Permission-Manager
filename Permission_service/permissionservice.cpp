#include "permissionservice.h"

#include <QDBusConnectionInterface>
#include <QDebug>
#include <QFileInfo>

PermissionService::PermissionService(QObject* parent)
    : QObject(parent), database(std::make_unique<Database>()) {}

//-------------------------------------------------------------------------------

PermissionService::~PermissionService() {}

//-------------------------------------------------------------------------------

void PermissionService::RequestPermission(int permissionEnumCode) {
  // Проверяем верный ли код разрешения
  if (!Permissions::types.contains(
          static_cast<Permissions::permType>(permissionEnumCode))) {
    sendErrorReply(QDBusError::Failed,
                   "Error, there is no such permission code");
    return;
  }

  if (!database->openDatabase("Permission.db")) {
    sendErrorReply(QDBusError::Failed,
                   "Error, the database could not be opened");
    database->closeDatabase();
    return;
  }

  // Получаем PID клиента и преобразовываем в путь до исполяемого файла
  QString path(getExecutablePath(
      connection().interface()->servicePid(message().service())));

  if (path.isEmpty()) {
    sendErrorReply(
        QDBusError::Failed,
        "Error, could not get the path to the client executable file");
    database->closeDatabase();
    return;
  }

  // Добовляем путь к клиенту и код разрешения в базу данных
  QSqlQuery* query = database->getQuery();
  query->exec(
      "CREATE TABLE IF NOT EXISTS permissions ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "exePath TEXT, "
      "code INTEGER, "
      "UNIQUE(exePath, code))");

  query->prepare(
      "INSERT INTO permissions (exePath, code) "
      "VALUES (:exePath, :code)");
  query->bindValue(":exePath", path);
  query->bindValue(":code", permissionEnumCode);

  if (!query->exec()) {
    sendErrorReply(
        QDBusError::Failed,
        "Error when inserting a record in sql:" + query->lastError().text());
    database->closeDatabase();
  }

  database->closeDatabase();
}

//-------------------------------------------------------------------------------

bool PermissionService::CheckApplicationHasPermission(
    QString applicationExecPath,
    int permissionEnumCode) {
  // Проверяем верный ли код разрешения
  if (!Permissions::types.contains(
          static_cast<Permissions::permType>(permissionEnumCode))) {
    sendErrorReply(QDBusError::Failed,
                   "Error, there is no such permission code");
    return false;
  }

  // TODO
  // будет ли открываться, если ее еще нет ?
  if (!database->openDatabase("Permission.db")) {
    sendErrorReply(QDBusError::Failed,
                   "Error, the database could not be opened");
    database->closeDatabase();
    return false;
  }

  QSqlQuery* query = database->getQuery();
  query->prepare(
      "SELECT exePath, code FROM permissions "
      "WHERE exePath = :exePath AND code = :code");
  query->bindValue(":exePath", applicationExecPath);
  query->bindValue(":code", permissionEnumCode);

  if (!query->exec()) {
    sendErrorReply(
        QDBusError::Failed,
        "Error when selecting an entry in sql:" + query->lastError().text());
    database->closeDatabase();
    return false;
  }

  database->closeDatabase();

  return query->next();
}

//-------------------------------------------------------------------------------

void PermissionService::showPermissions() {
  if (!database->openDatabase("Permission.db"))
    return;

  QSqlQuery* query = database->getQuery();
  query->exec("SELECT id, exePath, code FROM permissions");

  while (query->next()) {
    qDebug() << "| id " << query->value(0).toString() << "\t| exePath "
             << query->value(1).toString() << "\t| code "
             << query->value(2).toString() << " \t|";
  }

  database->closeDatabase();
}

//-------------------------------------------------------------------------------

QString getExecutablePath(QDBusReply<uint> reply) {
  if (reply.isValid()) {
    uint pid = reply.value();

    // Формируем путь к файлу /proc/<PID>/exe
    QString procFilePath = QString("/proc/%1/exe").arg(pid);

    // Используем QFileInfo для получения пути к исполняемому файлу
    QFileInfo exeFile(procFilePath);

    // Возвращаем полный путь к исполняемому файлу
    return exeFile.canonicalFilePath();
  }

  qWarning() << "Failed to get executable path for client " << reply.error();
  return QString();
}
