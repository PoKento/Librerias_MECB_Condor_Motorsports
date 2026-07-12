#include <Wire.h>
#include <Arduino.h>

// I²C direcciones de los sensores
#define ADXL345_ADDR 0x53  // Acelerómetro
#define ITG3200_ADDR 0x68  // Giroscopio
#define HMC5883L_ADDR 0x1E // Magnetómetro

const float alpha = 0.5; //Constante para filtro de acelerómetro
const float Gconv = 4/(2^10); //Convertidor de medición a G. La fórmula es (rango completo de g)/2^(resolución).            +-2g -> 4g de rango

const float sensScaleF = 14.375; //Factor de escalamiento del giroscopio (Entregado en el datasheet)

const float gain = 0.92; //Constante para leer la medición dependiendo de la ganancia del magnetómetro (datasheet).
const float decl = 0.14; //Declinación magnética que agrega error a la medición. Depende del lugar y el momento por lo que hay que actualizarlo constantemente (Santiago 16/01/2026 = 0.14)


void setup() {
  Wire.begin(); // Inicializar comunicación I2C
  //Serial.begin(9600); Comunicación serial para debugging

  // Inicializar ADXL345 (Acelerómetro)
    Wire.beginTransmission(ADXL345_ADDR);
    //Wire.write(0x31);  Registro de formato de datos 
    //Wire.write(0x02);  Definir +-8g como rango de medición      Descomentar para variar el rango de lectura o la resolución (Default +-2g y 10bits)
    //Wire.endTransmission;
    //Wire.beginTransmission(ADXL345_ADDR);
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

void loop() {
  // Leer datos de ADXL345 (Acelerómetro)
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x32); // Empezar a leer desde el registro 0x32
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345_ADDR, 6); // Solicitar 6 bytes (X, Y, Z)
  int16_t ax = (Wire.read() | (Wire.read() << 8))*Gconv;
  int16_t ay = (Wire.read() | (Wire.read() << 8))*Gconv;
  int16_t az = (Wire.read() | (Wire.read() << 8))*Gconv;

  int16_t fXg = ax * alpha + fXg * (1.0 - alpha);
  int16_t fYg = ay * alpha + fYg * (1.0 - alpha);         //Filtrado de señales
  int16_t fZg = az * alpha + fZg * (1.0 - alpha);


  int16_t Roll = atan2(fYg, fZg) * 180/PI;                //Transformación a Roll y Pitch
  int16_t Pitch = atan2(-fXg, sqrt(fYg*fYg + fZg*fZg)) * 180/PI;
  // Print accelerometer data
  
  Serial.print(" X: "); Serial.print(fXg);
  Serial.print(" Y: "); Serial.print(fYg);
  Serial.print(" Z: "); Serial.print(fZg);
  Serial.print(" Roll: "); Serial.print(Roll);
  Serial.print(" Pitch: "); Serial.println(Pitch);
  


  // Leer datos de ITG3200 (Giroscopio)
  Wire.beginTransmission(ITG3200_ADDR);
  Wire.write(0x1D); // Empezar a leer desde el registro 0x1D
  Wire.endTransmission(false);
  Wire.requestFrom(ITG3200_ADDR, 6); // Solicitar 6 bytes (X, Y, Z)
  int16_t gx = (Wire.read() | (Wire.read() << 8))/sensScaleF;
  int16_t gy = (Wire.read() | (Wire.read() << 8))/sensScaleF;     //Lectura del giroscopio en °/s. Se puede ir sumando en una variable auxiliar para obtener valores de ángulos.
  int16_t gz = (Wire.read() | (Wire.read() << 8))/sensScaleF;

  Serial.print(" Gyro X: "); Serial.print(gx);
  Serial.print(" Gyro Y: "); Serial.print(gy);
  Serial.print(" Gyro Z: "); Serial.println(gz);



  // Leer datos de HMC5883L (Magnetómetro)
  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x1D); // Empezar a leer desde el registro 0x1D
  Wire.endTransmission(false);
  Wire.requestFrom(HMC5883L_ADDR, 6); // Solicitar 6 bytes (X, Y, Z)
  int16_t mx = (Wire.read() | (Wire.read() << 8))*gain;
  int16_t mz = (Wire.read() | (Wire.read() << 8))*gain;     //Lecturas del magnetómetro 
  int16_t my = (Wire.read() | (Wire.read() << 8))*gain;

  int16_t heading = atan2(my,mx)+decl*PI/180;
  if (heading < 0){
    heading += 2*PI;            //Aseguramos un valor entre 0 y 2Pi
  }
  if (heading > 2*PI){
    heading -= 2*PI;
  }
  Serial.print(" Heading: "); Serial.print(heading*180/(2*PI)); //Imprimimos el valor en grados.
    
  delay(500); // Delay for readability
}