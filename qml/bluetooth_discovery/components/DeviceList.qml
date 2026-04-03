import QtQuick 2.15

import BleGattClient 1.0
import common.BLE

ListView {
    property DeviceListModel deviceModel : BleGattClient.deviceListModel

    id: deviceListView
    anchors.fill: parent
    anchors.margins: 12
    model: deviceModel
    spacing: 14
    clip: true

    delegate: DeviceItem {
        width: parent.width
        index: modelData.index
        isConnected : modelData.isConnected

        onConnectAttemp: (idx) => {
             deviceModel.connect(idx)
        }

        onDisconnectAttemp: (idx) => {
             deviceModel.disconnect(idx)
         }

    }
}
