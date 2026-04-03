import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts 6.5
import BleGattClient 1.0
import "components"

import "bluetooth_discovery/sections" as BTD_Sections
import "bluetooth_discovery/components" as BTD_components


ColumnLayout {
    id: root

    property bool isAndroid: Qt.platform.os === "android"


    BTD_Sections.Header {
        width: parent.width
    }

    BTD_Sections.Body {
        width: parent.width
    }

    Pane {
        Component.onCompleted: {
            if(isAndroid){
            // Request permissions when tab loads
                if (!AndroidPermissions.hasBluetoothPermissions()) {
                    AndroidPermissions.requestBluetoothPermissions()
                }
            }
        }
        /**
          Does not work at the moment
          */
        Connections {
            target: isAndroid ? AndroidPermissions : null

            function onPermissionsGranted() {
                console.log("Bluetooth permissions granted!")
                // Now you can scan for devices
            }

            function onPermissionsDenied() {
                console.log("Bluetooth permissions denied!")
                // Show error message
            }
        }
    }
}
