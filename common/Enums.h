#ifndef ENUMS_H
#define ENUMS_H

class Enums {
public:
    enum Services {
        S_SERVO = 0x111,
        S_MOTOR = 0x222,
    };

    enum Characteristics {
        C_SERVO = 0x112,
        C_MOTOR_LEFT = 0x223,
        C_MOTOR_RIGHT = 0x224
    };
};

#endif // ENUMS_H