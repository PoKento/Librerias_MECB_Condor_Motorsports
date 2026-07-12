#include <Arduino.h>
#include <UART_comm.h>

UART_comm uart(12, 13, 9600, Serial1);

API_data datos;

void setup() {
    uart.begin();
    
}

void loop() {
    
}
