import QtQuick 2.15

import common.BLE
import QtQuick.Controls.Material 6.5

DeviceItemUI {
    required property int index
    required property bool isConnected
    property var buttonBackground : isConnected ? Material.Green : Material.Blue

    index : index

    signal connectAttemp(int index)
    signal disconnectAttemp(int index)

    connectButton.onClicked: isConnected ? disconnectAttemp(index) : connectAttemp(index)

    connectButton.text: isConnected ? "Disconnect" : "Connect"

}
