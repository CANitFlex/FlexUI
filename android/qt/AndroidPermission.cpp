#include "AndroidPermissions.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QJniObject>
#include <QNativeInterface>
#endif

AndroidPermissions* AndroidPermissions::instance()
{
    static AndroidPermissions* inst = nullptr;
    if (!inst) {
        inst = new AndroidPermissions();
    }
    return inst;
}

AndroidPermissions::AndroidPermissions(QObject *parent) : QObject(parent)
{
}

void AndroidPermissions::requestBluetoothPermissions()
{
#ifdef Q_OS_ANDROID
    QStringList permissions;
    permissions << "android.permission.BLUETOOTH"
                << "android.permission.BLUETOOTH_ADMIN"
                << "android.permission.ACCESS_COARSE_LOCATION"
                << "android.permission.ACCESS_FINE_LOCATION";

    // Android 12+ permissions
    if (QNativeInterface::QAndroidApplication::sdkVersion() >= 31) {
        permissions << "android.permission.BLUETOOTH_SCAN"
                    << "android.permission.BLUETOOTH_CONNECT"
                    << "android.permission.BLUETOOTH_ADVERTISE";
    }

    // Use the public QtAndroid API to request permissions synchronously.
    // requestPermissionsSync returns a map of permission -> result.
    QHash<QString, QtAndroid::PermissionResult> results = QtAndroid::requestPermissionsSync(permissions);

    bool allGranted = true;
    for (const auto &perm : results.keys()) {
        if (results.value(perm) != QtAndroid::PermissionResult::Granted) {
            allGranted = false;
            break;
        }
    }

    if (allGranted) {
        emit permissionsGranted();
    } else {
        emit permissionsDenied();
    }
#else
    // Desktop - assume permissions granted
    emit permissionsGranted();
#endif
}

bool AndroidPermissions::hasBluetoothPermissions()
{
#ifdef Q_OS_ANDROID
    // Check at least one critical permission; return true only if all required permissions are authorized.
    QStringList checkPerms;
    checkPerms << "android.permission.BLUETOOTH"
               << "android.permission.BLUETOOTH_ADMIN"
               << "android.permission.ACCESS_COARSE_LOCATION"
               << "android.permission.ACCESS_FINE_LOCATION";

    if (QNativeInterface::QAndroidApplication::sdkVersion() >= 31) {
        checkPerms << "android.permission.BLUETOOTH_SCAN"
                   << "android.permission.BLUETOOTH_CONNECT"
                   << "android.permission.BLUETOOTH_ADVERTISE";
    }

    for (const QString &p : checkPerms) {
        if (QtAndroid::checkPermission(p) != QtAndroid::PermissionResult::Granted)
            return false;
    }
    return true;
#else
    return true; // Desktop
#endif
}
