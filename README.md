# FlexUI 

A flexible Qt/QML-based Controller application.
Used as Bridge between a User Device (Linux Desktop or Android Phone) and a BLEGatt Server

## Currently under heavy restruction

- Shared Data aka Configuration
- Test SetUp
- QML Revision
- Structering

## Features

## Current Project Structure

- **Common**: Shared C++ Code, Logic Layer, Platform Independent
- - **Service**
- - **BLE**
- **QML**: Shared device UI (Android/Linux, maybe others in Future)
- **Android**: Android Bridge for Gamepad and Permissions
- **SDL**: Gamepad Bridge for Desktops


## Requirements

### Development
- **Qt 6.x** (Qt 6.9)
  - **Qt Bluetooth module** (required for BLE support)
- **CMake 3.16+**
- **C++17 compatible compiler** (GCC, Clang, MSVC)
- **Ninja** (recommended build tool, optional)
- **SDL 2.0+** (optional, required only for Linux desktop gamepad support)

### Android Build
- **Android NDK** (r23 or later recommended)
- **Android SDK** (API level 24+)
- **Gradle** (bundled in android folder)
- **Java Development Kit** (JDK 11+)

### Runtime (Mobile)
- **Android 7.0+** (API level 24+)
- **Bluetooth 4.1+** capable device
- CAN controller/adapter support

### Runtime (Desktop)
- **Linux**: Qt 6.x libraries, SDL 2.0 (for gamepad, optional)

## Building

### Desktop Build (Linux/macOS/Windows)

The simplest setup is to use Qt Creator directly.

Alternatively, you can use the provided build script. The default Qt installation path is `/opt/qt/6.9.0/gcc_64`.

**Using the build script:**
```sh
chmod +x build.sh
./build.sh Debug
```

Or specify the Qt path explicitly:
```sh
chmod +x build.sh
./build.sh Debug /opt/qt/6.9.0/gcc_64
```

**Required Qt packages:**

See CMakeLists.txt for the required components:
- `Qt${QT_VERSION_MAJOR}::Core`
- `Qt${QT_VERSION_MAJOR}::Quick`
- `Qt${QT_VERSION_MAJOR}::Bluetooth`

### Android Build

Currently, use Qt Creator for Android builds. The setup is device-specific and includes NDK configuration, SDK paths, and device settings that are easier to manage through Qt Creator's IDE.


## Usage

1. Launch the application
2. Navigate to the **Bluetooth Discovery** tab
3. Scan for available BLE devices
4. Select and connect to your CAN controller device
5. Switch to the **Controller** tab to send commands
6. Use gamepad or touch controls to interact with connected devices

## Android Permissions

The application requires the following permissions (see `AndroidManifest.xml`):
- `BLUETOOTH` - For Bluetooth connectivity
- `BLUETOOTH_ADMIN` - For device scanning
- `BLUETOOTH_SCAN` - For BLE scanning (Android 12+)
- `BLUETOOTH_CONNECT` - For establishing connections (Android 12+)
- `ACCESS_FINE_LOCATION` - For BLE device discovery on older Android versions

## Development Notes

- **BLE Device Management**: Handled by `DeviceController` and `DeviceScanner`
- **Rate Limiting**: `RateLimiter` prevents command flooding
- **Android Bridge**: `AndroidGamepadBridge` provides native gamepad support
- **Services**: Motor and steering control via pluggable service pattern

## License

See LICENSE file for details.

## Contributing

Contributions are welcome! Please follow the existing code style and submit pull requests to the main branch.
