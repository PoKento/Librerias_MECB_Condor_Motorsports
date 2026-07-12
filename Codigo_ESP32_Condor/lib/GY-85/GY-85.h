/*
    GY-85.h - Librería para comunicación con el sensor GY-85. 
    Creado por Kento Chibana 29/06/2026
    Para MECB Cóndor Mótorsports
*/


#ifndef GY_85_h
#define GY_85_h

#include "Arduino.h"



class GY_85
{
    public:
        GY_85(int SDA_pin, int SCL_pin);
        void begin();
        std::tuple<int16_t,int16_t,int16_t,int16_t,int16_t> read_accel();
        int16_t read_magnet();
        std::tuple<int16_t, int16_t, int16_t> read_gyro();
        void config_accel(float accel_alpha, int accel_range);
        void config_magnet(int magn_gain_lvl, float magn_decl);
        
    private:
        int _SDA_pin;
        int _SCL_pin;
        float _accel_alpha;
        int _accel_range;
        float _magn_gain;
        float _magn_decl;
        float _Gconv;
        
};



#endif