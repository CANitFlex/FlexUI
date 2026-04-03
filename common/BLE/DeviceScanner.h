#ifndef DEVICESCANNER_H
#define DEVICESCANNER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QPointer>
#include <qqmlintegration.h>

namespace BLE {


class DeviceScanner : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool isScanning MEMBER mIsScanning NOTIFY isScanningChanged FINAL)
public:
    explicit DeviceScanner(QObject *parent = nullptr);

signals:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void isScanningChanged();

public slots:
    void scanRequest();
    void stopRequest();

private:
    QMetaObject::Connection disoveryConnection;
    QMetaObject::Connection disoveryFinished;
    QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    bool mIsScanning = false;
};

}

#endif // DEVICEDISCOVER_H
