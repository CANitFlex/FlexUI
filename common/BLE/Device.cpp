#include "Device.h"
#include <QBluetoothDeviceInfo>
#include <QBluetoothAddress>

using namespace BLE;

Device::Device(const QBluetoothDeviceInfo& info)
    : m_info(info){}

QString Device::name() const {
    return m_info.name();
}

QString Device::address() const {
    return m_info.address().toString();
}

qint16 Device::rssi() const {
    return m_info.rssi();  //
}

QString Device::signalStrength()
{
    if (rssi()  > -60) return "Excellent";
    if (rssi()  > -70) return "Good";
    if (rssi()  > -80) return "Fair";
    return "Weak";
}

QBluetoothDeviceInfo Device::getDeviceInfo() const {
    return m_info;
}

bool Device::isConnected() const {
    return m_isConnected;
}
void Device::setConnected(bool connected) {
    m_isConnected = connected;
}
