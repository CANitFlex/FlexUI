qt_add_library(ble_lib STATIC)

qt_add_qml_module(ble_lib
    URI common.BLE
    VERSION 1.0
    QML_FILES
    SOURCES
        BLEGattClient.cpp
        DeviceListModel.cpp
        Device.cpp
        DeviceScanner.cpp
        BLEGattClient.h
        DeviceListModel.h
        Device.h
        DeviceScanner.h
        DeviceController.h DeviceController.cpp
        DeviceCommandExecuter.h DeviceCommandExecuter.cpp
)

target_include_directories(ble_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ble_lib
PUBLIC
    Qt${QT_VERSION_MAJOR}::Bluetooth
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
)
