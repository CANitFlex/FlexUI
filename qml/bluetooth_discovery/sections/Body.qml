import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts 6.5

import "../components" as BTD_components


ColumnLayout {
    width: parent.width

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        color: surfaceColor2
        radius: 12
        border.color: "#cccccc"
        border.width: 1
        BTD_components.DeviceList {
            Layout.fillWidth: true
        }
    }
}
