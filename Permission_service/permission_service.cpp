#include "permission_service.h"

#include <QDBusConnectionInterface>
#include <QDebug>
#include <QFileInfo>

PermissionService::PermissionService(QObject* parent)
    : QObject(parent)
    , database_(std::make_unique<Database>())
{
    //Создаем пустую БД, если ее еще нет
    if (!database_->openDatabase(DATA_BASE_NAME)) {
        database_->closeDatabase();
    }

    QSqlQuery* query = database_->getQuery();
    query->exec(
        "CREATE TABLE IF NOT EXISTS permissions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "exePath TEXT, "
        "code INTEGER, "
        "UNIQUE(exePath, code))");

    database_->closeDatabase();
}

//-------------------------------------------------------------------------------

PermissionService::~PermissionService() { }

//-------------------------------------------------------------------------------

void PermissionService::requestPermission(const int permissionEnumCode) const
{
    // Проверяем верный ли код разрешения
    if (!Permissions::types.contains(
            static_cast<Permissions::permType>(permissionEnumCode))) {
        sendErrorReply(QDBusError::Failed,
            "Error, there is no such permission code");
        return;
    }

    if (!database_->openDatabase(DATA_BASE_NAME)) {
        sendErrorReply(QDBusError::Failed,
            "Error, the database could not be opened");
        database_->closeDatabase();
        return;
    }

    // Получаем PID клиента и преобразовываем в путь до исполяемого файла
    QString path(getExecutablePath(
        connection().interface()->servicePid(message().service())));

    if (path.isEmpty()) {
        sendErrorReply(
            QDBusError::Failed,
            "Error, could not get the path to the client executable file");
        database_->closeDatabase();
        return;
    }

    // Добовляем путь к клиенту и код разрешения в базу данных
    QSqlQuery* query = database_->getQuery();
    query->prepare(
        "INSERT INTO permissions (exePath, code) "
        "VALUES (:exePath, :code)");
    query->bindValue(":exePath", path);
    query->bindValue(":code", permissionEnumCode);

    if (!query->exec()) {
        sendErrorReply(
            QDBusError::Failed,
            "Error when inserting a record in sql: " + query->lastError().text());
        database_->closeDatabase();
    }

    database_->closeDatabase();
}

//-------------------------------------------------------------------------------

bool PermissionService::checkApplicationHasPermission(
    const QString applicationExecPath,
    const int permissionEnumCode) const
{
    // Проверяем верный ли код разрешения
    if (!Permissions::types.contains(
            static_cast<Permissions::permType>(permissionEnumCode))) {
        sendErrorReply(QDBusError::Failed,
            "Error, there is no such permission code");
        return false;
    }

    if (!database_->openDatabase(DATA_BASE_NAME)) {
        sendErrorReply(QDBusError::Failed,
            "Error, the database could not be opened");
        database_->closeDatabase();
        return false;
    }

    QSqlQuery* query = database_->getQuery();
    query->prepare(
        "SELECT exePath, code FROM permissions "
        "WHERE exePath = :exePath AND code = :code");
    query->bindValue(":exePath", applicationExecPath);
    query->bindValue(":code", permissionEnumCode);

    if (!query->exec()) {
        sendErrorReply(
            QDBusError::Failed,
            "Error when selecting an entry in sql: " + query->lastError().text());
        database_->closeDatabase();
        return false;
    }

    database_->closeDatabase();

    return query->next();
}

//-------------------------------------------------------------------------------

void PermissionService::showPermissions() const
{
    if (!database_->openDatabase(DATA_BASE_NAME))
        return;

    QSqlQuery* query = database_->getQuery();
    query->exec("SELECT id, exePath, code FROM permissions");

    while (query->next()) {
        qDebug() << "| id " << query->value(0).toString() << "\t| exePath "
                 << query->value(1).toString() << "\t| code "
                 << query->value(2).toString() << " \t|";
    }

    database_->closeDatabase();
}

//-------------------------------------------------------------------------------

QString getExecutablePath(const QDBusReply<uint> reply)
{
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
