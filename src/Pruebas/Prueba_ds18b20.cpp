#include "Arduino.h"
#include "Telemetria.h"

#define OneWirePin 14


Telemetria tele( 1, 1, OneWirePin, 0, 0, true, false, false, false);

void setup(){
    Serial.begin(115200);
    tele.setup();

}

void loop(){
    Serial.print("Temperatura: ");
    Serial.println(tele.read_temp(0));
}




