#ifndef STEERINGSERVICE_H
#define STEERINGSERVICE_H

#include "AbstractService.h"

class SteeringService : public AbstractService
{
public:
    SteeringService(){}
    ~SteeringService() = default;

    constexpr inline float exponentialCurve(float joystickValue) {
        float normalized = std::max(-1.0f, std::min(1.0f, joystickValue));

        // Apply exponential curve for more natural feel
        float sign = (normalized >= 0) ? 1.0f : -1.0f;
        float curved = sign * std::pow(std::abs(normalized), 1.5f); // Adjust exponent for different feel

        return curved; // Return float, not int
    }

    constexpr inline float linearInterpolation(float joystickValue) {
        // Clamp joystick value to -1.0 to 1.0 range
        float normalized = std::max(-1.0f, std::min(1.0f, joystickValue));
        
        // Linear interpolation: convert -1.0...1.0 to -100.0...100.0 percent
        float percent = normalized * 100.0f;
        
        return percent;
    }

    constexpr inline int linearInterpolationToAngle(float joystickValue) {
        float percent = linearInterpolation(joystickValue);
        
        // Map -100% to 100% to servo angles (50° to 130°)
        // -100% = 50°, 0% = 90°, +100% = 130°
        float angle = 90.0f + (percent * 0.4f); // 0.4f = 40°/100%
        
        return static_cast<int>(std::round(angle));
    }

    constexpr inline int joystickToServoAngle(float joystickValue) {
        float curved = exponentialCurve(joystickValue);

        // Map to servo angles (50° to 130°) - SWAPPED: left stick = right turn, right stick = left turn
        float angle = 90.0f + (curved * 40.0f);  // Changed from +40 to -40 to swap directions
        return static_cast<int>(std::round(angle));
    }

    void execute(const QVariant& var = QVariant()) override {
        if (var.canConvert<float>()) {
            float joystickValue = var.value<float>();

            // ✅ Transformation
            int servoAngle = joystickToServoAngle(joystickValue);

            // ✅ Direkt zum RateLimiter
            RateLimiter::instance()->queue(
                Enums::Services::S_SERVO,
                QVariant::fromValue(servoAngle)
                );

            qDebug() << "Steering Service - Joystick:" << joystickValue
                     << "→ Servo Angle:" << servoAngle;
        }
        else if (var.canConvert<QString>()) {
            QString command = var.value<QString>();
            int angle = 90; // Default

            if (command == "CENTER") {
                angle = 90;
            } else if (command == "LEFT") {
                angle = 50;
            } else if (command == "RIGHT") {
                angle = 130;
            }

            RateLimiter::instance()->queue(
                Enums::Services::S_SERVO,
                QVariant::fromValue(angle)
                );

            qDebug() << "Steering Service - Command:" << command
                     << "→ Servo Angle:" << angle;
        }
    }
};

#endif // STEERINGSERVICE_H
