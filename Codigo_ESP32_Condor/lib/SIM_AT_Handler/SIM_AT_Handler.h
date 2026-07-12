/*
    SIM_AT_Handler.h - Librería para comunicación con módulo SIM7600G mediante comandos AT.
    Creado por Kento Chibana 11/07/2026
    Para MECB Cóndor Mótorsports
*/


#ifndef SIM_Client_h
#define SIM_Client_h

#include "Arduino.h"
#include "UART_comm.h"


class SIM_Client
{
    public:
        SIM_Client(int RX_pin, int TX_pin, int PWR_pin, HardwareSerial &serial_channel);
        bool setup();
        bool send(API_data data);
        bool http_init();
        
    private:
        HardwareSerial _serialAT;
        int _RX_pin;
        int _TX_pin;
        int _PWR_pin;


};



#endif