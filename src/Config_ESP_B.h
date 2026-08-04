//Comunicación UART (Serial2)
int RX_uart = 17;
int TX_uart =18;


//GY-85
int SDA = 19;
int SCL = 20;
    //Acelerómetro
    float ACCEL_filter_alpha = 1;
    int ACCEL_range = 4;            //Valores válidos 4, 8, 16, 32 (+-2g, +-4g, +-8g, +-16g)

    //Magnetómetro
    int MAGN_gain = 1;              //Valores válidos del 1 al 8 (0.73 a 4.35 mGauss/LSB)
    float MAGN_decl = 0.3;            //Declinación magnética en ° (Positiva hacia este °E, negativa hacia oeste °W). Debe obtenerse en alguna página web, depende del lugar y de la fecha.


//Telemetría (Usa el mismo SDA y SCL)
int ADS1115_ADDR = 1;
int OneWirePin = 2;
float rated_Input_Current_HSTS016L = 150;
float rated_Supply_Voltage_HSTS016L = 3.3;