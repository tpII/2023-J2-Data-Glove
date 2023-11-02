#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266HTTPClient.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;
const char *ssid = "Personal-4B1-5GHz"; // The name of the Wi-Fi network that will be created
const char *password = "F7C090C4B1";   // The password required to connect to it, leave blank for an open network
String url = "http://192.168.0.";

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  /* WiFi.softAP(ssid, password);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());  */        // Send the IP address of the ESP8266 to the computer

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
    delay(500);
  
  if (!mpu.begin()) {
    Serial.println("No se pudo encontrar un sensor MPU6050.");
    while (1);
  }
  Serial.println("Sensor MPU6050 encontrado!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}

void loop()
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Acelerómetro (m/s^2): ");
  Serial.print(a.acceleration.x);
  Serial.print(", ");
  Serial.print(a.acceleration.y);
  Serial.print(", ");
  Serial.print(a.acceleration.z);
  Serial.println();

  Serial.print("Giroscopio (rad/s): ");
  Serial.print(g.gyro.x);
  Serial.print(", ");
  Serial.print(g.gyro.y);
  Serial.print(", ");
  Serial.print(g.gyro.z);
  Serial.println();

  Serial.print("Temperatura (C): ");
  Serial.println(temp.temperature);

  HTTPClient http;
  WiFiClient client;
  
  if (http.begin(client, url)) //Iniciar conexión
  {
    Serial.print("[HTTP] POST...\n");
    int httpCode = http.POST(g.gyro.y);  // Realizar petición

    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();  // Obtener respuesta
        Serial.println(payload);  // Mostrar respuesta por serial
      }
    }
    else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
  delay(1000);
}