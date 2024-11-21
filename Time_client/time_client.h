#ifndef TIMECLIENT_H
#define TIMECLIENT_H

#include <QDBusContext>
#include <QObject>

#include "server_interface.h"

class TimeClient : public QObject, protected QDBusContext {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", TIME_SERVICE_API)

public:
    explicit TimeClient(QObject* parent = nullptr);
    ~TimeClient() override;

    QString getSystemTimestamp() const;
    bool getPermission(const int permissionEnumCode) const;
};

#endif // TIMECLIENT_H
