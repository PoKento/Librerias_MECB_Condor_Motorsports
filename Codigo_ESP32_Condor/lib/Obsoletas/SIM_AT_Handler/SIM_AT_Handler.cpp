#include "SIM_AT_Handler.h"

#include "Arduino.h"
#include "..\GPRS_config.h"

//Función para separar strings por delimitador. Extraída de https://stackoverflow.com/questions/29671455/how-to-split-a-string-using-a-specific-delimiter-in-arduino
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


/**
 * Inicializa la ESP como cliente del módulo 7600G mediante un canal serial.
 * @param RX_pin Número del pin de recepción de la comunicación serial.
 * @param TX_pin Número del pin de transmisión de la comunicación serial.
 * @param serial_channel Canal serial a utilizar (Serial, Serial1, Serial2).
 */
SIM_Client::SIM_Client(int RX_pin, int TX_pin, int PWR_pin, HardwareSerial &serial_channel):_serialAT(serial_channel){
    _RX_pin = RX_pin;
    _TX_pin = TX_pin;
    _PWR_pin = PWR_pin;

}

/**
 * Inicializa la comunicación con el módulo SIM7600G.
 * @return true si se inicializó correctamente, false si algo falló
 */
bool SIM_Client::setup(){
    pinMode(_RX_pin, INPUT);
    pinMode(_TX_pin, OUTPUT);
    pinMode(_PWR_pin, OUTPUT);
    digitalWrite(_PWR_pin, HIGH);

    _serialAT.begin(115200, SERIAL_8N1, _RX_pin, _TX_pin);
    delay(10);

    while (_serialAT.available()){
        _serialAT.readStringUntil('\n');    //Limpiar el buffer de posible mensaje "RDY"
    }

    delay(10);
    _serialAT.println("AT");                //Enviamos commando de atención (Esperamos una respuesta "OK")
    int start_time = millis();
    int timeout = start_time;
   
    while(_serialAT.available()==0){        //Entra en loop hasta que se logra recibir un mensaje
        int timeout = millis();
        if (timeout-start_time > 1000){
            digitalWrite(_PWR_pin, LOW);
            delay(1000);                    //Probar "pulsar" el botón de encendido
            digitalWrite(_PWR_pin, HIGH);
            delay(20000);                   //Espera a que se encienda el módulo
            _serialAT.println("AT");
            start_time = millis();
        }
    }

    if(_serialAT.readStringUntil('\n') == "ERROR"){     //Conexión existente, pero mensaje de error
        return false;
    };

    while (_serialAT.available()){
        _serialAT.readStringUntil('\n');    //Limpiar el buffer de posible duplicado de OK por aparición de RDY.
    }

    _serialAT.println("AT+CNMP=38");                //Intentamos fijar modo sólo LTE (4G)

    while (!_serialAT.available()){}                //Espera a recibir respuesta

    if (_serialAT.readStringUntil('\n') == "ERROR"){
        _serialAT.println("AT+CNMP=2");             //Si no está disponible fijamos el modo automático
    }

    while (!_serialAT.available()){}

    if (_serialAT.readStringUntil('\n') == "ERROR"){
        return false;
    }
    
    _serialAT.print("AT+CGDCONT=1,IP,");            
    _serialAT.println(APN);                         //Conexión con el proveedor del servicio de internet
    
    while (!_serialAT.available()){}                //Espera a recibir respuesta

    if (_serialAT.readStringUntil('\n') == "ERROR"){
        return false;
    }


       
    _serialAT.println("AT+CGATT=1");                //Activar conexión con el "Packet Domain service"
    
    while (!_serialAT.available()){}                //Espera a recibir respuesta

    if (_serialAT.readStringUntil('\n') == "ERROR"){
        return false;
    }



    
    

    return true;
}


/**
 * Inicializa el protocolo HTTP de comunicación mediante la SIM7600G.
 * @return true si se inicializó correctamente, false si algo falló
 */
