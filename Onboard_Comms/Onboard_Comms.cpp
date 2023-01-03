#include "Onboard_Comms.hpp"

// Comms Out
/* Private */

void CommsOut::sendCommand()
{
    Wire.beginTransmission((*this).address);
    Wire.write((*this).currentState);

    Wire.endTransmission();
}

void CommsOut::sendCommand(uint8_t direction, uint8_t motorPow)
{
    Wire.beginTransmission((*this).address);
    Wire.write((*this).currentState);

    Wire.write(direction);
    Wire.write(motorPow);

    Wire.endTransmission();
}

void CommsOut::sendCommand(uint8_t direction, uint8_t motorPow, uint8_t angleDeg)
{
    Wire.beginTransmission((*this).address);
    Wire.write((*this).currentState);

    Wire.write(direction);
    Wire.write(motorPow);
    Wire.write(angleDeg);

    Wire.endTransmission();
}

/* Public */
CommsOut::CommsOut()
{
    (*this).address = SLAVE_ADDR;
    //startUp();
}

CommsOut::CommsOut(uint8_t address_in)
{
    (*this).address = address_in;
    startUp();
}

void CommsOut::startUp()
{
    (*this).currentState = STARTUP;
    Wire.begin((*this).address);
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

    Wire.begin((*this).address);
}

void CommsIn::commsRecieved(int numBytes)
{
    while((*this).commsAvail())
    {
        byte state = Wire.read();
        (*this).currentState = state;
        
        uint8_t direction;
        int motorPow;

        int angleDeg;
        float angle;
        int turnRad;
        float turningRadius;
        
        switch(state)
        {
            case STARTUP:
                // No sequence yet
                break;
            
            case CONTROL:
                // No sequence yet
                break;
            
            case DRIVE:
                direction = Wire.read();
                motorPow = Wire.read();

                if (direction != FORWARD)
                {
                   motorPow *= -1; 
                }

                (*this).driver.drive(motorPow);

                break;
            
            case OMNI_DRIVE:
                direction = Wire.read();
                motorPow = Wire.read();

                if (direction != FORWARD)
                {
                    motorPow *= -1;
                }

                angleDeg = Wire.read();
                angle = angleDeg * PI/180;

                (*this).driver.omniDrive(angle, motorPow);

                break;
            
            case SPIN_DRIVE:
                direction = Wire.read();
                motorPow = Wire.read();

                if (direction != FORWARD)
                    motorPow *= -1;
              
                (*this).driver.spin(motorPow);
                
                break;

            case SPIN_DRIVE_ANG:
                direction = Wire.read();
                motorPow = Wire.read();

                if (direction != FORWARD)
                    motorPow *= -1;
                
                angleDeg = Wire.read();
                (*this).driver.spin(motorPow, angleDeg);
                
                break;
                
            case TURN_DRIVE:
                direction = Wire.read();
                motorPow = Wire.read();

                if (direction != FORWARD)
                    motorPow *= -1;
                
                turnRad = Wire.read();
                turningRadius = turnRad / 10;

                (*this).driver.turn(motorPow, turningRadius);

                break;
            
            case STOP:
                (*this).driver.stop();

                break;
        }
    }
}