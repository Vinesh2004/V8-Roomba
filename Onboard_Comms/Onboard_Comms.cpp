#include "Onboard_Comms.hpp"

// Comms Out
/* Private */

void CommsOut::sendCommand()
{
    Wire.beginTransmission((*this).address);
    Wire.write((*this).state);

    Wire.endTransmission();
}

void CommsOut::sendCommand(uint8_t direction, uint8_t motorPow)
{
    Wire.beginTransmission((*this).address);
    Wire.write((*this).state);

    Wire.write(direction);
    Wire.write(motorPow);

    Wire.endTransmission();
}

void CommsOut::sendCommand(uint8_t direction, uint8_t motorPow, uint8_t angleDeg)
{
    Wire.beginTransmission((*this).address);
    Wire.write((*this).state);

    Wire.write(direction);
    Wire.write(motorPow);
    Wire.write(angleDeg);

    Wire.endTransmission();
}

/* Public */
CommsOut::CommsOut()
{
    address = MASTER_ADDR;
    startUp();
}

CommsOut::commsOut(uint8_t address_in)
{
    address = address_in;
    startUp();
}

void CommsOut::startUp()
{
    (*this).currentState = STATE_STARTUP;
    sendCommand();
}

void CommsOut::changeState(uint8_t state)
{
    (*this).currentState = state;
}

void CommsOut::omniDrive(int angleDeg, int motorPow)
{
    (*this).changeState(OMNI_DRIVE);
    uint8_t direction = FORWARD;

    if (motorPow < 0)
    {
        direction = BACKWARD;
    }

    sendCommand(direction, abs(motorPow), angleDeg);
}

void CommsOut::drive(int motorPow)
{
    (*this).changeState(DRIVE);
    uint8_t direction = FORWARD;

    if (motorPow < 0)
    {
        direction = BACKWARD;
    }

    sendCommand(direction, abs(motorPow));
}

void CommsOut::spin(int motorPow)
{
    (*this).changeState(SPIN_DRIVE);
    uint8_t direction = FORWARD;

    if (motorPow < 0)
    {
        direction = BACKWARD;
    }

    sendCommand(direction, abs(motorPow));
}

void CommsOut::spin(int motorPow, int angleDeg)
{
    (*this).changeState(SPIN_DRIVE_ANG);
    uint8_t direction = FORWARD;

    if (motorPow < 0)
    {
        direction = BACKWARD;
    }

    sendCommand(direction, abs(motorPow), angleDeg);
}

void CommsOut::turn(int motorPow, float turiningRadius)
{
    (*this).changeState(TURN_DRIVE);
    uint8_t direction = FORWARD;

    if (motorPow < 0)
    {
        direction = BACKWARD;
    }

    sendCommand(direction, abs(motorPow), turiningRadius*10);
}

void CommsOut::stop()
{
    (*this).changeState(STOP);
    sendCommand();
}

//Comms In
/* Private */

bool CommsIn::commsAvail()
{
    return (Wire.available() > 0);
}

/* Public */
CommsIn::CommsIn()
{
    (*this).address = SLAVE_ADDR;
    (*this).currentState = STANDBY;
}

CommsIn::CommsIn(uint8_t address_in, Motor_Driver driver_in)
{
    (*this).address = address_in;
    (*this).currentState = STANDBY;
    (*this).driver = driver_in;
}

CommsIn::void commsRecieved(int numBytes)
{
    while(commsAvail())
    {
        byte state = Wire.read();
        (*this).currentState = state;
        
        switch(state)
        {
            case STARTUP:
                // No sequence yet
                break;
            
            case CONTROL:
                // No sequence yet
                break;
            
            case DRIVE:
                uint8_t direction = Wire.read();
                int motorPow = Wire.read();

                if (direction != FORWARD)
                {
                   motorPow *= -1; 
                }

                (*this).driver.drive(motorPow);

                break;
            
            case OMNI_DRIVE:
                uint8_t direction = Wire.read();
                int motorPow = Wire.read();

                if (direction != FORWARD)
                {
                    motorPow *= -1;
                }

                int angleDeg = Wire.read();
                float angle = angleDeg * PI/180;

                (*this).driver.omniDrive(angle, motorPow);

                break;
            
            case SPIN_DRIVE || SPIN_DRIVE_ANG:
                uint8_t direction = Wire.read();
                int motorPow = Wire.read();

                if (direction != FORWARD)
                    motorPow *= -1;
                
                if (state == SPIN_DRIVE_ANG)
                {
                    int angleDeg = Wire.read();
                    (*this).driver.spin(motorPow, angleDeg);
                }
                else
                {
                    (*this).driver.spin(motorPow);
                }

                break;
            
            case TURN_DRIVE:
                uint8_t direction = Wire.read();
                int motorPow = Wire.read();

                if (direction != FORWARD)
                    motorPow *= -1;
                
                int turnRad = Wire.read();
                float turningRadius = turnRad / 10;

                (*this).driver.turn(motorPow, turningRadius);

                break;
            
            case STOP:
                (*this).driver.stop();

                break;
        }
    }
}