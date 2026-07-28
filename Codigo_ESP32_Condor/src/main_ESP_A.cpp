/*ESP32-A 
    Core 0: Comunicación por el módulo WiFi y UART.
    Core 1: Control de Relés y reactivación del circuito.
*/
#include "Arduino.h"
#include "UART_comm.h"
#include "TFT_ST7735.h"
#include "SIM_influxdb.h"
#include "Config_ESP_A.h"
#include "InfluxDbClient.h"
#include "InfluxDbCloud.h"
#include "GY-85.h"

// Protección para datos compartidos entre núcleos
portMUX_TYPE dataMux = portMUX_INITIALIZER_UNLOCKED;

Point sensor("Sensor_Data");
SIM_Client client(RX_comm, TX_comm, PWR_comm, Serial1, sensor);
UART_comm uart(RX_uart, TX_uart, 115200, Serial2);
TFT_ST7735 tft_1(CS_pin_tft_1, DC_pin, MOSI_pin, SCLK_pin, RST_pin);
TFT_ST7735 tft_2(CS_pin_tft_2, DC_pin, MOSI_pin, SCLK_pin, RST_pin);

API_data telemetry_data;
gps_data GPS_data;
GY_85_data GY85_data;

void task_InfluxComm(void *pv){
    client.send(telemetry_data);
    vTaskDelay(pdMS_TO_TICKS(500)); //Ejecutar esta tarea cada medio segundo
}

void task_gps(void *pv){
    portENTER_CRITICAL(&dataMux);
    GPS_data = client.gps_read();
    portEXIT_CRITICAL(&dataMux);
    
    vTaskDelay(pdMS_TO_TICKS(1000)); //Ejecutar esta tarea cada segundo
}

void task_UART(void *pv){
    portENTER_CRITICAL(&dataMux);
    uart.receive(telemetry_data, GY85_data);
    portEXIT_CRITICAL(&dataMux);
    vTaskDelay(pdMS_TO_TICKS(500)); //Ejecutar esta tarea cada medio segundo
}

void task_TFT1(void *pv){
    tft_1.update_orientation(128, 160, GY85_data.Roll, GY85_data.Pitch);
    vTaskDelay(pdMS_TO_TICKS(500)); //Ejecutar esta tarea cada medio segundo

}

void task_TFT2(void *pv){
    tft_2.update_telemetry(telemetry_data, GPS_data);
    vTaskDelay(pdMS_TO_TICKS(750)); //Delay de 750ms
    if (telemetry_data.temperatura_BP > MaxTempBP*0.9 || telemetry_data.temperatura_BS > MaxTempBS*0.9){
        tft_2.alert();
    }
    vTaskDelay(pdMS_TO_TICKS(750)); //Delay de 750ms
}




void setup(){
    client.setup();
    client.gps_init();
    uart.begin();
    tft_1.begin();
    tft_2.begin();


    // Núcleo 0: Comunicación por internet, GPS y UART
    xTaskCreatePinnedToCore(task_InfluxComm, "InfluxComm", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(task_gps, "GPS", 4096, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(task_UART, "UART", 4096, NULL, 3, NULL, 0);
  
    // Núcleo 1: Pantalla y alertas
    xTaskCreatePinnedToCore(task_TFT1, "TFT1", 4096, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(task_TFT2, "TFT2", 4096, NULL, 2, NULL, 1);

}






void loop(){
  
}