#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#ifdef ANDROID_PLATFORM
#include "android/qt/AndroidGamepadBridge.h"
#include <QJniObject>
#include "android/qt/AndroidPermissions.h"
#elif defined(LINUX_PLATFORM)
#include "sdl/SDLGamepadBridge.h"
#endif

#include "qml/controller/QMLGamepad.h"
#include "common/BLE/BLEGattClient.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Register BLE client singleton for QML
    qmlRegisterSingletonType<BleGattClient>("BleGattClient", 1, 0, "BleGattClient", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return BleGattClient::instance();
        });

    // Register QML Gamepad singleton for QML
    qmlRegisterSingletonType<QMLGamepad>("QMLGamepad", 1, 0, "QMLGamepad", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return QMLGamepad::instance();
        });



#ifdef ANDROID_PLATFORM
    // Android-specific initialization
    const char* gamepadBridgeClass = "com/btb/AndroidCanController/GamepadBridge";
    QJniObject::callStaticMethod<void>(gamepadBridgeClass, "init");
    
    // Register Android gamepad controller for QML
    qmlRegisterSingletonType<AndroidGamepadBridge>("GamepadController", 1, 0, "GamepadController", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return AndroidGamepadBridge::instance();
        });

    // Register AndroidPermissions singleton for QML
    qmlRegisterSingletonType<AndroidPermissions>("AndroidPermissions", 1, 0, "AndroidPermissions",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return AndroidPermissions::instance();
        });

    AndroidGamepadBridge::instance()->setPortrait();
        
#elif defined(LINUX_PLATFORM)
    // Linux-specific initialization (SDL)
    qDebug() << "Initializing Linux gamepad support";
    
    // Register SDL gamepad controller for QML
    qmlRegisterSingletonType<SDLGamepadBridge>("GamepadController", 1, 0, "GamepadController", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return SDLGamepadBridge::instance();
        });


#else
    // Desktop/other platforms - use QML gamepad as the main controller
    qmlRegisterSingletonType<QMLGamepad>("GamepadController", 1, 0, "GamepadController", 
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return QMLGamepad::instance();
        });
#endif

    qDebug() << "Gamepad bridges initialized";

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
