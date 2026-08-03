#include "Telemetria.h"






Telemetria::Telemetria(int SCL_Pin, int SDA_Pin, int ADDR_Pin, int oneWire_Pin, float rated_Input_Current_HSTS016L, float rated_Supply_Voltage_HSTS016L, Adafruit_ADS1115 &ads1, Adafruit_ADS1115 &ads2):_ads1(ads1),_ads2(ads2){
    _SCL_Pin = SCL_Pin;
    _SDA_Pin = SDA_Pin;
    _ADDR_Pin = ADDR_Pin;
    _oneWire_Pin = oneWire_Pin;
    _rated_Input_Current_HSTS016L = rated_Input_Current_HSTS016L;
    _rated_Supply_Voltage_HSTS016L = rated_Supply_Voltage_HSTS016L;
}


void Telemetria::setup(){
    _ads1.setGain(GAIN_TWO);
    _ads2.setGain(GAIN_TWO);
    _ads1.begin(0x48);           //ADDR -> GND
    _ads2.begin(0x49);           //ADDR -> 5V
    _ads1.setDataRate(RATE_ADS1115_250SPS);
    _ads2.setDataRate(RATE_ADS1115_250SPS);
}



/**
 *Lee la temperatura de la batería indicada.
 @param battery_index Índice que define la batería (0 Batería Primaria, 1 Batería Secundaria). 
 @return Devuelve el valor de la temperatura en °C (short).
*/
float Telemetria::read_temp(int battery_index){


}

/**
 *Lee el voltaje del componente indicado.
 @param component_index Índice que define el componente (0 Batería Primaria, 1 Batería Secundaria, 2 Motor). 
 @return Devuelve el valor del voltaje en V (float).
*/
float Telemetria::read_voltage(int component_index){
    if (component_index == 0){
        return _ads1.computeVolts(_ads1.readADC_Differential_0_1());
    }
    else if (component_index == 1){
        //Lectura del Ina226
    }
    else if (component_index == 2){
        return _ads1.computeVolts(_ads1.readADC_Differential_2_3());
    }
    return 0;
}

/**
 *Lee la corriente del componente indicado.
 @param component_index Índice que define el componente (0 Batería Primaria, 1 Batería Secundaria, 2 Motor). 
 @return Devuelve el valor de corriente en A (float).
*/
float Telemetria::read_current(int component_index){
    if (component_index == 0){
        return ((_ads2.computeVolts(_ads2.readADC_Differential_0_1())-(_rated_Supply_Voltage_HSTS016L/2))/0.625)*_rated_Input_Current_HSTS016L;
    }
    else if (component_index == 1){
        //Lectura del Ina226
    }
    else if (component_index == 2){
        return ((_ads2.computeVolts(_ads2.readADC_Differential_2_3())-(_rated_Supply_Voltage_HSTS016L/2))/0.625)*_rated_Input_Current_HSTS016L;
    }
    return 0;

}

API_data Telemetria::read_all(){

}