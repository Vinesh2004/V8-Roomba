#ifndef MOTORDRIVER_h
#define MOTORDRIVER_h

#include "Arduino.h"
#include "../Motor_Data_types.h"

class Motor_Driver
{
    public:
        Motor_Driver(motor_t *current_motor);
        ~Motor_Driver();
        void driveMotor(float speedPercent, bool forward);

    private:
        motor_t *_motor;
};

#endif