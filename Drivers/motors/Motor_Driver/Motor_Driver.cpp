#include "Motor_Driver.h"

Motor_Driver::Motor_Driver(motor_t *current_motor)
{
    _motor = current_motor;
    
    pinMode(_motor->forwardPin, OUTPUT);
    pinMode(_motor->backwardPin, OUTPUT);

}

void Motor_Driver::driveMotor(float speedPercent, bool forward = 1)
{
    float motorSpeed = speedPercent/100 * 255;

    digitalWrite(_motor->forwardPin, 
                forward?motorSpeed:0);
    digitalWrite(_motor->backwardPin, 
                !forward?motorSpeed:0);

    return;
}

Motor_Driver::~Motor_Driver()
{
    digitalWrite(_motor->forwardPin, 0);
    digitalWrite(_motor->backwardPin, 0);
}