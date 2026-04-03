import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Controls.Material 6.5
import QMLGamepad 1.0

QMLGamepadUI  {
    id: gamepadRoot


    anchors.fill: parent
    // 🔑 Skalierung
    property real scaleFactor: Math.min(width / 400, height / 250)

    property real buttonSize: 50 * scaleFactor
    property real spacingSize: 20 * scaleFactor
    property real marginSize: 50 * scaleFactor
}
