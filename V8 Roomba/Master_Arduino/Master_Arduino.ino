#include <Wire.h>

class Driver {
  private:
    uint8_t address;
    byte startUp = 0x11; //startup code of 00010001;

    void sendCommand(bool leftDirection, byte leftSpeed, bool rightDirection, byte rightSpeed) {
      Wire.beginTransmission(address);

      Wire.write(startUp); // 1 byte

      if (!leftDirection) {
        Wire.write('b'); // 1 byte
      }
      else {
        Wire.write('f'); // 1 byte
      }

      Wire.write(leftSpeed); // 1 byte

      if (!rightDirection) {
        Wire.write('b'); // 1 byte
      }
      else {
        Wire.write('f'); // 1 byte
      }
      Wire.write(rightSpeed); // 1 byte

      Wire.endTransmission(); // 11 bytes total
    }

  public:
    Driver(uint8_t address) {
      this->address = address;

      Serial.println("Motor driver set at address:" + String(address));
    }

    void forward(float leftS, float rightS) {
      byte leftSpeed = round(leftS * 255);
      byte rightSpeed = round(rightS * 255);

      sendCommand(1, leftSpeed, 1, rightSpeed);
    }

    void backward(float leftS, float rightS) {
      byte leftSpeed = round(leftS * 255);
      byte rightSpeed = round(rightS * 255);

      sendCommand(0, leftSpeed, 0, rightSpeed);
    }

    void spinRight(float leftS, float rightS) {
      byte leftSpeed = round(leftS * 255);
      byte rightSpeed = round(rightS * 255);

      sendCommand(1, leftSpeed, 0, rightSpeed);
    }

    void spinLeft(float leftS, float rightS) {
      byte leftSpeed = round(leftS * 255);
      byte rightSpeed = round(rightS * 255);

      sendCommand(0, leftSpeed, 1, rightSpeed);
    }

    void halt() {
      sendCommand(0, 0, 0, 0);
    }
};

uint8_t masterAddress = 0x00;

void setup() {
  Wire.begin(masterAddress);
}

Driver motorController = Driver(0x4);

void loop() {
  // controlls
  delay(100);
}
