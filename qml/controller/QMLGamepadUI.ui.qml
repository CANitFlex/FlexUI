import QtQuick 6.5
import QtQuick.Controls

Rectangle {

    color: "#2c3e50"
    radius: 15
    border.color: "#34495e"
    border.width: 2

    Column {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: marginSize
        spacing: spacingSize

        JoystickControl {
            width: 100 * scaleFactor
        }

        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 8 * scaleFactor

            Text {
                text: "STEERING"
                color: "white"
                font.bold: true
                font.pixelSize: 10 * scaleFactor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Row {
                spacing: 5 * scaleFactor

                Button {
                    text: "⬅️"
                    width: buttonSize
                    height: buttonSize
                    Material.background: "#3498db"
                }

                Button {
                    text: "🎯"
                    width: buttonSize
                    height: buttonSize
                }

                Button {
                    text: "➡️"
                    width: buttonSize
                    height: buttonSize
                    Material.background: "#3498db"
                }
            }
        }
    }

    // RIGHT SIDE
    Column {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: marginSize
        spacing: spacingSize

        JoystickControl {
            width: 100 * scaleFactor
        }

        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 8 * scaleFactor

            Text {
                text: "MOTOR"
                color: "white"
                font.bold: true
                font.pixelSize: 10 * scaleFactor
            }

            Row {
                spacing: 5 * scaleFactor

                Button {
                    text: "⬆️"
                    width: buttonSize
                    height: buttonSize
                    Material.background: "#27ae60"
                }

                Button {
                    text: "🛑"
                    width: buttonSize
                    height: buttonSize
                    Material.background: Material.Red
                }

                Button {
                    text: "⬇️"
                    width: buttonSize
                    height: buttonSize
                    Material.background: "#e67e22"
                }
            }
        }
    }

    // CENTER INFO
    Column {
        anchors.centerIn: parent
        spacing: 8 * scaleFactor

        Text {
            text: "CAN Controller"
            color: "white"
            font.bold: true
            font.pixelSize: 16 * scaleFactor
        }

        Button {
            text: "🚨 EMERGENCY"
            width: 120 * scaleFactor
            height: 35 * scaleFactor
            Material.background: Material.DeepOrange
        }
    }
}
