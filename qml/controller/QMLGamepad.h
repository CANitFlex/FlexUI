#ifndef QMLGAMEPAD_H
#define QMLGAMEPAD_H

#include <QObject>
#include <QQmlEngine>
#include "../../common/ControllerAgent.h"

class QMLGamepad : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    
    Q_PROPERTY(float leftStickX READ leftStickX WRITE setLeftStickX NOTIFY leftStickXChanged)
    Q_PROPERTY(float leftStickY READ leftStickY WRITE setLeftStickY NOTIFY leftStickYChanged)
    Q_PROPERTY(float rightStickX READ rightStickX WRITE setRightStickX NOTIFY rightStickXChanged)
    Q_PROPERTY(float rightStickY READ rightStickY WRITE setRightStickY NOTIFY rightStickYChanged)
    
public:
    static QMLGamepad* instance(QObject *parent = nullptr);
    
    // Delete copy constructor and assignment operator
    QMLGamepad(const QMLGamepad&) = delete;
    QMLGamepad& operator=(const QMLGamepad&) = delete;
    
    // Property getters
    float leftStickX() const { return m_leftStickX; }
    float leftStickY() const { return m_leftStickY; }
    float rightStickX() const { return m_rightStickX; }
    float rightStickY() const { return m_rightStickY; }

    Q_INVOKABLE void sendCommand(Enums::Services type, const QString& command)
    {
        agent->sendCommand(type, command);
    }
    
    // Property setters
    void setLeftStickX(float x);
    void setLeftStickY(float y);
    void setRightStickX(float x);
    void setRightStickY(float y);
    
    Q_INVOKABLE void updateJoystick(float lx, float ly, float rx, float ry);
    Q_INVOKABLE void resetSticks();

signals:
    void joystickChanged(float lx, float ly, float rx, float ry);
    void leftStickChanged(float lx, float ly);
    void rightStickChanged(float rx, float ry);
    
    // Individual property signals
    void leftStickXChanged();
    void leftStickYChanged();
    void rightStickXChanged();
    void rightStickYChanged();

private:
    explicit QMLGamepad(QObject *parent = nullptr);
    
    float m_leftStickX = 0.0f;
    float m_leftStickY = 0.0f;
    float m_rightStickX = 0.0f;
    float m_rightStickY = 0.0f;
    
    ControllerAgent *agent = nullptr;
};

#endif // QMLGAMEPAD_H
