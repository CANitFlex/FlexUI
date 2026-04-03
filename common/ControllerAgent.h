#ifndef CONTROLLERAGENT_H
#define CONTROLLERAGENT_H

#include <QObject>
#include <QTimer>
#include "BLEGattClient.h"
#include "Service/MotorService.h"
#include "Service/SteeringService.h"
#include "Enums.h"

using namespace BLE;

class ControllerAgent : public QObject
{
    Q_OBJECT

public:
    static ControllerAgent* instance(QObject *parent = nullptr);

    void sendCommand(Enums::Services serviceType, const QString& command);

public slots:
    void onLeftStickChanged(float lx, float ly);
    void onRightStickChanged(float rx, float ry);

private slots:
    void sendBufferedData();
    void onIdleTimeout();

private:
    explicit ControllerAgent(QObject *parent = nullptr);

    void resetIdleTimer();  // Add this
    void sendStopCommands(); // Add this helper

    BleGattClient *bleClient = nullptr;

    QTimer *idleTimer = nullptr;

    // Services
    SteeringService steeringService;
    MotorService motorService;

    static constexpr float CHANGE_THRESHOLD = 0.01f; // 1% minimum change
    // Last sent values to prevent duplicate sends
    float lastSentLeftX = 0.0f;  // Use impossible value to force first send
    float lastSentLeftY = 0.0f;
    float lastSentRightX = 0.0f;
    float lastSentRightY = 0.0f;

    // Buffered values
    float bufferedLeftX = 0.0f;
    float bufferedLeftY = 0.0f;
    float bufferedRightX = 0.0f;
    float bufferedRightY = 0.0f;

    // Last sent values (for change detection)
    float lastLeftX = 0.0f;
    float lastLeftY = 0.0f;
    float lastRightX = 0.0f;
    float lastRightY = 0.0f;

    // Update flags
    bool hasLeftStickUpdate = false;
    bool hasRightStickUpdate = false;

        // Idle timeout settings
    static constexpr int IDLE_TIMEOUT_MS = 10000; // 10 seconds

};

#endif // CONTROLLERAGENT_H
