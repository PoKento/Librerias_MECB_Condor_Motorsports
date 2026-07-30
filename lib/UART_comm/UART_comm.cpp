#include "Arduino.h"
#include "UART_comm.h"
#include <iostream>
#include <cstring>
#include "GY-85.h"



UART_comm::UART_comm(int RX_pin, int TX_pin, int baud_rate,  HardwareSerial &serial_channel) : _serial(serial_channel){
    _rx_pin = RX_pin;
    _tx_pin = TX_pin;
    _baud = baud_rate;
}

/**
 * Inicializa el canal de comunicación definiendo los pines RX y Tx como input y output, además de iniciar el canal serial con el baudrate definido.
 * @param config Configuración de paquetes datos (Debe ser la misma para emisor y receptor). Ej: 8 bits de datos 0 de paridad (SERIAL_8N1   default), 8 bits de datos 1 de paridad par (SERIAL_8E1).
*/
void UART_comm::begin(SerialConfig config){
    pinMode(_rx_pin, INPUT);
    pinMode(_tx_pin, OUTPUT);
    _serial.begin(_baud, config, _rx_pin, _tx_pin);     
}

/**
 * Envía la estructura de datos a través del canal serial en paquetes de 8 bits.
 * @param data Estructura del tipo API_data con los datos a enviar.
 * @param gy85 Estructura del tipo GY_85_data con los datos a enviar.
*/
void UART_comm::send(API_data data, GY_85_data gy85){
    uint8_t* ptr = (uint8_t*)&data; //Casting del puntero de la estructura a un puntero de 8 bits.

    for (byte i=0 ; i < sizeof(data) ;i++){
        _serial.write(*ptr++);      //Envío de 8 bits actuales y avance de la variable ptr en 1.
    }

    uint8_t* ptr = (uint8_t*)&gy85; //Casting del puntero de la estructura a un puntero de 8 bits.

    for (byte i=0 ; i < sizeof(gy85) ;i++){
        _serial.write(*ptr++);      //Envío de 8 bits actuales y avance de la variable ptr en 1.
    }


}


/**
 * Lee los datos del buffer serial y los reemplaza directamente en el sitio de memoria de la estructura entregada. En caso de no haber suficientes bytes en el buffer de recepción no hace nada.
 * @param data Estructura de datos API_data que se quiera actualizar con los datos entrantes.
 * @param gy85 Estructura de datos del sensor GY-85 que se quiera actualizar.
*/
void UART_comm::receive(API_data data, GY_85_data gy85){

    if (_serial.available() > sizeof(API_data)+sizeof(gy85)-1){
        int received_bytes = 0;
        uint8_t byte_array_tele[sizeof(API_data)];
        uint8_t byte_array_gy85[sizeof(GY_85_data)];
        received_bytes += _serial.readBytes(byte_array_tele, sizeof(API_data));
        received_bytes += _serial.readBytes(byte_array_gy85, sizeof(GY_85_data));

        std::memcpy(&data, byte_array_tele, sizeof(data));
        std::memcpy(&gy85, byte_array_gy85, sizeof(gy85));

    }




    /*

    if (_serial.available()%sizeof(API_data)==0 && _serial.available()>0){//Nos aseguramos de que hayan llegado una cantidad entera de datos.
        delay((12/_baud)*1000); //Esperamos a que se lea 1 byte extra (10 bits, start-stop-8 datos + 2 bits de tiempo extra)

        if (_serial.available()%sizeof(API_data)==0){ //Suponiendo que la frecuencia a la que se envían nuevos datos no es la máxima posible, si el valor del buffer cambió significa que en algún punto se perdió un byte y la lectura está desfasada.
            received_bytes = _serial.readBytes(byte_array, sizeof(API_data));

            if (received_bytes == 16){
            std::memcpy(&data, byte_array, sizeof(data));
            }
        }
        else{
            delay((12/_baud)*1000*20); // Dejamos tiempo para que llegue el resto de la estructura.
            while (_serial.available()>0){
                _serial.read();
            }
        }
    }
    */



}