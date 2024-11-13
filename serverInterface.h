#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#define PERNISSIONS_SERVICE_NAME "com.system.permissions"
#define PERNISSIONS_SERVICE_PATH "/com/system/permissions"
#define PERNISSIONS_SERVICE_API "com.system.permissionsAPI"

#define TIME_SERVICE_NAME "com.system.time"
#define TIME_SERVICE_PATH "/com/system/time"
#define TIME_SERVICE_API "com.system.timeAPI"

#include <QVector>

namespace Permissions {

enum permType : int { SystemTime = 0 };
const QVector<permType> types = {permType::SystemTime};

}  // namespace Permissions


#endif // SERVERINTERFACE_H
