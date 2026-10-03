#include "Arduino.h"
#include "Telemetria.h"

#define SDA 21
#define SCL 22



Telemetria tele(SDA, SCL, 1, 0, 0, false, true, false, false);

void setup(){
    Serial.begin(115200);
    tele.setup();

}

void loop(){
    Serial.print("VBP: ");
    Serial.print(tele.read_voltage(0));
    Serial.print("VM: ");
    Serial.print(tele.read_voltage(2));
    Serial.print("CBP: ");
    Serial.print(tele.read_current(0));
    Serial.print("CM: ");
    Serial.println(tele.read_current(2));
}




