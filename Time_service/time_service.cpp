#include "time_service.h"

#include <QDBusConnectionInterface>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QtDBus>

TimeService::TimeService(QObject* parent)
    : QObject(parent)
{
}

//-------------------------------------------------------------------------------

TimeService::~TimeService() { }

//-------------------------------------------------------------------------------

quint64 TimeService::getSystemTime() const
{
    // Получаем PID клиента и преобразовываем в путь до исполяемого файла
    QString path(getExecutablePath(connection().interface()->servicePid(message().service())));

    qDebug() << "ass1";
    if (path.isEmpty()) {
        sendErrorReply(QDBusError::Failed,
            "Error, could not get the path to the client executable file");
    }

    QDBusInterface iface(PERNISSIONS_SERVICE_NAME, PERNISSIONS_SERVICE_PATH,
        PERNISSIONS_SERVICE_API, QDBusConnection::sessionBus());

    if (!iface.isValid()) {
        QString error("Error, failed to connect to the interface ");
        error.append(PERNISSIONS_SERVICE_NAME);
        sendErrorReply(QDBusError::Failed, error);
    }

    QDBusReply<bool> reply = iface.call("CheckApplicationHasPermission", path,
        Permissions::SystemTime);

    if (!reply.isValid()) {
        sendErrorReply(QDBusError::Failed, reply.error().message());
    }

    if (reply.value()) {
        // Получаем и отправляем timestamp
        QDateTime currentTime = QDateTime::currentDateTime();
        quint64 timestamp = static_cast<quint64>(currentTime.toMSecsSinceEpoch());
        return timestamp;
    } else {
        // Создание ответа
        QDBusMessage reply = message().createReply(Permissions::UnauthorizedAccess);
        QDBusConnection::sessionBus().send(reply);
        return 0;
    }
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
