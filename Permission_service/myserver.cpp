#include "myserver.h"

#include <QDBusConnectionInterface>
#include <QDebug>
#include <QFileInfo>

MyServer::MyServer(QObject* parent) : QObject(parent), database{this} {}

//-------------------------------------------------------------------------------

MyServer::~MyServer() {}

//-------------------------------------------------------------------------------

QString MyServer::getExecutablePath(QDBusReply<uint> reply) {
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

//-------------------------------------------------------------------------------

void MyServer::RequestPermission(int permissionEnumCode) {
  // TODO
  //сделать проверку кода разрешения
  // Permissions code = permissionEnumCode;

  // TODO
  // В случае ошибки метод должен возвращать DBus
  // ошибку с человекочитаемым сообщением.

  if (!database.openDatabase("Permission.db"))
    return;

  // Получаем PID клиента и преобразовываем в путь до исполяемого файла
  QString path = getExecutablePath(
      connection().interface()->servicePid(message().service()));

  if (path.isEmpty()) {
    qWarning() << "Path empty";
    return;
  }

  QSqlQuery* query = database.getQuery();
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
    qWarning() << "Error when inserting a record in sql:"
               << query->lastError().text();
  }

  database.closeDatabase();
}

//-------------------------------------------------------------------------------

bool MyServer::CheckApplicationHasPermission(QString applicationExecPath,
                                             int permissionEnumCode) {
  // TODO
  //  В случае ошибки метод должен возвращать DBus
  //  ошибку с человекочитаемым сообщением

  if (!database.openDatabase("Permission.db"))
    return false;

  QSqlQuery* query = database.getQuery();

  query->prepare(
      "SELECT exePath, code FROM permissions "
      "WHERE exePath = :exePath AND code = :code");
  query->bindValue(":exePath", applicationExecPath);
  query->bindValue(":code", permissionEnumCode);

  if (!query->exec()) {
    qWarning() << "Error when selecting an entry in sql:"
               << query->lastError().text();
  }

  database.closeDatabase();
  return query->next();
}

//-------------------------------------------------------------------------------

void MyServer::showPermissions() {
  if (!database.openDatabase("Permission.db"))
    return;

  QSqlQuery* query = database.getQuery();
  query->exec("SELECT id, exePath, code FROM permissions");

  while (query->next()) {
    qDebug() << "| id " << query->value(0).toString() << "\t| exePath "
             << query->value(1).toString() << "\t| code "
             << query->value(2).toString() << " \t|";
  }

  database.closeDatabase();
}
