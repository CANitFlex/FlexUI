#ifndef BLE_GATT_CLIENT_H
#define BLE_GATT_CLIENT_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>
#include <QVariantList>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QPointer>

#include "BLE.h"

#include "../RateLimiter.h"

#include "DeviceScanner.h"
#include "DeviceListModel.h"
#include "DeviceController.h"

/*

*/

namespace BLE {

class BleGattClient : public QObject {
    Q_OBJECT

    Q_PROPERTY(DeviceScanner* deviceScanner MEMBER m_deviceScanner NOTIFY deviceScannerChanged)
    Q_PROPERTY(DeviceListModel* deviceListModel MEMBER m_deviceListModel NOTIFY deviceListModelChanged)


public:
    // Singleton pattern
    static BleGattClient* instance(QObject *parent = nullptr);

    // Delete copy constructor and assignment operator
    BleGattClient(const BleGattClient&) = delete;
    BleGattClient& operator=(const BleGattClient&) = delete;

    ~BleGattClient();
    //Q_INVOKABLE void sendCommand(Enums::Services serviceTyp, Enums::Characteristics characteristicTyp, int value);

    bool isScanning() const;
    bool isDeviceConnected() const;

signals:
    void deviceScannerChanged();
    void deviceListModelChanged();

private slots:
    void deviceControllerFinishedDiscovery(int index);
    void deviceDisconnected(int index);
    void onDataReady(Enums::Services service, const QVariant& value);


private:
    // Private constructor for singleton
    explicit BleGattClient(QObject *parent = nullptr);

    DeviceScanner* m_deviceScanner;
    DeviceListModel* m_deviceListModel;
    DeviceController* m_deviceController;

    RateLimiter* m_rateLimiter = RateLimiter::instance(this);
};

}

#endif // BLE_GATT_CLIENT_H
