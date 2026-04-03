#include "BLEGattClient.h"
#include "DeviceScanner.h"

using namespace BLE;


// Singleton implementation
BleGattClient* BleGattClient::instance(QObject *parent) {
    static BleGattClient* inst = nullptr;
    if (!inst) {
        inst = new BleGattClient(parent);
    }
    return inst;
}

BleGattClient::BleGattClient(QObject *parent)
    : QObject(parent),
    m_deviceScanner(new DeviceScanner(this)),
    m_deviceListModel(new DeviceListModel(this)),
    m_deviceController(new DeviceController(this))
{
    connect(m_deviceScanner, &DeviceScanner::deviceDiscovered, m_deviceListModel, &DeviceListModel::deviceDiscovered);

    connect(m_deviceListModel, &DeviceListModel::connectAttempt, m_deviceController, &DeviceController::connectRequest);
    connect(m_deviceListModel, &DeviceListModel::disconnectAttempt, m_deviceController, &DeviceController::disconnectRequest);

    connect(m_deviceController, &DeviceController::deviceConnectedAndDiscoverd, this, &BleGattClient::deviceControllerFinishedDiscovery);
    connect(m_deviceController, &DeviceController::deviceDisconnected, this, &BleGattClient::deviceDisconnected);

    connect(m_rateLimiter, &RateLimiter::dataReady, this, &BleGattClient::onDataReady);

}

BleGattClient::~BleGattClient()
{

}

void BleGattClient::deviceControllerFinishedDiscovery(int index)
{
    m_deviceListModel->setDeviceConnected(index, true);
}

void BleGattClient::deviceDisconnected(int index)
{
    m_deviceListModel->setDeviceConnected(index, false);
}

void BleGattClient::onDataReady(Enums::Services service, const QVariant& value) {
    qDebug() << service;
}

/*

void BleGattClient::sendCommand(QLowEnergyService* service, QLowEnergyCharacteristic characteristic, int value)
{
    if(service)
    {
        const auto services = controller->services();
        auto it = std::find(services.begin(), services.end(), service->serviceUuid());
        if (it != services.end()) {
            QByteArray byteArray;
            byteArray.append(value);
            auto service = controller->createServiceObject(*it);
            service->writeCharacteristic(characteristic, byteArray,  QLowEnergyService::WriteWithoutResponse);
            delete service;
        }
    }
}


void BleGattClient::sendCommand(Enums::Services serviceTyp, Enums::Characteristics characteristicTyp, int value)
{
    qDebug() << "ServiceTyp:" << serviceTyp << "characteristicTyp" << characteristicTyp << "value"<< value;
    switch (characteristicTyp) {
    case Enums::Characteristics::C_SERVO:
        sendCommand(m_servoService, m_servoCharacteristic, value);
        break;
    case Enums::Characteristics::C_MOTOR_LEFT:
        sendCommand(m_motorService, m_motor_leftCharacteristic, value);
        break;
    case Enums::Characteristics::C_MOTOR_RIGHT:
        sendCommand(m_motorService, m_motor_rightCharacteristic, value);
        break;
    default:
        break;
    }
}

*/
