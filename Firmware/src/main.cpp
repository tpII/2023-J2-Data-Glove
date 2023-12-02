#include <wifi.h>
#include <mpu.h>

String data; 
int count;

void setup() {
  //Iniciamos la comunicacion serial
  Serial.begin(115200);

  //Inicializamos la red
  WiFiSetup();

  //Inicializamos el MPU6050 con sus respectivos rangos
  MPUSetup();
  pinMode(ROTARY_ANGLE_SENSOR, INPUT);
  count = 0;
}

void loop() {
  //Leemos las medidas provenientes del MPU
  data = MPURead();
  if(count == 3){
    count = 0;
    WiFiSend(data);
  }
  delay(20);
  count++;
}
