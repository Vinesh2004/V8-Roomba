#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9,8);
const byte rx_address[6] = "10000";
const byte tx_address[6] = "00001";

void radioSetup() {
  radio.begin();

  radio.openReadingPipe(0, rx_address);
  radio.openWritingPipe(tx_address);

  radio.startListening();
}

void setup(){
  Serial.begin(9600);
  radioSetup();
  
}

bool radioFailed = false;
uint32_t setupTimer = millis();

bool listening = false;

char toSendPackage = ' ';

void loop(){
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
      uint32_t receiveTimer = millis();
      while (radio.available()) {
        if (millis() - receiveTimer > 500) {
          radioFailed = true;
          break;
        }
        radio.read(&receivedData, sizeof(receivedData));
      }
    }
  }
  else
  {
    radio.stopListening();
    
    if (!radio.write(&toSendPackage, sizeof(toSendPackage))) {
      Serial.println("Attempting to send package to the bot");
    }
  }

  if (Serial.available()){
    toSendPackage = Serial.read();
    Serial.end();
    Serial.begin(9600);
    Serial.println(toSendPackage);
  }
}
