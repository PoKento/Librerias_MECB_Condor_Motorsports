/*
    Relay_Control.h - Librería para control de los relés del catamarán.
    Creado por Kento Chibana 16/07/2026
    Para MECB Cóndor Mótorsports
*/


#ifndef Relay_Control_h
#define Relay_Control_h

#include "Arduino.h"



class Relay_Control
{
    public:
        Relay_Control(int SolarPin, int BattPin);
        void on();
        void off();

    private:
        int _SolarPin;
        int _BattPin;


};



#endif