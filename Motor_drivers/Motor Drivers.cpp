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

class Ultrasonic {
  private:
    uint8_t trigPin;
    uint8_t echoPin_r;
    uint8_t echoPin_l;
    const float soundSpeed = 0.034;
  public:
    float distance_r, distance_l;
  
    Ultrasonic(uint8_t trigPin, uint8_t echoPin_r, uint8_t echoPin_l) {
      this->trigPin = trigPin;
      this->echoPin_r = echoPin_r;
      this->echoPin_l = echoPin_l;

      pinMode(trigPin, OUTPUT);
      pinMode(echoPin_r, INPUT);
      pinMode(echoPin_l, INPUT);

      init();
    }

    void init() {
      Serial.println("-----------------------------");
      Serial.println("Ultrasonic Sensor Initialized");
      Serial.println("Trig pin: pin" + String(trigPin));
      Serial.println("Right echo pin: pin" + String(echoPin_r));
      Serial.println("Left echo pin: pin" + String(echoPin_l));
      Serial.println("-----------------------------");
    }

    void measure() {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      
      this->distance_r = pulseIn(echoPin_r, HIGH) * soundSpeed / 2;
      this->distance_l = pulseIn(echoPin_l, HIGH) * soundSpeed / 2;

      displayDist();
    }

    void displayDist() {
      Serial.println("#dr" + String(distance_r) + "#");
      Serial.println("#dl" + String(distance_l) + "#");
    }
};

class Transiever{
  private:
    uint8_t chipEnable;
    uint8_t chipSelect;
    byte *addresses[2];
    RF24 radio;
    
    unsigned long prevTimeRecieved = 0;
  public:    
    boolean signalLost = false;
    
    Transiever(uint8_t chipEnable, uint8_t chipSelect, byte address_tx[6], byte address_rx[6]){
      this->chipEnable = chipEnable;
      this->chipSelect = chipSelect;
      this->addresses[0] = address_tx;
      this->addresses[1] = address_rx;
      
      init();
    }

    void init(){
      RF24 radio(chipEnable, chipSelect);
      this->radio = radio;

      radio.begin();
      radio.setDataRate(RF24_2MBPS);

      radio.openWritingPipe(addresses[0]);
      radio.openReadingPipe(0, addresses[1]);

      radio.startListening();
    }

    void transmit(char *transmit_buf){
      delay(1);
      radio.stopListening();
      
      radio.write(transmit_buf, sizeof(transmit_buf));

      Serial.print("Transmitted: ");
      displayData(transmit_buf);
      
      delay(1);
      radio.startListening();
    }
    
    char *checkRecieved(){
      unsigned long currentTime = millis();

      // 1 second timeout
      if (currentTime - prevTimeRecieved > 1000){
        signalLost = true;
      }
      
      static char *recieved_buf;
      
      while (radio.available()){
        prevTimeRecieved = millis();
        
        signalLost = false;
        
        recieved_buf = recieve();        
      }
      return recieved_buf;
    }
    
    char *recieve(){
      static char recieve_buf[32] = {0};
      radio.read(recieve_buf, sizeof(recieve_buf));

      Serial.print("Recieved: ");
      displayData(recieve_buf);

      return recieve_buf;      
    }

    void displayData(char *text_buf){
      Serial.println(text_buf);
    }
};

void setup() {
  
}

void loop() {

}
