#include "Arduino.h"
#include "TFT_ST7735.h"

#define CS 1
#define DC 2
#define MOSI 3
#define SCLK 4
#define RST 5

TFT_ST7735 tft(CS, DC, MOSI, SCLK, RST);

void setup(){
    Serial.begin(115200);
    tft.begin();
    tft.condor_logo(10,10);
}

void loop(){
    if(Serial.available()){
        tft.write(31620,Serial.readStringUntil('\n'),5,10,10);
    }
}