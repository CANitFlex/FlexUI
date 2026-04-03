import QtQuick 6.5
import QtQuick.Controls 6.5

Text {
    property var deviceData: null  // Changed from modelData to deviceData

    text: {
        if (!deviceData) return "📡"  // Safety check
        
        let deviceName = (deviceData.name || "").toLowerCase()

        // Audio devices
        if (deviceName.includes("headphone") || deviceName.includes("airpod") ||
            deviceName.includes("emberton") || deviceName.includes("dockin") ||
            deviceName.includes("pixels") || deviceName.includes("dockin") ||
            deviceName.includes("earphone") || deviceName.includes("earbud") ||
            deviceName.includes("headset") || deviceName.includes("speaker") ||
            deviceName.includes("audio") || deviceName.includes("beats") ||
            deviceName.includes("sony") || deviceName.includes("bose")) {
            return "🎧"
        }

        // RC Car / Vehicle devices
        else if (deviceName.includes("car") || deviceName.includes("vehicle") ||
                deviceName.includes("rc") || deviceName.includes("remote") ||
                deviceName.includes("auto") ||
                deviceName.includes("drive") || deviceName.includes("motor")) {
            return "🚗"
        }

        // Mouse devices
        else if (deviceName.includes("mouse") || deviceName.includes("trackpad") ||
                deviceName.includes("pointer")) {
            return "🖱️"
        }

        // Keyboard devices
        else if (deviceName.includes("keyboard") || deviceName.includes("key")) {
            return "⌨️"
        }

        // Gaming controllers
        else if (deviceName.includes("controller") || deviceName.includes("gamepad") ||
                deviceName.includes("joystick") || deviceName.includes("xbox") ||
                deviceName.includes("playstation") || deviceName.includes("ps4") ||
                deviceName.includes("ps5") || deviceName.includes("nintendo")) {
            return "🎮"
        }

        // Fitness/Health devices
        else if (deviceName.includes("watch") || deviceName.includes("fitness") ||
                deviceName.includes("heart") || deviceName.includes("band") ||
                deviceName.includes("tracker") || deviceName.includes("health")) {
            return "⌚"
        }

        // Phone/Mobile devices
        else if (deviceName.includes("phone") || deviceName.includes("mobile") ||
                deviceName.includes("iphone") || deviceName.includes("android") ||
                deviceName.includes("samsung") || deviceName.includes("xiaomi")) {
            return "📱"
        }

        // Computer/Laptop devices
        else if (deviceName.includes("computer") || deviceName.includes("laptop") ||
                deviceName.includes("pc") || deviceName.includes("macbook") ||
                deviceName.includes("imac") || deviceName.includes("desktop")) {
            return "💻"
        }

        // Printer devices
        else if (deviceName.includes("printer") || deviceName.includes("print")) {
            return "🖨️"
        }

        // Camera devices
        else if (deviceName.includes("camera") || deviceName.includes("cam") ||
                deviceName.includes("webcam")) {
            return "📷"
        }

        // Smart home devices
        else if (deviceName.includes("smart") || deviceName.includes("home") ||
                deviceName.includes("alexa") || deviceName.includes("google") ||
                deviceName.includes("hub") || deviceName.includes("assistant") ||
                deviceName.includes("echo")) {
            return "🏠"
        }

        // Generic Bluetooth device
        else {
            return "📡"
        }
    }
    font.pixelSize: 24
}
