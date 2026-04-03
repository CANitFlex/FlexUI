#include "DeviceScanner.h"
#include <QDebug>
#include <QBluetoothDeviceDiscoveryAgent>


using namespace BLE;

DeviceScanner::DeviceScanner(QObject *parent)
    : QObject{parent}
    , discoveryAgent(nullptr)
{}

void DeviceScanner::scanRequest()  {
    // Prevent starting another scan while already scanning
    if (mIsScanning) {
        qDebug() << "scanRequest(): already scanning";
        return;
    }

    // Create discovery agent if missing
    if (!discoveryAgent) {
        discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    } else {
        // If an existing agent is active, don't start again
        // Should not happen
        if (discoveryAgent->isActive()) {
            qDebug() << "scanRequest(): discoveryAgent already active";
            mIsScanning = true; // keep state consistent
            emit isScanningChanged();
            return;
        }
    }

    // Disconnect previous connections (if any) to avoid duplicate connections
    QObject::disconnect(disoveryConnection);
    QObject::disconnect(disoveryFinished);

    // Connect signals BEFORE starting the discovery to avoid missing early events
    disoveryConnection = connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &DeviceScanner::deviceDiscovered);
    disoveryFinished = connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &DeviceScanner::stopRequest);

    mIsScanning = true;
    discoveryAgent->start();
    qDebug() << "Starting discover BLE Devices";
    emit isScanningChanged();
}

void DeviceScanner::stopRequest() {
    // If not scanning and no active agent, nothing to do
    // Not possible in reality ...
    if (!mIsScanning && (!discoveryAgent || !discoveryAgent->isActive())) {
        qDebug() << "stopRequest(): not scanning";
        return;
    }

    mIsScanning = false;

    // Disconnect connections
    QObject::disconnect(disoveryConnection);
    QObject::disconnect(disoveryFinished);

    if (discoveryAgent) {
        if (discoveryAgent->isActive())
            discoveryAgent->stop();
        discoveryAgent->deleteLater();
        discoveryAgent = nullptr;
    }

    qDebug() << "Stopping discover BLE Devices";
    emit isScanningChanged();
}
