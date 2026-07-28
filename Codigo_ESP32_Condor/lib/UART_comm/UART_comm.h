/*
    UART_comm.h - Librería para comunicación UART. 
    Creado por Kento Chibana 27/06/2026
    Para MECB Cóndor Mótorsports
*/


#ifndef UART_comm_h
#define UART_comm_h

#include "Arduino.h"

#pragma pack(push, 1)
/** Estructura que define todos los datos enviados entre ESPs y a la base de datos. 
 * @param temperatura_BP Temperatura de la batería principal.    
 * @param temperatura_BS Temperatura de la batería secundaria .  
 * @param V_BP           Voltaje de la batería principal. 
 * @param V_BS           Voltaje de la batería secundaria. 
 * @param V_Motor        Voltaje del motor. 
 * @param C_BP           Corriente de la batería principal. 
 * @param C_BS           Corriente de la batería secundaria. 
 * @param C_Motor        Corriente del motor. 
 */
struct API_data 
{
    int16_t temperatura_BP;     /** Temperatura de la batería principal. */    
    int16_t temperatura_BS;     /** Temperatura de la batería secundaria .*/   
    int16_t V_BP;               /** Voltaje de la batería principal. */
    int16_t V_BS;               /** Voltaje de la batería secundaria. */
    int16_t V_Motor;            /** Voltaje del motor. */
    int16_t C_BP;               /** Corriente de la batería principal. */
    int16_t C_BS;               /** Corriente de la batería secundaria. */
    int16_t C_Motor;            /** Corriente del motor. */
};
#pragma pack(pop)

/**
 * Define un objeto de la clase UART_comm para iniciar una transmisión UART entre las ESPs.
 * @param RX_pin Número del pin de recepción de datos (RX).
 * @param TX_pin Número del pin de envío de datos (TX).
 * @param baud_rate Velocidad de transmisión en bits por segundo.
 * @param serial_channel Canal de comunicación serial. Serial1 o Serial2 por ejemplo.
*/
class UART_comm
{
    public:
        UART_comm(int RX_pin, int TX_pin, int baud_rate, HardwareSerial &serial_channel);
        void begin(SerialConfig config = SERIAL_8N1);
        void send(API_data data, GY_85_data gy85);
        void receive(API_data data, GY_85_data gy85);

    private:
        int _rx_pin;
        int _tx_pin;
        int _baud;
        HardwareSerial _serial;
};



#endif