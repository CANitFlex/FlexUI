#ifndef MOTORSERVICE_H
#define MOTORSERVICE_H


#include "AbstractService.h"


class MotorService : public AbstractService
{
public:
    MotorService(){}
    ~MotorService() = default;

    void setDownValue(int f)
    {
        /*
        bleClient->sendCommand(Enums::Services::S_MOTOR, Enums::Characteristics::C_MOTOR_LEFT, 255);
        bleClient->sendCommand(Enums::Services::S_MOTOR, Enums::Characteristics::C_MOTOR_RIGHT, f);
        */
    }

    void setUpValue(int f)
    {
        /*
        bleClient->sendCommand(Enums::Services::S_MOTOR, Enums::Characteristics::C_MOTOR_RIGHT, 255);
        bleClient->sendCommand(Enums::Services::S_MOTOR, Enums::Characteristics::C_MOTOR_LEFT, f);
        */
    }

    void setStopValue()
    {
        /*
        bleClient->sendCommand(Enums::Services::S_MOTOR, Enums::Characteristics::C_MOTOR_LEFT, 255);
        bleClient->sendCommand(Enums::Services::S_MOTOR, Enums::Characteristics::C_MOTOR_RIGHT, 255);
        */
    }

    void execute(const QVariant& var = QVariant()) override {
        if(var.canConvert<float>()) {
            float f = var.value<float>();
            
            // Only process non-zero values
            if (std::abs(f) > 0.001f) {  // Small threshold to avoid noise
                // Choose between linear or exponential curve
                int currentStrength = 255 - (std::abs(f) * 255);
                qDebug() << "Motor strength:" << currentStrength << "for input:" << f;
                
                // CORRECTED: Positive stick = forward, negative stick = backward
                if(f > 0) {
                    setUpValue(currentStrength);    // Forward
                } else {
                    setDownValue(currentStrength);  // Backward
                }
            } else {
                // Zero input = stop
                setStopValue();
            }
        }
        else if(var.canConvert<QString>()) {
            QString command = var.value<QString>();
            if(command == "STOP") {
                qDebug() << "Motor: STOP command received";
                setStopValue();
            }
            else if(command == "FORWARD") {
                qDebug() << "Motor: FORWARD command received";
                setUpValue(0);  // Full speed forward (255-255=0)
            }
            else if(command == "BACKWARD") {
                qDebug() << "Motor: BACKWARD command received"; 
                setDownValue(0);  // Full speed backward
            }
            else {
                qDebug() << "Motor: Unknown command:" << command;
            }
        }
    }
};

#endif // MOTORSERVICE_H
