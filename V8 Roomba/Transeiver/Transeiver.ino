#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>


class Transiever{
  protected:
    uint8_t chipEnable;
    uint8_t chipSelect; // chip select not (ommitting the not jus for naming)
    uint64_t address;
    RF24 radio;
  public:
    Transiever(uint8_t pin_CE, uint8_t pin_CSN, uint64_t address){
      this->chipEnable = pin_CE;
      this->chipSelect = pin_CSN;

      this->address = address;

      init();
    }
    virtual void init() = 0;
};

class Transmitter : public Transiever{
  public:
    Transmitter(uint8_t pin_CE, uint8_t pin_CSN, uint64_t address) : Transiever(pin_CE, pin_CSN, address) {}

    void init(){
      RF24 radio(chipEnable, chipSelect);
      radio.begin();
      //set data rate
      radio.setDataRate(RF24_2MBPS);
      //setting address
      radio.openWritingPipe(address);
      //starts listenining();
      radio.stopListening();

      this->radio = radio;
    }

    char *transmit(char transmit_buf[]){
      radio.write(transmit_buf, sizeof(transmit_buf));
      
      displayTrasnmitted(transmit_buf);

      return transmit_buf;
    }

    void displayTrasnmitted(char text_buf[]){
      Serial.print("Transmitted:");
      Serial.println(text_buf);    
    }
};

class Reciever : public Transiever{
  public:
    Reciever(uint8_t pin_CE, uint8_t pin_CSN, byte *address) : Transiever(pin_CE, pin_CSN, address) {}
    
    void init(){
      RF24 radio(chipEnable, chipSelect);
      radio.begin();
      //set data rate
      radio.setDataRate(RF24_2MBPS);
      //setting address
      radio.openReadingPipe(0, address);
      //starts listenining();
      radio.startListening();
    }

    char recieve(){
      static char recieve_buf[32] = {0};
      radio.read(recieve_buf, sizeof(recieve_buf));
      
      displayRecieved(recieve_buf);

      return recieve_buf;
    }

    void displayRecieved(char text_buf){
      Serial.print("Received:");
      Serial.println(text_buf);    
    }
};

void setup(){
  
}

void loop(){
  
}
