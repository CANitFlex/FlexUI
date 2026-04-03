#ifndef ANDROIDGAMEPADBRIDGE_H
#define ANDROIDGAMEPADBRIDGE_H

#include <QDebug>
#include <QJniObject>
#include <QObject>
#include "../../common/ControllerAgent.h"

class AndroidGamepadBridge: public QObject {
    Q_OBJECT

public:
    static AndroidGamepadBridge* instance(QObject *parent = nullptr);

    // Method to call from JNI
    void updateJoystick(float lx, float ly, float rx, float ry) {
        emit joystickChanged(lx, ly, rx, ry);
        emit leftStickChanged(lx, ly);
        emit rightStickChanged(rx, ry);
    }

    Q_INVOKABLE void setLandscape();
    Q_INVOKABLE void setPortrait();
    Q_INVOKABLE void setSensor();

signals:
    void joystickChanged(float lx, float ly, float rx, float ry);
    void leftStickChanged(float lx, float ly);
    void rightStickChanged(float rx, float ry);

private:
    explicit AndroidGamepadBridge(QObject *parent = nullptr);
    ControllerAgent *agent = nullptr;
};

#endif // ANDROIDGAMEPADBRIDGE_H
