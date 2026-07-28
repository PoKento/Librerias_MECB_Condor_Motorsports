#include "Arduino.h"
#include "UART_comm.h"
#include "TFT_ST7735.h"
#include "SIM_influxdb.h"
#include "Relay_Control.h"
#include "GY-85.h"
#include "Config_ESP_B.h"

// Protección para datos compartidos entre núcleos
portMUX_TYPE dataMux = portMUX_INITIALIZER_UNLOCKED;

UART_comm uart(RX_uart, TX_uart, 115200, Serial2);
GY_85 gy85(GY_85_SDA, GY_85_SCL);

API_data telemetry_data;
GY_85_data GY85_data;



void task_GY85(void *pv){
    portENTER_CRITICAL(&dataMux);
    GY85_data = gy85.read_all();
    portEXIT_CRITICAL(&dataMux);
    vTaskDelay(pdMS_TO_TICKS(500)); //Ejecutar esta tarea cada medio segundo
    
}

void task_UART(void *pv){
    uart.send(telemetry_data, GY85_data);
    vTaskDelay(pdMS_TO_TICKS(500)); //Ejecutar esta tarea cada medio segundo
}

void task_Tele(void *pv){
    portENTER_CRITICAL(&dataMux);
    telemetry_data;
    portEXIT_CRITICAL(&dataMux);
    vTaskDelay(pdMS_TO_TICKS(500)); //Ejecutar esta tarea cada medio segundo
}

void setup(){
    uart.begin();
    gy85.begin();
    gy85.config_accel(ACCEL_filter_alpha, ACCEL_range);
    gy85.config_magnet(MAGN_gain, MAGN_decl);



    // Núcleo 0: Lectura de sensores y UART
    xTaskCreatePinnedToCore(task_Tele, "Telemetría", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(task_GY85, "GY85", 4096, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(task_UART, "UART", 4096, NULL, 3, NULL, 0);
  
    // Núcleo 1: 
    //xTaskCreatePinnedToCore(task_TFT1, "TFT1", 4096, NULL, 2, NULL, 1);
   
}


void loop(){
  
}


