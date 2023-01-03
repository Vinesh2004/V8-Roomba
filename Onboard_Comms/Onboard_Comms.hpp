#ifndef ONBOARD_COMMS_H
#define ONBOARD_COMMS_H

#define STANDBY 0

#define STARTUP 1
#define CONTROL 2

#define DRIVE 3
#define OMNI_DRIVE 4
#define SPIN_DRIVE 5
#define SPIN_DRIVE_ANG 6
#define TURN_DRIVE 7
#define STOP 8

#define FORWARD 9
#define BACKWARD 10

#define MASTER_ADDR = 0x01
#define SLAVE_ADDR = 0x02

#include "Arduino.h"
#include "../Motor_Drivers/Motor_Driver.hpp"

class CommsOut
{
    private:
        uint8_t address;
        int currentState;
        
        void sendCommand();
        void sendCommand(uint8_t direction, uint8_t motorPow);
        void sendCommand(uint8_t direction, uint8_t motorPow, uint8_t angleDeg);

    public:
        CommsOut();
        commsOut(uint8_t address_in);

        void startUp();

        void changeState(int state);

        void omniDrive(int angleDeg, int motorPow);

        void drive(int motorPow);

        void spin(int motorPow);
        void spin(int motorPow, int angleDeg);

        void turn(int motorPow, float turiningRadius, int angleDeg);
        void turn(int motorPow, float turningRadius);

        void stop(); 
};

class CommsIn
{
    private:
        uint8_t address;
        Motor_Driver driver;
        
        int currentState;

        bool commsAvail();

    public:
        CommsIn();
        CommsIn(uint8_t address_in, Motor_Driver driver_in);
      
        void commsRecieved(int numBytes);
};

#endif