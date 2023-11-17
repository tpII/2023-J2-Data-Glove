#include <wifi.h>
#include <mpu.h>

String data;

void setup() {
  Serial.begin(115200);
  WiFiSetup();
  MPUSetup();
}

void loop() {
  // Maneja las solicitudes del servidor
  data = MPURead();
  WiFiSend(data);
  delay(25);
}
