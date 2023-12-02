#include <wifi.h>
#include <mpu.h>

String data; 
int count;

void setup() {
  Serial.begin(115200);
  WiFiSetup();
  MPUSetup();
  pinMode(ROTARY_ANGLE_SENSOR, INPUT);
  count = 0;
}

void loop() {
  // Maneja las solicitudes del servidor
  data = MPURead();
  if(count == 3){
    count = 0;
    WiFiSend(data);
  }
  delay(20);
  count++;
}
