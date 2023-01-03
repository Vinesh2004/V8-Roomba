#include "Onboard_Comms.hpp"

CommsOut comms;

void setup()
{
  Serial.begin(9600);
  comms = CommsOut(SLAVE_ADDR);
}


void loop()
{
  comms.drive(-255);
}
