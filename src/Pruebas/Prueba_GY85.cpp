#include "Arduino.h"
#include "GY-85.h"
#include "..\..\lib\config.h"
#include <iostream>
#include <tuple>

#define SDA 21
#define SCL 22

//GY-85
//Acelerómetro
#define ACCEL_filter_alpha 1
#define ACCEL_range 4            //Valores válidos 4, 8, 16, 32 (+-2g, +-4g, +-8g, +-16g)

//Magnetómetro
#define MAGN_gain 1              //Valores válidos del 1 al 8 (0.73 a 4.35 mGauss/LSB)
#define MAGN_decl 0.3           //Declinación magnética en ° (Positiva hacia este °E, negativa hacia oeste °W). Debe obtenerse en alguna página web, depende del lugar y de la fecha.

GY_85 gy85(SDA, SCL);

void setup(){
    Serial.begin(115200);
    gy85.begin();
    gy85.config_accel(ACCEL_filter_alpha, ACCEL_range);
    gy85.config_magnet(MAGN_gain, MAGN_decl);
}

void loop(){
    Serial.print("Accel X: ");
    Serial.print(std::get<0>(gy85.read_accel()));
    Serial.print("Gyro X: ");
    Serial.print(std::get<0>(gy85.read_gyro()));
    Serial.print("Magn: ");
    Serial.println(gy85.read_magnet());

}