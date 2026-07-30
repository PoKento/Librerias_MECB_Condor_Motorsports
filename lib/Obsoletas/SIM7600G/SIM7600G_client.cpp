#include "SIM7600G_client.h"

#define TINY_GSM_MODEM_SIM7600  //Definimos el módulo Sim. Este se debe definir antes de llamar a la librería TinyGsmClient.h

#include "Arduino.h"
#include "TinyGsmClient.h"
#include "StreamDebugger.h"
#include <iostream>
#include <memory>
#include "../config.h"

/**
 * 
 */
SIM7600G_client::SIM7600G_client(int RX_pin, int TX_pin, int PWR_pin, HardwareSerial &serial_channel): _serialAT(serial_channel), _modem(serial_channel), _client(_modem){
    _RX_pin = RX_pin;
    _TX_pin = TX_pin;
    _PWR_pin = PWR_pin;

}

/**
 * Inicializa la conexión con 
 */
bool SIM7600G_client::begin(){
    pinMode(_RX_pin, INPUT);
    pinMode(_TX_pin, OUTPUT);
    pinMode(_PWR_pin, OUTPUT);
    digitalWrite(_PWR_pin, HIGH);
    _serialAT.begin(115200, SERIAL_8N1, _RX_pin, _TX_pin);
    delay(10);


    bool initial = _modem.init();

    if (!initial){
        digitalWrite(_PWR_pin, LOW);
        delay(1000); // Una señal de 0 durante 0,5 segundos marca encendido o apagado del módulo. Nos aseguramos con 1 segundo.
        digitalWrite(_PWR_pin, HIGH);
        delay(10);
        initial = _modem.init();
    }


    #ifdef GSM_Pin
        _modem.simUnlock(GSM_Pin);      //Desbloqueando acceso a la SIM si es que existe contraseña.
    #endif

    #ifdef GPRS_User 
        _modem.gprsConnect(APN, GPRS_User, GPRS_Pass);  //Conectándose a la red.
    #else
        _modem.gprsConnect(APN);
    #endif
    delay(10);


    return _modem.isGprsConnected(); 
}

/**
 * 
 */
bool SIM7600G_client::connect_to_server(char server[], int port){
    if (!_client.connect(server, port)) {

    return false;

    return true;
  }
}