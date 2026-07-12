#include "Arduino.h"
#include "GY-85.h"
#include "Wire.h"


// I²C direcciones de los sensores
#define ADXL345_ADDR 0x53  // Acelerómetro
#define ITG3200_ADDR 0x68  // Giroscopio
#define HMC5883L_ADDR 0x1E // Magnetómetro


/**
 * Inicializa el objeto GY_85 que controla el sensor.
 * @param SDA_pin Número del pin SDA conectado al sensor.
 * @param SCL_pin Número del pin SCL conectado al sensor.
 */
GY_85::GY_85(int SDA_pin, int SCL_pin){
    _SDA_pin = SDA_pin;
    _SCL_pin = SCL_pin;
    _magn_gain = 0.92; //Constante para leer la medición dependiendo de la ganancia del magnetómetro (datasheet).
    _magn_decl = 0.14; //Declinación magnética que agrega error a la medición. Depende del lugar y el momento por lo que hay que actualizarlo constantemente (Santiago 16/01/2026 = 0.14)
    _accel_alpha = 0.2; //Constante para filtro de acelerómetro (El valor entrante tiene un peso de alpha en relación al valor anterior)
    _accel_range = 4; //Rango completo de medición del acelerómetro en Gs.
    _Gconv = 4/(2^10); //Convertidor de medición a G. La fórmula es (rango completo de g)/2^(resolución).            +-2g -> 4g de rango
}

/**
 * Inicia la comunicación con todos los sensores. Seleccionando sus respectivos modos de operación normal de medición.
 */
void GY_85::begin(){
    // Inicializar ADXL345 (Acelerómetro)
    
    Wire.beginTransmission(ADXL345_ADDR);
    Wire.write(0x2D); // Registro de control de alimentación
    Wire.write(0x08); // Fijar modo de medición
    Wire.endTransmission();

    // Inicializar ITG-3200 (Giroscopio)
    Wire.beginTransmission(ITG3200_ADDR);
    Wire.write(0x3E); // Registro de control de alimentación
    Wire.write(0x00); // Fijar modo normal para todos los giroscopios
    Wire.endTransmission();

    // Initialize HMC5883L (Magnetometro)
    Wire.beginTransmission(HMC5883L_ADDR);
    Wire.write(0x00); // Registro de configuración A
    Wire.write(0x00); // Fijamos muestras no promediadas y 15Hz a la salida
    Wire.endTransmission();
    Wire.beginTransmission(HMC5883L_ADDR);
    Wire.write(0x02); // Registro de modo
    Wire.write(0x00); // Fijamos modo de muestreo continuo
    Wire.endTransmission();
}


/**
 * Lectura del acelerómetro. 
 * @return Tupla con el Roll y el Pitch en grados respecto al eje (°), y las aceleraciones filtradas por eje en gs ({Roll, Pitch, aX, aY, aZ}).
 */
std::tuple<int16_t,int16_t,int16_t,int16_t,int16_t> GY_85::read_accel(){
    

    // Leer datos de ADXL345 (Acelerómetro)
    Wire.beginTransmission(ADXL345_ADDR);
    Wire.write(0x32); // Empezar a leer desde el registro 0x32
    Wire.endTransmission(false);
    Wire.requestFrom(ADXL345_ADDR, 6); // Solicitar 6 bytes, 2 por coordenada (X, Y, Z)
    int16_t ax = (Wire.read() | (Wire.read() << 8))*_Gconv;
    int16_t ay = (Wire.read() | (Wire.read() << 8))*_Gconv;
    int16_t az = (Wire.read() | (Wire.read() << 8))*_Gconv;

    int16_t fXg = ax * _accel_alpha + fXg * (1.0 - _accel_alpha);
    int16_t fYg = ay * _accel_alpha + fYg * (1.0 - _accel_alpha);         //Filtrado de señales
    int16_t fZg = az * _accel_alpha + fZg * (1.0 - _accel_alpha);


    int16_t Roll = atan2(fYg, fZg) * 180/PI;                //Transformación a Roll y Pitch
    int16_t Pitch = atan2(-fXg, sqrt(fYg*fYg + fZg*fZg)) * 180/PI;
    
    return {Roll, Pitch, fXg, fYg, fZg};
}


/**
 * Lectura del magnetómetro. 
 * @return Entero que representa los grados respecto del norte magnético. Se debe calibrar usando config_magnet().
 */
