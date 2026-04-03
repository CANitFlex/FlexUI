#include "DeviceListModel.h"
#include "Device.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>

using namespace BLE ;

DeviceListModel::DeviceListModel(QObject* parent) : QAbstractListModel(parent) {

}

Device* DeviceListModel::getDeviceByIndex(int index) const
{
    return m_devices.at(index).get();
}

void DeviceListModel::setDeviceConnected(int index, bool isConnected)
{
    m_devices.at(index)->setConnected(isConnected);
    QModelIndex modelIndex = this->createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex);
}

void DeviceListModel::connect(int index)
{
    emit connectAttempt(m_devices.at(index)->getDeviceInfo(), index);
}

void DeviceListModel::disconnect(int index)
{
    emit disconnectAttempt(index);
}

void DeviceListModel::deviceDiscovered(const QBluetoothDeviceInfo &device) {
    // qDebug() << "deviceDiscovered:" << device.name() << device.address().toString();
    const int row = static_cast<int>(m_devices.size());
    beginInsertRows(QModelIndex(), row, row);
    m_devices.emplace_back(std::make_unique<Device>(device));
    endInsertRows();
    emit devicesChanged();
}

int DeviceListModel::rowCount(const QModelIndex &parent) const {
    return  m_devices.size();
}

QVariant DeviceListModel::data(const QModelIndex &index, int role) const {
    BLE::Device *ptr =  m_devices.at(index.row()).get();
    switch (role) {
    case DeviceName:
        return ptr->name();
    case AddressRole:
        return ptr->address();
    case RSSI:
        return ptr->rssi();
    case SignalStrength:
        return ptr->signalStrength();
    case IsConnected:
        return ptr->isConnected();
    default:
        return {};
    }
}

QHash<int, QByteArray> DeviceListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DeviceName] = "name";
    roles[AddressRole] = "address";
    roles[RSSI] = "rssi";
    roles[SignalStrength] = "signalStrength";
    roles[IsConnected] = "isConnected";
    return roles;
}
