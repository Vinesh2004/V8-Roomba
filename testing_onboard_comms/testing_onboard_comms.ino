#define MOTOR_ROWS 2

#include "Motor_Driver.hpp"
#include "Onboard_Comms.hpp"

int motorPins[MOTOR_ROWS][MOTOR_COLS][NUM_MOTOR_PINS];
Motor_Driver testDriver;
CommsIn comms;

void receivedData(int numBytes)
{
  comms.commsRecieved(numBytes);
}

void setup()
{
  Serial.begin(9600);
  motorPins[0][0][0] = 3;
  motorPins[0][0][1] = 5;
  
  motorPins[0][1][0] = 6;
  motorPins[0][1][1] = 9;

  testDriver = Motor_Driver(motorPins, true);
  comms = CommsIn(SLAVE_ADDR, testDriver);
  Wire.onReceive(receivedData);

}

void loop()
{
  delay(1);
}
