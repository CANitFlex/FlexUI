#include "ControllerAgent.h"
#include <QTimer>

ControllerAgent* ControllerAgent::instance(QObject *parent) {
    static ControllerAgent* inst = nullptr;
    if (!inst) {
        inst = new ControllerAgent(parent);
    }
    return inst;
}

ControllerAgent::ControllerAgent(QObject *parent) : QObject(parent) {
    bleClient = BleGattClient::instance(this);


    // Setup idle timeout timer
    idleTimer = new QTimer(this);
    idleTimer->setSingleShot(true);
    idleTimer->setInterval(IDLE_TIMEOUT_MS); // 10 seconds
    connect(idleTimer, &QTimer::timeout, this, &ControllerAgent::onIdleTimeout);

    // Start idle timer immediately
    idleTimer->start();
}

void ControllerAgent::onLeftStickChanged(float lx, float ly) {
    // ADD THIS: Reset idle timer on any stick movement
    resetIdleTimer();

    static constexpr float CHANGE_THRESHOLD = 0.01f;

    if (std::abs(lx - lastSentLeftX) > CHANGE_THRESHOLD ||
        std::abs(ly - lastSentLeftY) > CHANGE_THRESHOLD) {

        // qDebug() << "Left stick change detected:" << lx << ly << "vs last:" << lastSentLeftX << lastSentLeftY;

        bufferedLeftX = lx;
        bufferedLeftY = ly;
        hasLeftStickUpdate = true;

        sendBufferedData();
    }
}

void ControllerAgent::onRightStickChanged(float rx, float ry) {
    // ADD THIS: Reset idle timer on any stick movement
    resetIdleTimer();

    static constexpr float CHANGE_THRESHOLD = 0.01f;

    if (std::abs(rx - lastSentRightX) > CHANGE_THRESHOLD ||
        std::abs(ry - lastSentRightY) > CHANGE_THRESHOLD) {

        // qDebug() << "Right stick change detected:" << rx << ry << "vs last:" << lastSentRightX << lastSentRightY;

        bufferedRightX = rx;
        bufferedRightY = ry;
        hasRightStickUpdate = true;

        sendBufferedData();
    }
}

void ControllerAgent::resetIdleTimer() {
    if (idleTimer) {
        idleTimer->stop();
        idleTimer->start(); // Restart the 10-second countdown
    }
}

void ControllerAgent::sendStopCommands() {
    // Set all values to zero (start position)
    bufferedLeftX = 0.0f;
    bufferedLeftY = 0.0f;
    bufferedRightX = 0.0f;
    bufferedRightY = 0.0f;

    // Force send regardless of change threshold
    hasLeftStickUpdate = true;
    hasRightStickUpdate = true;

    // Send immediately
    sendBufferedData();

    qDebug() << "Stop commands sent - controller returned to start position";
}

void ControllerAgent::onIdleTimeout() {
    qDebug() << "Idle timeout reached - sending stop commands";

    // Send stop commands to return to "start" position
    sendStopCommands();

    // Reset the idle timer for next period
    idleTimer->start();
}

void ControllerAgent::sendBufferedData() {
    if (hasLeftStickUpdate) {
        qDebug() << "Sending left stick:" << bufferedLeftX << bufferedLeftY;
        steeringService.execute(QVariant::fromValue(bufferedLeftX));

        // Update last sent values
        lastSentLeftX = bufferedLeftX;
        lastSentLeftY = bufferedLeftY;
        hasLeftStickUpdate = false;
    }

    if (hasRightStickUpdate) {
        qDebug() << "Sending right stick:" << bufferedRightX << bufferedRightY;
        motorService.execute(QVariant::fromValue(bufferedRightY));

        // Update last sent values
        lastSentRightX = bufferedRightX;
        lastSentRightY = bufferedRightY;
        hasRightStickUpdate = false;
    }
}

void ControllerAgent::sendCommand(Enums::Services serviceType, const QString& command) {
    if(serviceType == Enums::Services::S_SERVO) {
        steeringService.execute(QVariant::fromValue(command));
    }
    else if(serviceType == Enums::Services::S_MOTOR) {
        motorService.execute(QVariant::fromValue(command));
    }
}
