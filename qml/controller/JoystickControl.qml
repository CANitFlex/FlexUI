import QtQuick 6.5

Item {
    id: joystickRoot
    width: 100 * scaleFactor
    height: 100 * scaleFactor
    property real stickX: 0.0  // -1.0 to 1.0
    property real stickY: 0.0  // -1.0 to 1.0
    property real maxRadius: 80

    signal stickMoved(real x, real y)

    function reset() {
        stickX = 0.0
        stickY = 0.0
        knob.x = joystickRoot.width/2 - knob.width/2
        knob.y = joystickRoot.height/2 - knob.height/2
        stickMoved(0.0, 0.0)
    }

    // Outer circle (base)
    Rectangle {
        id: base
        anchors.centerIn: parent
        width: 120 * scaleFactor
        height: 120 * scaleFactor
        radius: 60
        color: "#34495e"
        border.color: "#2c3e50"
        border.width: 2

        // Inner guide circle
        Rectangle {
            anchors.centerIn: parent
            width: 60
            height: 60
            radius: 30
            color: "transparent"
            border.color: "#7f8c8d"
            border.width: 1
            opacity: 0.5
        }
    }

    // Draggable knob
    Rectangle {
        id: knob
        width: 40
        height: 40
        radius: 20
        color: "#3498db"
        border.color: "#2980b9"
        border.width: 1

        x: joystickRoot.width/2 - width/2
        y: joystickRoot.height/2 - height/2

        // Visual feedback when pressed
        scale: mouseArea.pressed ? 1.2 : 1.0
        Behavior on scale {
            NumberAnimation { duration: 100 }
        }
    }

    // Mouse area that covers the entire joystick for better interaction
    MouseArea {
        id: mouseArea
        anchors.fill: parent

        property bool isDragging: false

        onPressed: function(mouse) {
            isDragging = true
            // Move knob to mouse position immediately
            updateKnobPosition(mouse.x, mouse.y)
        }

        onPositionChanged: function(mouse) {
            if (isDragging) {
                updateKnobPosition(mouse.x, mouse.y)
            }
        }

        onReleased: function(mouse) {
            isDragging = false
            resetJoystick()
        }

        onCanceled: {
            isDragging = false
            resetJoystick()
        }

        function resetJoystick() {
            knob.x = joystickRoot.width/2 - knob.width/2
            knob.y = joystickRoot.height/2 - knob.height/2
            stickX = 0.0
            stickY = 0.0
            stickMoved(90.0, 0.0)
            updateKnobPosition(joystickRoot.width/2, joystickRoot.height/2)
        }

        function updateKnobPosition(mouseX, mouseY) {
            // Calculate distance from center
            var centerX = joystickRoot.width/2
            var centerY = joystickRoot.height/2
            var deltaX = mouseX - centerX
            var deltaY = mouseY - centerY
            var distance = Math.sqrt(deltaX * deltaX + deltaY * deltaY)

            // Limit to maxRadius
            if (distance > maxRadius) {
                var angle = Math.atan2(deltaY, deltaX)
                deltaX = Math.cos(angle) * maxRadius
                deltaY = Math.sin(angle) * maxRadius
            }

            // Set knob position
            knob.x = centerX + deltaX - knob.width/2
            knob.y = centerY + deltaY - knob.height/2

            // Convert to -1.0 to 1.0 range
            stickX = deltaX / maxRadius
            stickY = deltaY / maxRadius

            // Clamp values
            stickX = Math.max(-1.0, Math.min(1.0, stickX))
            stickY = Math.max(-1.0, Math.min(1.0, stickY))

            stickMoved(stickX, stickY)
        }
    }

    // Center cross indicator
    Rectangle {
        anchors.centerIn: parent
        width: 2
        height: 10
        color: "#95a5a6"
        opacity: 0.7
    }

    Rectangle {
        anchors.centerIn: parent
        width: 10
        height: 2
        color: "#95a5a6"
        opacity: 0.7
    }
}
