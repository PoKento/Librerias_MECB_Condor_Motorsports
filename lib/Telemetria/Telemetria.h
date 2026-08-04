/*
    Telemetria.h - Librería para lectura de sensores de telemetría (Temperatura y corriente). 
    Creado por Kento Chibana 30/07/2026
    Para MECB Cóndor Mótorsports
*/
#include "Arduino.h"
#include "Adafruit_ADS1X15.h"
#include "UART_comm.h"
#include <Wire.h>
#include "Adafruit_ADS1X15.h"
#include "INA226.h"
#include "OneWire.h"
#include "DallasTemperature.h"

class Telemetria
{
    public:
        Telemetria(int SCL_Pin, int SDA_Pin, int ADDR_Pin, int oneWire_Pin, float rated_Input_Current_HSTS016L, float rated_Supply_Voltage_HSTS016L);
        void setup();
        float read_temp(int battery_index); 
        float read_voltage(int component_index);
        float read_current(int component_index);
        API_data read_all();

    private:
        int _SCL_Pin;
        int _SDA_Pin;
        int _ADDR_Pin;
        int _oneWire_Pin;
        float _rated_Input_Current_HSTS016L;
        float _rated_Supply_Voltage_HSTS016L;
        Adafruit_ADS1115 _ads1; //ADC de voltajes
        Adafruit_ADS1115 _ads2; //ADC de corrientes
        INA226 _Ina226 = INA226(0x40);
        OneWire _ds18b20;
        DallasTemperature _T_sensors;
    
};

