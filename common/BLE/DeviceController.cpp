#include "DeviceController.h"

using namespace BLE;


struct DeviceController::Impl {

public:
    QString getNameByUuid(qint16 uuid) {
        return uuids[uuid];
    }
    QString uuidToHex(qint16 uuid) {
        return QString("0x%1").arg(uuid, 3, 16, QLatin1Char('0')).toUpper();
    }
    qint16 uuidToUuid16(const QBluetoothUuid& uuid){
        QString uuid16str = uuid.toString().mid(5, 4);
        return uuid16str.toInt(nullptr, 16);
    }


private:
    qint16 S_SERVO = 0x111;
    qint16 S_MOTOR = 0x222;

    qint16 C_MOTOR_LEFT = 0x223;
    qint16 C_MOTOR_RIGHT = 0x224;
    qint16 C_SERVO = 0x112;

    std::map<qint16, QString> uuids {
        {S_SERVO, "Service Servo"},
        {S_MOTOR, "Service Motor"},
        {C_MOTOR_LEFT, "Characteristic Motor Left"},
        {C_MOTOR_RIGHT, "Characteristic Motor Right"},
        {C_SERVO, "Characteristic Servo"}
    };
};

DeviceController::~DeviceController(){

}

DeviceController::DeviceController(QObject *parent)
    : QObject{parent}, d(std::make_unique<Impl>())
{}



void DeviceController::setDevice(Device* device)
{
    m_device = device;
}

void DeviceController::connectRequest(const QBluetoothDeviceInfo& info, int index) {
    qDebug() << "Request Attempt";
    m_deviceIndex = index;
    m_controller = QLowEnergyController::createCentral(info);

    connect(m_controller, &QLowEnergyController::connected, this, &DeviceController::onDeviceConnected);
    connect(m_controller, &QLowEnergyController::disconnected, this, &DeviceController::onDeviceDisconnected);
    connect(m_controller, &QLowEnergyController::discoveryFinished, this, &DeviceController::onServiceDiscoveryFinished);
    connect(m_controller, &QLowEnergyController::disconnected, this, &DeviceController::onDeviceDisconnected);
    m_controller->connectToDevice();
}

void DeviceController::disconnectRequest(int index) {
    m_device = nullptr;
    m_controller->disconnectFromDevice();

}

void DeviceController::onDeviceConnected() {
    qDebug() << "Device connected.";

    // Connect to discoveryFinished signal BEFORE starting discovery
    connect(m_controller, &QLowEnergyController::discoveryFinished, this, &DeviceController::onServiceScanDone);
    m_controller->discoverServices();
}

void DeviceController::onDeviceDisconnected() {
    qDebug() << "Device disconnected.";

    for (auto &kv : m_services) {
        if (kv.second) {
            kv.second->deleteLater();
        }
    }
    m_services.clear();
    m_characteristics.clear();
    m_controller.clear();
    m_device = nullptr;

    emit deviceDisconnected(m_deviceIndex);

}

void DeviceController::onServiceDiscoveryFinished(){
    emit deviceConnectedAndDiscoverd(m_deviceIndex);
}

void DeviceController::onServiceScanDone() {
    for (const auto &serviceUuid : m_controller->services()) {
        QLowEnergyService* service =  m_controller->createServiceObject(serviceUuid, this);
        auto uuid = d->uuidToUuid16(serviceUuid);
        m_services.emplace(uuid, service);
        service->discoverDetails();
        connectService(*service);
        qDebug() << "Service: " << d->getNameByUuid(uuid) << " as Hex: " << d->uuidToHex(uuid);
    }
}

void DeviceController::connectService(QLowEnergyService& service)
{
    for(const auto& characteristic : service.characteristics()) {
        //m_characteristics.emplace(uuidToUuid16(characteristic.uuid()), characteristic);
        auto uuid = d->uuidToUuid16(characteristic.uuid());
        qDebug() << "Characteristic: " << d->getNameByUuid(uuid) << " as Hex: " << d->uuidToHex(uuid);
        m_characteristics.emplace(uuid, &characteristic);
    }
}
