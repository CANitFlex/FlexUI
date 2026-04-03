#include "SDLGamepadBridge.h"

SDLGamepadBridge::SDLGamepadBridge(QObject *parent) : QObject(parent) {
    qDebug() << "Initialize SDLGamepadBridge";
    agent = ControllerAgent::instance(this);

    // Initialize SDL
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        qDebug() << "SDL konnte nicht initialisiert werden:" << SDL_GetError();
        return;
    }
}

SDLGamepadBridge* SDLGamepadBridge::instance(QObject *parent) {
    static SDLGamepadBridge* inst = nullptr;
    if (!inst) {
        inst = new SDLGamepadBridge(parent);
    }
    return inst;
}

void SDLGamepadBridge::connectToController() {
    qDebug() << "Attempting to connect to controller...";
    
    // 1. FIRST: Disconnect any existing connections
    disconnectFromController();
    
    // 2. Create timer if it doesn't exist
    if (!mTimer) {
        mTimer = new QTimer(this);
    }
    
    // 3. Add fresh connections
    connectionHandler.addConnection(Connections::TIMER_UPDATE, 
        connect(mTimer, &QTimer::timeout, this, &SDLGamepadBridge::onUpdate));
    connectionHandler.addConnection(Connections::LEFT_STICK_CHANGED, 
        connect(this, &SDLGamepadBridge::leftStickChanged,
                agent, &ControllerAgent::onLeftStickChanged));
    connectionHandler.addConnection(Connections::RIGHT_STICK_CHANGED,
        connect(this, &SDLGamepadBridge::rightStickChanged,
                agent, &ControllerAgent::onRightStickChanged));

    // 4. Check for controllers
    if (SDL_NumJoysticks() < 1) {
        qDebug() << "Kein Controller angeschlossen.";
        return;
    }

    if (SDL_IsGameController(0)) {
        mController = SDL_GameControllerOpen(0);
        if (mController) {
            qDebug() << "Controller verbunden:" << SDL_GameControllerName(mController);
            mTimer->start(100);  // Start polling
            // emit connectionChanged(); // Uncomment if you have this signal
        } else {
            qDebug() << "Konnte Controller nicht öffnen:" << SDL_GetError();
        }
    } else {
        qDebug() << "Device is not a game controller";
    }
}

void SDLGamepadBridge::disconnectFromController() {
    qDebug() << "Disconnecting SDL gamepad...";
    
    // 1. Send stop commands FIRST
    if (agent) {
        // Send zeros to stop the car
        emit leftStickChanged(0.0f, 0.0f);
        emit rightStickChanged(0.0f, 0.0f);
        
        // Give agent time to send stop commands
        //QThread::msleep(100);
        
        // Disconnect agent
        //agent->disconnectController();
    }
    
    // 2. Stop the timer
    if (mTimer && mTimer->isActive()) {
        mTimer->stop();
    }
    
    // 3. Disconnect signals
    connectionHandler.disconnectAll();
    
    // 4. Close SDL controller
    if (mController) {
        SDL_GameControllerClose(mController);
        mController = nullptr;
    }
    
    // 5. Emit connection status change
    //emit connectionChanged();
    
    qDebug() << "SDL gamepad safely disconnected";
}

void SDLGamepadBridge::onUpdate() {
    if (!mController) return;

    constexpr int DEADZONE = 500;

    // Update SDL events
    SDL_PumpEvents();

    // Get both axes for left stick
    float leftX = getStickMovement(SDL_CONTROLLER_AXIS_LEFTX);
    float leftY = getStickMovement(SDL_CONTROLLER_AXIS_LEFTY);

    // Get both axes for right stick
    float rightX = getStickMovement(SDL_CONTROLLER_AXIS_RIGHTX);
    float rightY = getStickMovement(SDL_CONTROLLER_AXIS_RIGHTY);

    // Always emit current values (even if zero due to deadzone)
    emit leftStickChanged(leftX, leftY);
    emit rightStickChanged(rightX, rightY);
}
