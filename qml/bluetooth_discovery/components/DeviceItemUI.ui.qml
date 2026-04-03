import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts 6.5
import "../../components"

Rectangle {
    // ✅ Top-Level: Flexible Width
    implicitHeight: 120
    implicitWidth: 400 // Fallback, wenn nicht in Layout


    // ✅ Daten als required property
    required property var modelData
    property alias connectButton: connectButton

    color: Material.background
    radius: 12
    border.width: 1
    property bool hovered: false

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: parent.hovered = true
        onExited: parent.hovered = false
    }

    // Shadow...
    Rectangle {
        anchors.fill: parent
        anchors.topMargin: 2
        color: parent.color
        radius: parent.radius
        opacity: parent.hovered ? 0.1 : 0.05
        z: -1
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        // Device icon
        Rectangle {
            width: 48
            height: 48
            radius: 24
            Layout.alignment: Qt.AlignVCenter

            BluetoothDeviceIcon {
                anchors.centerIn: parent
                deviceData: modelData
                font.pixelSize: 24
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            Text {
                id: _text
                text: modelData.name ?? "Unknown Device"
                font.pixelSize: 16
                font.weight: Font.Medium
                color: Material.foreground
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Button {
                id: connectButton
                text: "Connect"
                Layout.fillWidth: true
                Material.background: buttonBackground
            }
        }

        // ✅ Signal-Stärke oben rechts!
        Rectangle {
            width: 40
            height: 40
            radius: 20
            color: modelData.signalStrength

            Text {
                anchors.centerIn: parent
                text: modelData.signalStrength
                font.pixelSize: 18
                styleColor: "#ffffff"
            }
        }
    }

    // Connection status indicator
    Rectangle {
        width: 8
        height: 8
        radius: 4
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 8
    }
}