int16_t GY_85::read_magnet(){
    // Leer datos de HMC5883L (Magnetómetro)
    Wire.beginTransmission(HMC5883L_ADDR);
    Wire.write(0x1D); // Empezar a leer desde el registro 0x1D
    Wire.endTransmission(false);
    Wire.requestFrom(HMC5883L_ADDR, 6); // Solicitar 6 bytes (X, Y, Z)
    int16_t mx = (Wire.read() | (Wire.read() << 8))*_magn_gain;
    int16_t mz = (Wire.read() | (Wire.read() << 8))*_magn_gain;     //Lecturas del magnetómetro 
    int16_t my = (Wire.read() | (Wire.read() << 8))*_magn_gain;

    int16_t heading = atan2(my,mx)+_magn_decl*PI/180;
    if (heading < 0){
    heading += 2*PI;            //Aseguramos un valor entre 0 y 2Pi
    }
    if (heading > 2*PI){
    heading -= 2*PI;
    }
    return heading*(180/PI);

}

/**
 * @brief Lectura del giroscopio.
 * @return Tupla con tres valores ({gX, gY, gZ}), las aceleraciones angulares de los ejes en °/s.
 */
std::tuple<int16_t, int16_t, int16_t> GY_85::read_gyro(){
    float sensScaleF = 14.375; //Factor de escalamiento del giroscopio (Entregado en el datasheet)
    // Leer datos de ITG3200 (Giroscopio)
    Wire.beginTransmission(ITG3200_ADDR);
    Wire.write(0x1D); // Empezar a leer desde el registro 0x1D
    Wire.endTransmission(false);
    Wire.requestFrom(ITG3200_ADDR, 6); // Solicitar 6 bytes (X, Y, Z)
    int16_t gx = (Wire.read() | (Wire.read() << 8))/sensScaleF;
    int16_t gy = (Wire.read() | (Wire.read() << 8))/sensScaleF;     //Lectura del giroscopio en °/s. Se puede ir sumando en una variable auxiliar para obtener valores de ángulos.
    int16_t gz = (Wire.read() | (Wire.read() << 8))/sensScaleF;

    return {gx, gy, gz};
}


/**
 * Modificación de parámetros del acelerómetro. Si se ingresa un valor de rango inválido, se ignorará y modificará solo el alpha.
 * @param accel_alpha El peso de la última medición en relación con el valor actual de aceleración. (1 no hay filtro, <1 es la proporción que equivale a la ultima medición) Ej: alpha=0.2, filtro pasa bajos en el que la última medición equivale al 20% del valor.
 * @param accel_range Rango completo de valores medibles por el acelerómetro en g. Valores permitidos 4, 8, 16 y 32 (+-2g, +-4g, +-8g, +-16g).
 */
void GY_85::config_accel(float accel_alpha, int accel_range){
    int hex_code;
    _accel_alpha = accel_alpha;
    if (accel_range != _accel_range && (accel_range == 4 || accel_range == 8 || accel_range == 16 || accel_range == 32)){
        _accel_range = accel_range;
        
        switch(accel_range){
            case 4:
                hex_code = 0x00;
            case 8:
                hex_code = 0x01;
            case 16:
                hex_code = 0x02;
            case 32:
                hex_code = 0x03;

        }
        Wire.beginTransmission(ADXL345_ADDR);
        Wire.write(0x31);  //Registro de formato de datos 
        Wire.write(hex_code);  //Definir rango de medición     
        Wire.endTransmission();
        _Gconv = _accel_range/(2^10);
    }
    
};

/**
 * Modificación de parámetros del giroscopio.
 * @param magn_gain_lvl Define el nivel de ganancia que utiliza el giroscopio. Con este se define la precisión del sensor. Valores válidos del 1 al 8 (0.73 a 4.35 mGauss/LSB).
 * @param magn_decl Declinación magnética en ° (Positiva hacia este °E, negativa hacia oeste °W). Debe obtenerse en alguna página web, depende del lugar y de la fecha.
 */
void GY_85::config_magnet(int magn_gain_lvl, float magn_decl){
    int hex_code;
    _magn_decl = magn_decl;
    switch(magn_gain_lvl){
        case 1:
            _magn_gain = 0.73;
            hex_code = 0x00;
        case 2:
            _magn_gain = 0.92;
            hex_code = 0x20;
        case 3:
            _magn_gain = 1.22;
            hex_code = 0x40;
        case 4:
            _magn_gain = 1.52;
            hex_code = 0x60;
        case 5:
            _magn_gain = 2.27;
            hex_code = 0x80;
        case 6:
            _magn_gain = 2.56;
            hex_code = 0xA0;
        case 7:
            _magn_gain = 3.03;
            hex_code = 0xC0;
        case 8:
            _magn_gain = 4.35;    
            hex_code = 0xE0;
    }

    Wire.beginTransmission(HMC5883L_ADDR);
    Wire.write(0x01); // Registro de configuración B
    Wire.write(hex_code); // Fijamos ganancia seleccionada
    Wire.endTransmission();

};