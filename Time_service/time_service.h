#ifndef TIMESERVICE_H
#define TIMESERVICE_H

#include <QDBusContext>
#include <QDBusReply>
#include <QObject>

#include "server_interface.h"

class TimeService : public QObject, protected QDBusContext {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", TIME_SERVICE_API)

public:
    explicit TimeService(QObject* parent = nullptr);
    ~TimeService() override;

public Q_SLOTS:
    Q_SCRIPTABLE quint64 getSystemTime() const;
};

QString getExecutablePath(const QDBusReply<uint> reply);
#endif // TIMESERVICE_H
