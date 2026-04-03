#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QQmlEngine>

#include <QBluetoothDeviceInfo>


namespace BLE {


class Device
{

public:
    Device(const QBluetoothDeviceInfo& info);

    QString name() const;
    QString address() const;
    qint16 rssi() const;

    QString signalStrength();
    QBluetoothDeviceInfo getDeviceInfo() const;

    bool isConnected() const;
    void setConnected(bool);


private:
    const QBluetoothDeviceInfo m_info;
    bool m_isConnected = false;

};

}

#endif // DEVICE_H
