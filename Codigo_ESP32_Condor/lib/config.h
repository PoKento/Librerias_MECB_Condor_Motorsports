//Configuraciones de modem SIM7600

#define API_URL "http://ejemplo.com" //URL de la api de base de datos
#define APN "bam.entelpcs.cl"   //Nombre del punto de acceso. Ej: (Entel: bam.entelpcs.cl)

#define GSM_Pin "1234"          //Pin de acceso. Usualmente 0000 o 1234.  Si no es necesario iniciar sesión comentar esta linea.

#define GPRS_User "Tonina"      //Nombre del usuario del servicio.      Si no es necesario iniciar sesión comentar esta linea.
#define GPRS_Pass "123456"      //Contraseña                            Si no es necesario iniciar sesión comentar esta linea.  



//Configuraciones de InfluxDB

#define INFLUXDB_URL "http://your-influx-host:8086"     //Reemplazar por la dirección del servidor en el que se Hostea la instancia de InfluxDB.
#define INFLUXDB_ORG "caffdd9942df495a"                 //ID de la organización en InfluxDB
#define INFLUXDB_BUCKET "Datos_Tonina"
#define INFLUXDB_TOKEN "6031a4db7cc388d134d7185d18aa1609de8f49b51999b2526aa1ee552269af5c" //Token generado aleatoriamente. Se genera en la plataforma de InfluxDB.
