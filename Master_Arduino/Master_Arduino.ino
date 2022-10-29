#include <Wire.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

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
      byte leftSpeed = round(leftS);
      byte rightSpeed = round(rightS);

      sendCommand(1, leftSpeed, 1, rightSpeed);
    }

    void backward(float leftS, float rightS) {
      byte leftSpeed = round(leftS);
      byte rightSpeed = round(rightS);

      sendCommand(0, leftSpeed, 0, rightSpeed);
    }

    void spinRight(float leftS, float rightS) {
      byte leftSpeed = round(leftS);
      byte rightSpeed = round(rightS);

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

RF24 radio(9, 8);
const byte rx_address[6] = "00001";
const byte tx_address[6] = "10000";

void radioSetup() {
  radio.begin();

  radio.openReadingPipe(0, rx_address);
  //radio.openWritingPipe(tx_address);

  radio.startListening();
}

void setup() {
  Wire.begin(masterAddress);
  Serial.begin(9600);
  radioSetup();
}

Driver motorController = Driver(0x4);

float rightMotorSpeed = 255;
float leftMotorSpeed = 255;

bool radioFailed = false;
uint32_t setupTimer = millis();

bool listening = true;

char toSendPackage[32] = {0};

void loop() {
  if (millis() - setupTimer > 2000) {
    setupTimer = millis();
    if (radio.getDataRate() != RF24_1MBPS) {
      radioFailed = true;
    }
  }

  if (radioFailed) {
    Serial.println("Reseting radio");
    radioSetup();
    radioFailed = false;
  }

  if (listening)
  {
    radio.startListening();

    char receivedData = ' ';
    if (radio.available()) {
      Serial.println(receivedData);
      uint32_t receiveTimer = millis();
      while (radio.available()) {
        if (millis() - receiveTimer > 500) {
          radioFailed = true;
          break;
        }
        radio.read(&receivedData, sizeof(receivedData));
        Serial.println(receivedData);
        switch (toupper(receivedData)) {
          case 'W':
            motorController.forward(leftMotorSpeed, rightMotorSpeed);
            break;

          case 'S':
            motorController.backward(leftMotorSpeed, rightMotorSpeed);
            break;

          case 'D':
            motorController.spinRight(leftMotorSpeed, rightMotorSpeed);
            break;

          case 'A':
            motorController.spinLeft(leftMotorSpeed, rightMotorSpeed);
            break;

          case 'Q':
            leftMotorSpeed += 20;
            break;

          case 'Z':
            leftMotorSpeed -= 20;
            break;
  
          case 'E':
            rightMotorSpeed += 20;
            break;

          case 'C':
            rightMotorSpeed -= 20;
            break;
          
          default:
            break;
        }
      }
    }
  }
  else
  {
    radio.stopListening();

    while (!radio.write(&toSendPackage, sizeof(toSendPackage))) {
      Serial.println("Attempting to send package to the dock");
    }
    listening = true;
  }

}
