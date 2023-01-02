#define MOTOR_ROWS 1
#include "Motor_Driver.hpp"

int motorPins[MOTOR_ROWS][MOTOR_COLS][NUM_MOTOR_PINS];
Motor_Driver testDriver;

void setup()
{
  Serial.begin(9600);
  motorPins[0][0][0] = 3;
  motorPins[0][0][1] = 5;
  
  motorPins[0][1][0] = 6;
  motorPins[0][1][1] = 9;

  testDriver = Motor_Driver(motorPins, true);
}

void loop()
{
  testDriver.drive(-255);
  //delay(1000);
  //testDriver.stop();
  //delay(1000);
}
