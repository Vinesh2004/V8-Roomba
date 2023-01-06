#ifndef RF_COMMS_H
#define RF_COMMS_H

#include "Arduino.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define BOT_TX "00001"
#define BOT_RX "00002"

typedef struct{
    // Sensor info
    // Info roomba would send to dock
} dataOut_t;

typedef struct{
    // Dock commands (a State + optional values)
    // Position info from CV?
    // other relevent info
} dataIn_t;

class Rf_Comms
{
    private:
        int chipEnable;
        int chipSelect;
        
        byte addresses[][6];
        bool dock;

        void initAddresses();

        RF24 radio;

    public:
        Rf_Comms();
        Rf_Comms(int CE, int CSN, bool dock_in);

        void init();

        void Dock_readData(dataOut_t &roombaData);
        void Dock_sendData(dataIn_t const &dataPackage);

        void Bot_readData(dataIn_t &dockData);
        void Bot_sendData(dataOut_t const &dataPackage);

};

#endif 