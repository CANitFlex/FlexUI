#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include <QLowEnergyController>

namespace BLE {

class Device;

class DeviceController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit DeviceController(QObject *parent = nullptr);
    ~DeviceController();

    static qint16 uuidToUuid16(const QBluetoothUuid& uuid);

    void setDevice(Device* device);

signals:
    void deviceDisconnected(int index);
    void deviceConnectedAndDiscoverd(int index);

public slots:
    void disconnectRequest(int index);
    void connectRequest(const QBluetoothDeviceInfo& info, int index);

private slots:
    void onDeviceConnected();
    void onDeviceDisconnected();
    void onServiceDiscoveryFinished();

    void onServiceScanDone();

private:

    struct Impl;
    std::unique_ptr<Impl> d;

    Device* m_device;
    int m_deviceIndex;

    QString uuidToHex(qint16 uuid);
    QPointer<QLowEnergyController> m_controller;


    std::map<qint16, QLowEnergyService*> m_services;
    std::map<qint16, const QLowEnergyCharacteristic*> m_characteristics;

    void connectService(QLowEnergyService& service);

};

}

#endif // DEVICECONTROLLER_H
