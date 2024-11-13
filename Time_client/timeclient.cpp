#include "timeclient.h"

#include <QDBusConnectionInterface>
#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QtDBus>

TimeClient::TimeClient(QObject* parent) : QObject(parent) {}

//-------------------------------------------------------------------------------

TimeClient::~TimeClient() {}

//-------------------------------------------------------------------------------

QString TimeClient::getSystemTimestamp() {
  //Подключаемся к сервису системного времени
  QDBusInterface iface(TIME_SERVICE_NAME, TIME_SERVICE_PATH, TIME_SERVICE_API,
                       QDBusConnection::sessionBus());

  if (!iface.isValid()) {
    qWarning() << "Error, failed to connect to the interface "
               << TIME_SERVICE_NAME;
    return "";
  }

  //Попытка запросить время
  QDBusMessage responce = iface.call("GetSystemTime");
  QDBusReply<quint64> reply;

  //Если нет разрешения, то запрашиваем его и повторяем запрос времени
  if (responce.arguments().at(0) == Permissions::UnauthorizedAccess) {
    //Просим разрешение
    if (!getPermission(Permissions::SystemTime))
      return "";

    reply = iface.call("GetSystemTime");
  } else {
    reply = responce;
  }

  if (!reply.isValid()) {
    qWarning() << reply.error().message();
    return "";
  }

  // Преобразуем timestamp в человекочитаемый вид
  QDateTime dateTime =
      QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(reply.value()));
  QString formattedDateTime = dateTime.toString("dd.MM.yyyy hh:mm:ss");
  return formattedDateTime;
}

//-------------------------------------------------------------------------------

bool TimeClient::getPermission(int permissionEnumCode) {
  //Подключаемся к сервису разрешений
  QDBusInterface iface(PERNISSIONS_SERVICE_NAME, PERNISSIONS_SERVICE_PATH,
                       PERNISSIONS_SERVICE_API, QDBusConnection::sessionBus());

  if (!iface.isValid()) {
    qWarning() << "Error, failed to connect to the interface "
               << PERNISSIONS_SERVICE_NAME;
    return false;
  }

  //Попытка запросить разрешение
  QDBusReply<void> reply = iface.call("RequestPermission", permissionEnumCode);

  //Обработка ошибок
  if (!reply.isValid()) {
    qWarning() << reply.error().message();
    return false;
  }

  return true;
}

//-------------------------------------------------------------------------------
