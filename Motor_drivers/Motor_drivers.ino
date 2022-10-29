#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

class Motor {
  private:
    uint8_t forwardPin;
    uint8_t backwardPin;

  public:
    Motor (uint8_t forward, uint8_t backward){
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
      float pwmOut = speedPercentage * 255;

      digitalWrite(forwardPin, pwmOut);
      digitalWrite(backwardPin, LOW);

      Serial.println("f" + String(speedPercentage) + "#");
    }

    void backward(float speedPercentage) {
      float pwmOut = speedPercentage * 255;

      digitalWrite(backwardPin, pwmOut);
      digitalWrite(forwardPin, LOW);

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


void setup() {
  Serial.begin(9600);
}


Motor rightMotor = Motor(3,5);
Motor leftMotor = Motor(6,9);

Driver botDriver = Driver(&rightMotor, &leftMotor);

void loop() {
  botDriver.forward(1,1);
  delay(1000);
  botDriver.backward(1,1);
  delay(1000);
  botDriver.forward(0.25, 0.25);
  delay(1000);
  botDriver.backward(0.25, 0.25);
  delay(1000);
}
