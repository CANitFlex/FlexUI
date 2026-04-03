import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts 6.5
import BleGattClient 1.0
import "../../components"

import common.BLE

ColumnLayout {

    property DeviceScanner deviceScanner: BleGattClient.deviceScanner


    spacing: 8

    Header {
        topText: "Device Search"
        bottomText: "Connect to your RC car"
        Layout.fillWidth: true
    }

    RowLayout {
        Layout.fillWidth: true

        Button {
            id: btnSearch
            text: deviceScanner.isScanning ? "Stop Scan" : "Scan for Devices"
            icon.source: deviceScanner.isScanning ? "qrc:/icons/stop.svg" : "qrc:/icons/refresh.svg"
            Material.background: deviceScanner.isScanning ? Material.Red : Material.Blue
            Layout.preferredWidth: 200

            onClicked: {
                deviceScanner.isScanning ? deviceScanner.stopRequest() : deviceScanner.scanRequest()
            }
        }

        Item {
            Layout.fillWidth: true   // 👈 nimmt den ganzen Rest ein

            RowLayout {
                anchors.centerIn: parent   // 👈 dann wirklich zentriert
                spacing: 8

                Rectangle {
                    width: 10
                    height: 10
                    radius: 5
                    color: BleGattClient.isDeviceConnected ? "#4CAF50" : "#F44336"
                }

                Label {
                    text: BleGattClient.isDeviceConnected ? "Connected" : "Disconnected"
                    font.weight: Font.Medium
                    color: BleGattClient.isDeviceConnected ? "#4CAF50" : "#F44336"
                }
            }
        }
    }
}
