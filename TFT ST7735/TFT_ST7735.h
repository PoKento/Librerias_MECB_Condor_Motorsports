/*
    UART_comm.h - Librería para comunicación con pantallas TFT_ST7735. 
    Creado por Kento Chibana 29/06/2026
    Para MECB Cóndor Mótorsports
*/


#ifndef TFT_ST7735_h
#define TFT_ST7735_h

#include "Arduino.h"
#include "Adafruit_ST7735.h"

class TFT_ST7735
{
    public:
        TFT_ST7735(int CS_pin, int DC_pin, int MOSI_pin, int SCLK_pin, int RST_pin);
        void begin();
        void write(int16_t color, String text, int size, int pos_X, int pos_Y);
        void condor_logo(int pos_X, int pos_Y);


    private:
        int _CS_pin;
        int _DC_pin;
        int _MOSI_pin;
        int _SCLK_pin;
        int _RST_pin;
        Adafruit_ST7735 _tft = Adafruit_ST7735(_CS_pin, _DC_pin, _MOSI_pin, _SCLK_pin, _RST_pin);
};



#endif