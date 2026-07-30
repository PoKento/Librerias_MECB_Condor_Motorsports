/*
    GY-85.h - Librería para comunicación con el sensor GY-85. 
    Creado por Kento Chibana 29/06/2026
    Para MECB Cóndor Mótorsports
*/


#ifndef GY_85_h
#define GY_85_h

#include "Arduino.h"


#pragma pack(push, 1)
/** Estructura que define todos los datos recolectados del GY-85.
*@param gx      Lectura eje X giroscopio en °/s
*@param gy      Lectura eje Y giroscopio en °/s
*@param gz      Lectura eje Z giroscopio en °/s 
*@param magnet  Lectura Magnetómetro en ° respecto al norte magnético
*@param Roll    Orientación Roll en ° 
*@param Pitch   Orientación Pitch en ° 
*@param fXg     Aceleración eje X en g 
*@param fYg     Aceleración eje Y en g 
*@param fZg     Aceleración eje Z en g 
 */
struct GY_85_data 
{
    int16_t gx;         /** Lectura eje X giroscopio en °/s */    
    int16_t gy;         /** Lectura eje Y giroscopio en °/s */   
    int16_t gz;         /** Lectura eje Z giroscopio en °/s */
    int16_t magnet;     /** Lectura Magnetómetro en ° respecto al norte magnético */
    short   Roll;       /** Orientación Roll en ° */
    short   Pitch;      /** Orientación Pitch en ° */
    int16_t fXg;        /** Aceleración eje X en g */
    int16_t fYg;        /** Aceleración eje Y en g */
    int16_t fZg;        /** Aceleración eje Z en g */
};
#pragma pack(pop)

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
        GY_85_data read_all();

        
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