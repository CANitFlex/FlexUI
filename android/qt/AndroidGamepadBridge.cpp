#include "AndroidGamepadBridge.h"

AndroidGamepadBridge::AndroidGamepadBridge(QObject *parent) : QObject(parent) {
    // Initialize agent here
    agent = ControllerAgent::instance(this);

    // Connect signals
    connect(this, &AndroidGamepadBridge::leftStickChanged,
            agent, &ControllerAgent::onLeftStickChanged);
    connect(this, &AndroidGamepadBridge::rightStickChanged,
            agent, &ControllerAgent::onRightStickChanged);
}

// Fix the singleton implementation - add class scope
AndroidGamepadBridge* AndroidGamepadBridge::instance(QObject *parent) {
    static AndroidGamepadBridge* inst = nullptr;
    if (!inst) {
        inst = new AndroidGamepadBridge(parent);
    }
    return inst;
}

void AndroidGamepadBridge::setLandscape() {
    QJniObject::callStaticMethod<void>(
        "com/btb/AndroidCanController/GamepadBridge",
        "setLandscape",
        "()V"
    );
}

void AndroidGamepadBridge::setPortrait() {
    QJniObject::callStaticMethod<void>(
        "com/btb/AndroidCanController/GamepadBridge",
        "setPortrait",
        "()V"
    );
}

void AndroidGamepadBridge::setSensor() {
    QJniObject::callStaticMethod<void>(
        "com/btb/AndroidCanController/GamepadBridge",
        "setSensor",
        "()V"
    );
}

extern "C" {
JNIEXPORT void JNICALL
Java_com_btb_AndroidCanController_GamepadBridge_nativeTestJNI(JNIEnv *env, jclass clazz) {
    qDebug() << "JNI TEST - Native method called successfully!";
}

JNIEXPORT void JNICALL
Java_com_btb_AndroidCanController_GamepadBridge_nativeOnGamepadJoystick(JNIEnv *env, jclass clazz, jfloat lx, jfloat ly, jfloat rx, jfloat ry) {
    qDebug() << "Joystick callback - Left:" << lx << ly << "Right:" << rx << ry;

    // Update the GamepadController instance
    AndroidGamepadBridge::instance()->updateJoystick(lx, ly, rx, ry);
}
}
