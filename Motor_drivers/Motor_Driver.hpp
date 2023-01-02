#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#define MAX_POW 255

#ifndef MOTOR_ROWS
#define MOTOR_ROWS 1
#endif

#define MOTOR_COLS 2

#define NUM_MOTOR_PINS 2

#include "Arduino.h"

/**
 * @brief Motor structure definition.
 * 
 */
class Motor
{
    private:
        int forward;
        int backward;
        int motorPow;
    public:
        Motor(int foward_in, int backward_in);
        Motor();
        void drive();
        void drive(int motorPow_in);
        void halt();
        void halt(int timeMsec);
};

class Motor_Driver
{
    private:
        Motor motor[MOTOR_ROWS][MOTOR_COLS];
        bool omniDriveVal;
    public:
        Motor_Driver(int motorPins_in[MOTOR_ROWS][MOTOR_COLS][NUM_MOTOR_PINS],
                     bool omniDrive_in);
        Motor_Driver();
        
        void omniDrive(float angle, int motorPow);

        void drive(int motorPow);

        void spin(int motorPow);
        void spin(int motorPow, int angle);

        void turn(int motorPow, float turiningRadius, int angle);
        void turn(int motorPow, float turningRadius);

        void stop();
        void stop(int timeMsec); 
};

#endif