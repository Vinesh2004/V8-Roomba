#include "RF_Comms.hpp"


void Rf_Comms::initAddresses()
{
    if(!(*this).dock)
    {
        (*this).addresses = {BOT_TX, BOT_RX};
    }
    else
    {
        (*this).addresses = {BOT_RX, BOT_TX};
    }
}

Rf_Comms::Rf_Comms()
{
    (*this).chipEnable = 7;
    (*this).chipSelect = 8;

    (*this).dock = false;
    initAddresses();
}

Rf_Comms::Rf_Comms(int CE, int CSN, bool dock_in)
{
    (*this).chipEnable = CE;
    (*this).chipSelect = CSN;

    (*this).dock = dock_in;
    initAddresses();
    init();
}

void Rf_Comms::init()
{
    (*this).radio = radio((*this).chipEnable,
                          (*this).chipSelect);
    (*this).radio.begin();
    (*this).radio.openWritingPipe((*this).addresses[0]);
    (*this).radio.openReadingPipe(1, addresses[1]);
    (*this).radio.setPALevel(RF24_PA_LOW); // LOW power level
    radio.setDataRate(RF24_250KBPS); // Max dist slowest speed
}

void Rf_Comms::Dock_readData(dataOut_t &roombaData)
{
    delay(1);
    (*this).radio.startListening();
    if((*this).radio.available())
    {
        (*this).radio.read(&roombaData,sizeof(roombaData));
    }
}

void Rf_Comms::Dock_sendData(dataIn_t const &dataPackage)
{
    delay(1);
    (*this).radio.stopListening();
    (*this).radio.write(&dataPackage, sizeof(dataPackage));
}

void Rf_Comms::Bot_readData(dataIn_t &dockData)
{
    delay(1);
    (*this).radio.startListening();
    if((*this).radio.available())
    {
        (*this).radio.read(&dockData,sizeof(dockData));
    }
}
void Rf_Comms::Bot_sendData(dataOut_t const &dataPackage)
{
    delay(1);
    (*this).radio.stopListening();
    (*this).radio.write(&dataPackage, sizeof(dataPackage));
}