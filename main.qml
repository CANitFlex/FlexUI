import QtQuick 6.5
import QtQuick.Window 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 6.5
import BleGattClient 1.0
import GamepadController
import "qml/controller"
import "qml"

ApplicationWindow {

    id: root
    visible: true
    // Make window adapt to the screen size
    // Pixel 7a: 1080x2400 @ ~2.75 DPR → ~393x873 DIPs

    property bool isMobile : Qt.platform.os === "android" || Qt.platform === "ios";

    width: isMobile ? Screen.width : 480
    height: isMobile ? Screen.height : 720
    
    title: "CanACar Controller"
    
    // Material theme configuration
    Material.theme: Material.Dark
    Material.accent: Material.Blue
    Material.primary: Material.DeepPurple
    Material.background: "#121212"
    
    // Custom colors
    readonly property color surfaceColor1: Qt.rgba(1, 1, 1, 0.05)  // 5% white
    readonly property color surfaceColor2: Qt.rgba(1, 1, 1, 0.08)  // 8% white
    readonly property color surfaceColor3: Qt.rgba(1, 1, 1, 0.11)  // 11% white
    
    // Shadow effect for elevated surfaces

    Pane {
        id: background
        anchors.fill: parent
        Material.background: root.Material.background
        anchors.topMargin: 15


        Column {
            width: parent.width
            height: parent.height
            spacing: 0

            SwipeView {
                height: parent.height - tabBar.height
                width:  parent.width
                id: swipeView
                currentIndex: tabBar.currentIndex
                interactive: false
                clip: true // Add this to prevent content overflow


                TabBluetoothDiscovery {
                }

                TabController {

                }
            }

            TabBar {
                id: tabBar
                width: parent.width
                Material.background: surfaceColor3
                currentIndex: swipeView.currentIndex // Sync with SwipeView

                TabButton {
                    text: "Device Search"
                }
                TabButton {
                    text: "Gamepad"
                }
            }
        }
    }
}
