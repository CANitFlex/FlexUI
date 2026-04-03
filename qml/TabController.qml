import QtQuick 6.5
import QtQuick.Controls.Material 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 6.5
import GamepadController 1.0
import "controller"
import "components"

ColumnLayout {

    QMLGamepad {
        id: gamepad
        width: parent.width
    }
}
