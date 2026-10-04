#include "Arduino.h"
#include "..\..\lib\config.h"
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <WiFi.h>

// Reemplaza con tus credenciales de red
const char* ssid = "tu_red_wifi";
const char* password = "tu_contraseña";


Point sensor;
int x = 0;

void setup(){
    Serial.begin(115200);

    // Configurar en modo estación (conectarse a un router)
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.print("Conectando a WiFi...");
    
    // Esperar hasta que se establezca la conexión
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    // Conexión exitosa
    Serial.println("\n¡Conectado!");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());






    //Inicialización de conexión con InfluxDB

    // Time zone info
    #define TZ_INFO "UTC-4"     

    // Declare InfluxDB client instance with preconfigured InfluxCloud certificate
    InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
    
    // Declare Data point
    //Point _sensor("Sensor_Data");
    

    // Accurate time is necessary for certificate validation and writing in batches
    // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
    // Syncing progress and the time will be printed to Serial.
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
  
  
    // Check server connection
    Serial.print("Conección: ");
    Serial.println(client.validateConnection());


    // Add tags to the data point
    sensor.addTag("Valores de Prueba", "Tonina Virtual");
}

void loop(){
    x++;
    if (x>100){
        x=0;
    }
    sensor.addField("Valor cte (50)", 50);
    sensor.addField("Rampa", x);
    delay(10);
}