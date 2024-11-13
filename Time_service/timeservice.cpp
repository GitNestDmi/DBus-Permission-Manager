#include "timeservice.h"

#include <QDBusConnectionInterface>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QtDBus>

TimeService::TimeService(QObject* parent) : QObject(parent) {}

//-------------------------------------------------------------------------------

TimeService::~TimeService() {}

//-------------------------------------------------------------------------------

quint64 TimeService::GetSystemTime() {
  // Получаем PID клиента и преобразовываем в путь до исполяемого файла
  QString path(getExecutablePath(
      connection().interface()->servicePid(message().service())));

  if (path.isEmpty()) {
    sendErrorReply(
        QDBusError::Failed,
        "Error, could not get the path to the client executable file");
    return 0;
  }

  QDBusInterface iface(PERNISSIONS_SERVICE_NAME, PERNISSIONS_SERVICE_PATH,
                       PERNISSIONS_SERVICE_API, QDBusConnection::sessionBus());

  if (!iface.isValid()) {
    QString error("Error, failed to connect to the interface ");
    error.append(PERNISSIONS_SERVICE_NAME);
    sendErrorReply(QDBusError::Failed, error);
    return 0;
  }

  QDBusReply<bool> reply = iface.call("CheckApplicationHasPermission", path,
                                      Permissions::SystemTime);

  if (!reply.isValid()) {
    sendErrorReply(QDBusError::Failed, reply.error().message());
    return 0;
  }

  if (reply.value()) {
    // Получаем и отправляем timestamp
    QDateTime currentTime = QDateTime::currentDateTime();
    quint64 timestamp = static_cast<qint64>(currentTime.toMSecsSinceEpoch());
    return timestamp;
  } else {
    // Создание ответа
    QDBusMessage reply = message().createReply(Permissions::UnauthorizedAccess);
    QDBusConnection::sessionBus().send(reply);
    return 0;
  }
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
