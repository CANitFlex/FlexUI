#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QObject>
#include <QQmlEngine>
#include <QAbstractListModel>
#include <QBluetoothDeviceInfo>

#include "BLE.h"
#include <Device.h>

namespace BLE {


class DeviceListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        DeviceName,
        AddressRole,
        RSSI,
        SignalStrength,
        IsConnected
    };
    DeviceListModel(QObject* parent = nullptr);

    Q_INVOKABLE void connect(int);
    Q_INVOKABLE void disconnect(int);


    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;  // ← Rollenname für QML
    Device* getDeviceByIndex(int index) const;
    void setDeviceConnected(int index, bool isConnected);

signals:
    void devicesChanged();
    void connectAttempt(const QBluetoothDeviceInfo& device, int index);
    void disconnectAttempt(int index);

private:
    std::vector<BLE::Device_uPtr> m_devices;

public slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);

};

}

#endif // DEVICEMODEL_H
