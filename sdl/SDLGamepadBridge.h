#ifndef SDLGAMEPADBRIDGE_H
#define SDLGAMEPADBRIDGE_H

#include <QObject>
#include <QQmlEngine>
#include "../common/ControllerAgent.h"
#include "../common/ConnectionHandler.h"
#include <QDebug>
#include <SDL.h>
#include <QTimer>
class SDLGamepadBridge: public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    static SDLGamepadBridge* instance(QObject *parent = nullptr);
    
    // QML singleton factory method
    static SDLGamepadBridge* create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)
        return instance();
    }

public slots:  // Make it a public slot
    void connectToController();
    void disconnectFromController();

signals:
    void joystickChanged(float lx, float ly, float rx, float ry);
    void leftStickChanged(float lx, float ly);
    void rightStickChanged(float rx, float ry);

private slots:
    void onUpdate();

private:
    enum Connections{
        TIMER_UPDATE,
        LEFT_STICK_CHANGED,
        RIGHT_STICK_CHANGED
    };
    QTimer* mTimer = nullptr;  // Initialize to nullptr
    SDL_GameController* mController = nullptr;  // Initialize to nullptr
    ControllerAgent *agent = nullptr;
    ConnectionHandler connectionHandler;

    explicit SDLGamepadBridge(QObject *parent = nullptr);

    static constexpr int DEADZONE = 500;
    constexpr float normalLizeStickMoveMent(Sint16 val)
    {
        return (std::abs(val) > DEADZONE) ? val / 32767.0f : 0.0f;
    }

    constexpr float getStickMovement(SDL_GameControllerAxis ev)
    {
        return normalLizeStickMoveMent(SDL_GameControllerGetAxis(mController, ev));
    }


};

#endif // SDLGAMEPADBRIDGE_H
