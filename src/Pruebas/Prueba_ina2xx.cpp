#include "Arduino.h"
#include "Telemetria.h"

#define SDA 21
#define SCL 22



Telemetria tele(SDA, SCL, 1, 0, 0, false, false, true, false);

void setup(){
    Serial.begin(115200);
    tele.setup();

}

void loop(){
    Serial.print("Corriente: ");
    Serial.print(tele.read_current(1));
    Serial.print("  Voltaje: ");
    Serial.println(tele.read_voltage(1));
}




