#include "timeclient.h"

#include <QDebug>

int main() {
  TimeClient service{};
  QString date(service.getSystemTimestamp());

  if (!date.isEmpty())
    qInfo() << "Date " << date;

  return 0;
}
