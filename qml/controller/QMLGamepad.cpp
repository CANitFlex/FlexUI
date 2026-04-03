#include "QMLGamepad.h"

// Singleton implementation
QMLGamepad* QMLGamepad::instance(QObject *parent) {
    static QMLGamepad* inst = nullptr;
    if (!inst) {
        inst = new QMLGamepad(parent);
    }
    return inst;
}

QMLGamepad::QMLGamepad(QObject *parent)
    : QObject{parent}
{
    // Initialize ControllerAgent
    agent = ControllerAgent::instance(this);
    
    // Connect signals to ControllerAgent
    connect(this, &QMLGamepad::leftStickChanged,
            agent, &ControllerAgent::onLeftStickChanged);
    connect(this, &QMLGamepad::rightStickChanged,
            agent, &ControllerAgent::onRightStickChanged);
}

void QMLGamepad::setLeftStickX(float x) {
    if (qFuzzyCompare(m_leftStickX, x)) return;
    m_leftStickX = qBound(-1.0f, x, 1.0f);
    qDebug() << x;
    emit leftStickXChanged();
    emit leftStickChanged(m_leftStickX, m_leftStickY);
    emit joystickChanged(m_leftStickX, m_leftStickY, m_rightStickX, m_rightStickY);
}

void QMLGamepad::setLeftStickY(float y) {
    if (qFuzzyCompare(m_leftStickY, y)) return;
    
    m_leftStickY = qBound(-1.0f, y, 1.0f);
    emit leftStickYChanged();
    emit leftStickChanged(m_leftStickX, m_leftStickY);
    emit joystickChanged(m_leftStickX, m_leftStickY, m_rightStickX, m_rightStickY);
}

void QMLGamepad::setRightStickX(float x) {
    if (qFuzzyCompare(m_rightStickX, x)) return;
    
    m_rightStickX = qBound(-1.0f, x, 1.0f);
    emit rightStickXChanged();
    emit rightStickChanged(m_rightStickX, m_rightStickY);
    emit joystickChanged(m_leftStickX, m_leftStickY, m_rightStickX, m_rightStickY);
}

void QMLGamepad::setRightStickY(float y) {
    if (qFuzzyCompare(m_rightStickY, y)) return;
    
    m_rightStickY = qBound(-1.0f, y, 1.0f);
    emit rightStickYChanged();
    emit rightStickChanged(m_rightStickX, m_rightStickY);
    emit joystickChanged(m_leftStickX, m_leftStickY, m_rightStickX, m_rightStickY);
}

void QMLGamepad::updateJoystick(float lx, float ly, float rx, float ry) {
    // Clamp values to -1.0 to 1.0 range
    lx = qBound(-1.0f, lx, 1.0f);
    ly = qBound(-1.0f, ly, 1.0f);
    rx = qBound(-1.0f, rx, 1.0f);
    ry = qBound(-1.0f, ry, 1.0f);
    
    bool changed = false;
    
    if (!qFuzzyCompare(m_leftStickX, lx)) {
        m_leftStickX = lx;
        emit leftStickXChanged();
        changed = true;
    }
    
    if (!qFuzzyCompare(m_leftStickY, ly)) {
        m_leftStickY = ly;
        emit leftStickYChanged();
        changed = true;
    }
    
    if (!qFuzzyCompare(m_rightStickX, rx)) {
        m_rightStickX = rx;
        emit rightStickXChanged();
        changed = true;
    }
    
    if (!qFuzzyCompare(m_rightStickY, ry)) {
        m_rightStickY = ry;
        emit rightStickYChanged();
        changed = true;
    }
    
    if (changed) {
        emit leftStickChanged(m_leftStickX, m_leftStickY);
        emit rightStickChanged(m_rightStickX, m_rightStickY);
        emit joystickChanged(m_leftStickX, m_leftStickY, m_rightStickX, m_rightStickY);
    }
}

void QMLGamepad::resetSticks() {
    updateJoystick(0.0f, 0.0f, 0.0f, 0.0f);
}
