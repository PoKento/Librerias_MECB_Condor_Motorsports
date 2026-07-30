/*
    SIM_AT_Handler.h - Librería para comunicación con módulo SIM7600G mediante comandos AT.
    Creado por Kento Chibana 11/07/2026
    Para MECB Cóndor Mótorsports
*/


#ifndef SIM_influxdb_h
#define SIM_influxdb_h

#include "Arduino.h"
#include "UART_comm.h"


#pragma pack(push, 1)
/** Estructura que define todos los datos recibidos del GPS. 
 * @param lat       Latitud         dd°mm.mmmmmm'[N/S]
 * @param log       Longitud        ddd°mm.mmmmmm'[E/O]
 * @param date      Fecha Actual    dd/mm/yyyy
 * @param UTC_time  Hora            hh:mm ss.s
 * @param alt       Altitud         metros (m)
 * @param speed     Velocidad       nudos (kn)
 * @param course    Dirección       grados respecto al norte (°)
 */
struct gps_data 
{
    String lat;         //Latitud                       dd°mm.mmmmmm'[N/S]
    String log;         //Longitud                      ddd°mm.mmmmmm'[E/O]
    String date;        //Fecha                         dd/mm/yyyy
    String UTC_time;    //Hora UTC                      hh:mm ss.s
    String alt;         //Altitud                       metros
    String speed;       //Velocidad                     Nudos
    String course;      //Rumbo                         ° (grados)
};
#pragma pack(pop)

class SIM_Client
{
    public:
        SIM_Client(int RX_pin, int TX_pin, int PWR_pin, HardwareSerial &serial_channel, Point &sensor);
        bool setup();
        bool send_http(API_data data);
        bool send(API_data data);
        bool http_init();
        bool gps_init();
        gps_data gps_read();

        
        
    private:
        HardwareSerial _serialAT;
        int _RX_pin;
        int _TX_pin;
        int _PWR_pin;
        Point _sensor;


};



#endif