bool SIM_Client::http_init(){
    
    _serialAT.println("AT+HTTPINIT");               //Inicia la comunicación por HTTP(s)
    
    while (!_serialAT.available()){}                //Espera a recibir respuesta

    if (_serialAT.readStringUntil('\n') == "ERROR"){
        return false;
    }



    _serialAT.println('AT+HTTPPARA="CID",1');       //Se conecta con la configuración hecha en setup()
    
    while (!_serialAT.available()){}                //Espera a recibir respuesta

    if (_serialAT.readStringUntil('\n') == "ERROR"){
        return false;
    }

    
    _serialAT.print('AT+HTTPPARA="URL",');       //Conexión con el servidor
    _serialAT.println(API_URL);

    while (!_serialAT.available()){}                //Espera a recibir respuesta

    if (_serialAT.readStringUntil('\n') == "ERROR"){
        return false;
    }


    _serialAT.println('AT+HTTPPARA="CONTENT","application/octet-stream"');       //Definimos que los datos a enviar son bytes.

    while (!_serialAT.available()){}                //Espera a recibir respuesta

    if (_serialAT.readStringUntil('\n') == "ERROR"){
        return false;
    }


}

/**
 * Envía la estructura de datos con los valores de sensores de temperatura, voltaje y corriente a la base de datos separándola en bytes.
 * @param data Datos a enviar en el formato de struct API_data.
 * @return true si se envió correctamente, false si algo falló
 */
bool SIM_Client::send(API_data data){
    _serialAT.println('AT+HTTPDATA=16, 5');       //Definimos el tamaño del mensaje (16 bytes) y el máximo tiempo de envío (5s).

    uint8_t* ptr = (uint8_t*)&data; //Casting del puntero de la estructura a un puntero de 8 bits.

    for (byte i=0 ; i < sizeof(data) ;i++){
        _serialAT.write(*ptr++);      //Envío de 8 bits actuales y avance de la variable ptr en 1.
    }
    
    while (!_serialAT.available()){}                //Espera a recibir respuesta

    if (_serialAT.readStringUntil('\n') == "ERROR"){
        return false;
    }

    _serialAT.println('AT+HTTPACTION=1');       //Enviamos el buffer de datos

    while (!_serialAT.available()){}                //Espera a recibir respuesta

    if (_serialAT.readStringUntil('\n') == "ERROR"){
        return false;
    }

    while (_serialAT.available()){_serialAT.readStringUntil('\n');}                //Vacía el segundo mensaje de respuesta

    return true;
}

/**
 * Inicializa el GPS. 
 */
bool SIM_Client::gps_init(){
    _serialAT.println('AT+CGPS=1');       //Iniciamos la sesión de GPS

    while (!_serialAT.available()){}                //Espera a recibir respuesta (Puede demorar hasta 5 minutos)

    if (_serialAT.readStringUntil('\n') == "ERROR"){
        return false;
    }

}

/**
 * Recibe los datos del gps.
 * @return Los datos se entregan en el formato de estructura gps_data.
 */
gps_data SIM_Client::gps_read(){
    _serialAT.println('AT+CGPSINFO');       //Solicita la información del GPS

    while (!_serialAT.available()){}                //Espera a recibir respuesta 

    String data = _serialAT.readStringUntil('\n');

    if (data == "ERROR"){
        return;
    }

    gps_data Output_Data;
    Output_Data.lat = data.substring(10,12)+"°"+data.substring(12,21)+"'"+data.substring(22,23);
    Output_Data.log = data.substring(24,27)+"°"+data.substring(27,36)+"'"+data.substring(37,38);
    Output_Data.date = data.substring(39,41)+"/"+data.substring(41,43)+"/20"+data.substring(43,45);
    Output_Data.UTC_time = data.substring(46,48)+":"+data.substring(48,50)+" "+data.substring(46,54);
    String resto = data.substring(55);
    Output_Data.alt = getValue(resto,',',0)+" m";
    Output_Data.speed = getValue(resto,',',1)+" kn";
    Output_Data.course = getValue(resto,',',2)+"°";

    return Output_Data;

}