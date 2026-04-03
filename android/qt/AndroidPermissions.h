#pragma once

#include <QObject>

class AndroidPermissions : public QObject
{
    Q_OBJECT
    
public:
    static AndroidPermissions* instance();
    
    Q_INVOKABLE void requestBluetoothPermissions();
    Q_INVOKABLE bool hasBluetoothPermissions();
    
signals:
    void permissionsGranted();
    void permissionsDenied();
    
private:
    explicit AndroidPermissions(QObject *parent = nullptr);
};