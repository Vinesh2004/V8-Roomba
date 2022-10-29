#include <Wire.h>

class Motor {
  private:
    uint8_t forwardPin;
    uint8_t backwardPin;

  public:
    Motor (uint8_t forward, uint8_t backward) {
      this->forwardPin = forward;
      this->backwardPin = backward;

      init();
    }
    void init() {
      pinMode(forwardPin, OUTPUT);
      pinMode(backwardPin, OUTPUT);

      halt();
    }

    void halt() {
      digitalWrite(forwardPin, LOW);
      digitalWrite(backwardPin, LOW);

      Serial.println("s#");
    }

    void forward(float speedPercentage) {
      int pwmOut = speedPercentage;

      digitalWrite(forwardPin, pwmOut);
      digitalWrite(backwardPin, 0);

      Serial.println("f" + String(speedPercentage) + "#");
    }

    void backward(float speedPercentage) {
      int pwmOut = speedPercentage;

      digitalWrite(backwardPin, pwmOut);
      digitalWrite(forwardPin, 0);

      Serial.println("b" + String(speedPercentage) + "#");
    }

    void identify() {
      Serial.println("Forward: Pin" + String(forwardPin));
      Serial.println("Backward: Pin" + String(backwardPin));
    }
};

class Driver {
  private:
    Motor *rightMotor;
    Motor *leftMotor;

  public:
    Driver(Motor *rightMotor, Motor *leftMotor) {
      this->rightMotor = rightMotor;
      this->leftMotor = leftMotor;

      init();
    }

    void init() {
      Serial.println("------------------------");
      Serial.println("Motor driver initialized");

      Serial.println("Right motor pins:");
      rightMotor->identify();
      Serial.println("------------------");

      Serial.println("Left motor pins:");
      leftMotor->identify();
      Serial.println("-------------------------");
    }

    void forward(float rightS, float leftS) {
      Serial.print("#r");
      rightMotor->forward(rightS);
      Serial.print("#l");
      leftMotor->forward(leftS);
    }

    void backward(float rightS, float leftS) {
      Serial.print("#r");
      rightMotor->backward(rightS);
      Serial.print("#l");
      leftMotor->backward(leftS);
    }

    void spinRight(float rightS, float leftS) {
      Serial.print("#r");
      rightMotor->backward(rightS);
      Serial.print("#l");
      leftMotor->forward(leftS);
    }

    void spinLeft(float rightS, float leftS) {
      Serial.print("#r");
      rightMotor->forward(rightS);
      Serial.print("#l");
      leftMotor->backward(leftS);
    }

    void halt() {
      Serial.print("#r");
      rightMotor->halt();
      Serial.print("#l");
      leftMotor->halt();
    }
};

byte deviceAddress = 0x4;
byte startUp = 0x11;

void setup() {
  Wire.begin(deviceAddress);
  Wire.onReceive(recievedData);
  Serial.begin(9600);
}


Motor rightMotor = Motor(3, 5);
Motor leftMotor = Motor(6, 9);

Driver motorDriver = Driver(&rightMotor, &leftMotor);

void loop(){
  delay(1);
}

void recievedData(int numBytes) {
  while (Wire.available() > 0) {
    byte first = Wire.read();
    if (first == startUp) {

      // left motor
      char lDirection = Wire.read();
      uint8_t lSpeed = Wire.read();

      Serial.println(lDirection + String(lSpeed));

      // right motor
      char rDirection = Wire.read();
      uint8_t rSpeed = Wire.read();

      Serial.println(rDirection + String(rSpeed));

      if (rSpeed == 0 && lSpeed == 0) {
        motorDriver.halt();
      }
      else if (rDirection == 'f' && lDirection == 'f') {
        motorDriver.forward(rSpeed, lSpeed);
      }
      else if (rDirection == 'b' && lDirection == 'b') {
        motorDriver.backward(rSpeed, lSpeed);
      }
      else if (rDirection == 'b' && lDirection == 'f') {
        motorDriver.spinRight(rSpeed, lSpeed);
      }
      else if (rDirection == 'f' && lDirection == 'b') {
        motorDriver.spinLeft(rSpeed, lSpeed);
      }

    }
  }
}
