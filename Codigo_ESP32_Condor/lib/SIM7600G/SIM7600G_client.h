#ifndef SIM7600G_client_h
#define SIM7600G_client_h

#include "Arduino.h"
#include "HardwareSerial.h"

class SIM7600G_client
{
    public:
        SIM7600G_client(int RX_pin, int TX_pin, int PWR_pin, HardwareSerial &serial_channel);
        bool begin();
        bool connect_to_server(char server[], int port);
        
        
    private:
        HardwareSerial _serialAT;
        TinyGsm _modem;
        TinyGsmClient _client;
        int _RX_pin;
        int _TX_pin;
        int _PWR_pin;
        
};



#endif