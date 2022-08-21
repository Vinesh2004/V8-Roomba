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

class Ultrasonic {
  private:
    uint8_t trigPin_r;
    uint8_t trigPin_l
    uint8_t echoPin_r;
    uint8_t echoPin_l;
    const float soundSpeed = 0.034;
  public:
    float distance_r, distance_l;
  
    Ultrasonic(uint8_t trigPin_r, uint8_t echoPin_r, uint8_t echoPin_r, uint8_t echoPin_l) {
      this->trigPin_r = trigPin_r;
      this->trigPin_l = trigPin_l;
      this->echoPin_r = echoPin_r;
      this->echoPin_l = echoPin_l;

      pinMode(trigPin_r, OUTPUT);
      pinMode(trigPin_l, OUTPUT);
      pinMode(echoPin_r, INPUT);
      pinMode(echoPin_l, INPUT);
    }

    void measure() {
      digitalWrite(trigPin_l, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin_l, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin_l, LOW);
      
      this->distance_r = pulseIn(echoPin_l, HIGH) * soundSpeed / 2;
      
      digitalWrite(trigPin_r, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin_r, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin_r, LOW);
      this->distance_l = pulseIn(echoPin_r, HIGH) * soundSpeed / 2;

      displayDist();
    }

    void displayDist() {
      Serial.println("#dr" + String(distance_r) + "#");
      Serial.println("#dl" + String(distance_l) + "#");
    }
};

uint8_t masterAddress = 0x00;

RF24 radio(9, 8);
const byte rx_address[6] = "00001";
const byte tx_address[6] = "10000";

void radioSetup() {
  radio.begin();

  radio.openReadingPipe(0, rx_address);
  radio.openWritingPipe(tx_address);

  radio.startListening();
}

void setup() {
  Wire.begin(masterAddress);
  Serial.begin(9600);
  radioSetup();
}

Driver motorController = Driver(0x4);

float rightMotorSpeed = 0.5;
float leftMotorSpeed = 0.5;

bool radioFailed = false;
uint32_t setupTimer = millis();

bool listening = true;

char toSendPackage[32] = {0};

bool docking = False;

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

  if (listening && docking)
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
          
          case 'G':
            motorController.halt(leftMotorSpeed, rightMotorSpeed);
            break;

          case 'Q':
            leftMotorSpeed += 0.1;
            break;

          case 'Z':
            leftMotorSpeed -= 0.1;
            break;
  
          case 'E':
            rightMotorSpeed += 0.1;
            break;

          case 'C':
            rightMotorSpeed -= 0.1;
            break;
          
          default:
            break;
        }
      }
    }
  }
  else if (docking)
  {
    radio.stopListening();

    while (!radio.write(&toSendPackage, sizeof(toSendPackage))) {
      Serial.println("Attempting to send package to the dock");
    }
    listening = true;
  }
  else{
    // code for autonomous control
  }

}
