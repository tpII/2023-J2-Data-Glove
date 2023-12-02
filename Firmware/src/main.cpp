#include <wifi.h>
#include <mpu.h>

//Definimos un String para hacer el pasaje entre lo que leemos del MPU y lo que enviaremos al WebServer
String data;

void setup() {
  //Iniciamos la comunicacion serial
  Serial.begin(115200);

  //Inicializamos la red
  WiFiSetup();

  //Inicializamos el MPU6050 con sus respectivos rangos
  MPUSetup();
}

void loop() {
  //Leemos las medidas provenientes del MPU
  data = MPURead();

  //Enviamos las medidas al WebServer
  WiFiSend(data);
  
  //Esperamos 25ms para sincronizar el filtro
  delay(25);
}
