#ifndef TIMECLIENT_H
#define TIMECLIENT_H

#include <QDBusContext>
#include <QObject>

#include "serverInterface.h"

class TimeClient : public QObject, protected QDBusContext {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", TIME_SERVICE_API)

 public:
  explicit TimeClient(QObject* parent = nullptr);
  ~TimeClient() override;

  QString getSystemTimestamp();
  bool getPermission(int permissionEnumCode);
};

#endif  // TIMECLIENT_H
