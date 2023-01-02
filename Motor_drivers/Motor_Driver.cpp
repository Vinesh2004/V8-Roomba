#include "Motor_Driver.hpp"

Motor::Motor(int forward_in, int backward_in)
{
    (*this).forward = forward_in;
    (*this).backward = backward_in;

    pinMode((*this).forward, OUTPUT);
    pinMode((*this).backward, OUTPUT);
}

Motor::Motor()
{
  (*this).forward = 3;
  (*this).backward = 5;

  pinMode((*this).forward, OUTPUT);
  pinMode((*this).backward, OUTPUT);
}

void Motor::drive()
{
    bool fwdDrive = bool((motorPow / abs(motorPow)) > 0);
    analogWrite((*this).forward, abs(motorPow) * fwdDrive);
    analogWrite((*this).backward, abs(motorPow) * !fwdDrive);
}


void Motor::drive(int motorPow_in)
{
    (*this).motorPow = min(abs(motorPow_in), MAX_POW);
  	(*this).motorPow *= (motorPow_in / abs(motorPow_in));
  	
    (*this).drive();
}

void Motor::halt()
{
    analogWrite((*this).forward, LOW);
    analogWrite((*this).backward, LOW);
    (*this).motorPow = 0;
}

void Motor::halt(int timeMsec)
{
    int tempMotorPow = (*this).motorPow;
    (*this).halt();
    delay(timeMsec);
    (*this).drive(tempMotorPow);
}

Motor_Driver::Motor_Driver(
                int motorPins_in[MOTOR_ROWS][MOTOR_COLS][NUM_MOTOR_PINS],
                bool omniDrive_in = false)
{
    if (MOTOR_ROWS > 1)
    {
        (*this).omniDriveVal = omniDrive_in;
    }
    else
    {
        (*this).omniDriveVal = false;
    }

    for(int motor_row = 0; motor_row < MOTOR_ROWS; motor_row ++)
    {
        for (int motor_col = 0; motor_col < MOTOR_COLS; motor_col ++)
        {
            motor[motor_row][motor_col] = Motor(
                                    motorPins_in[motor_row][motor_col][0],
                                    motorPins_in[motor_row][motor_col][1]);
        }
    }
}

Motor_Driver::Motor_Driver()
{
    (*this).omniDriveVal = false;

    for(int motor_row = 0; motor_row < MOTOR_ROWS; motor_row ++)
    {
        for (int motor_col = 0; motor_col < MOTOR_COLS; motor_col ++)
        {
            motor[motor_row][motor_col] = Motor();
        }
    }
}

void Motor_Driver::omniDrive(float angle, int motorPow)
{
    if (!(*this).omniDriveVal)
        return;
    
    int power1 = (sin(angle)-cos(angle)) * motorPow;
    int power2 = (sin(angle)+cos(angle)) * motorPow;

    for (int row = 0; row < MOTOR_ROWS; row++)
    {
        for (int col = 0; col < MOTOR_COLS; col++)
        {
            if (row == col)
            {
                motor[row][col].drive(power1);
            }
            else
            {
                motor[row][col].drive(power2);
            }
        }
    }
}

void Motor_Driver::drive(int motorPow)
{
    for (int row = 0; row < MOTOR_ROWS; row ++)
    {
        for (int col = 0; col < MOTOR_COLS; col ++)
        {
            motor[row][col].drive(motorPow);
        }
    }
}

void Motor_Driver::spin(int motorPow)
{
    for (int row = 0; row < MOTOR_ROWS; row ++)
    {
      for(int col = 0; col < MOTOR_COLS; col ++)
      {
        motor[row][0].drive(motorPow);
        motor[row][1].drive(-motorPow);
      }
    }
}
void Motor_Driver::spin(int motorPow, int angle)
{
    // not implemented; needs a gyro sensor or some method of determining angle.
    return;
}


void Motor_Driver::turn(int motorPow, float turningRadius)
{
    // turning radius is expressed in length relative to the length of
    // the roomba where the roomba's length is represent with a value of 1
    const double vehicleLength = 1.00;

    int rightSide = motorPow;

    int leftSide = rightSide / (1 + vehicleLength/abs(turningRadius));
    
  	if (turningRadius < 0)
    {
      int temp = rightSide;
      rightSide = leftSide;
      leftSide = temp;
    }
  
  	for (int row = 0; row < MOTOR_ROWS; row++)
    {
        motor[row][0].drive(leftSide);
        motor[row][1].drive(rightSide);
    }
}

void Motor_Driver::turn(int motorPow, float turiningRadius, int angle)
{
    // not implemented; needs a gyro sensor or some method of determining angle.
    return;
}

void Motor_Driver::stop()
{
    for (int row = 0; row < MOTOR_ROWS; row++)
    {
        for (int col = 0; col < MOTOR_COLS; col++)
        {
            motor[row][col].halt();
        }
    }
}

void Motor_Driver::stop(int timeMsec)
{
    for (int row = 0; row < MOTOR_ROWS; row++)
    {
        for (int col = 0; col < MOTOR_COLS; col++)
        {
            motor[row][col].halt(timeMsec);
        }
    }
}