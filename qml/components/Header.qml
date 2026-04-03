import QtQuick 2.15
import QtQuick.Controls 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Layouts 6.5

Pane {
    property string topText: "TopText"
    property string bottomText: "BottomText"

    id: headerPane
    Material.elevation: 3
    Material.background: surfaceColor2

    Column {
        id: headerContent
        anchors.centerIn: parent

        Label {
            text: topText
            font.weight: Font.Medium
            color: Material.foreground
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: bottomText
            color: Material.foreground
            opacity: 0.7
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
