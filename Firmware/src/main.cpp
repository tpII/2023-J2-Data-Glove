#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266HTTPClient.h>
#include <Adafruit_MPU6050.h>
#include <filtro.h>
#include <string>

Adafruit_MPU6050 mpu;
const char *ssid = "DataGlove"; // The name of the Wi-Fi network that will be created
const char *password = "laTercera";   // The password required to connect to it, leave blank for an open network
String url = "http://192.168.4.7/data";
extern float SEq_1, SEq_2, SEq_3, SEq_4;
uint16_t count;

void setup() {

  count = 0;
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  WiFi.softAP(ssid, password);             // Start the access point
  Serial.println("Access Point");
  Serial.print(ssid);
  Serial.println("started");

  Serial.println("IP address:");
  Serial.println(WiFi.softAPIP());  

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
  filterUpdate(g.gyro.x, g.gyro.y, g.gyro.z, a.acceleration.x, a.acceleration.y, a.acceleration.z);

  delay(1);
  count++;

  if(count==1000){
    count = 0;
    HTTPClient http;
    WiFiClient client;
    
    if (http.begin(client,url)) //Iniciar conexión
    {
      Serial.println("[HTTP] POST...\n");
      std::string seq_1 = std::to_string(SEq_1);
      std::string seq_2 = std::to_string(SEq_2);
      std::string seq_3 = std::to_string(SEq_3);
      std::string seq_4 = std::to_string(SEq_4);
      std::string dataJsonSTD = "{\"SEq_1\":" + seq_1 + ",\"SEq_2\":" + seq_2 + ",\"SEq_3\":" + seq_3 + ",\"SEq_4\":" + seq_4 + "}";
      String dataJson = String(dataJsonSTD.c_str());
      Serial.println(dataJson);
      http.addHeader("Content-Type", "text/plain");
      int httpCode = http.POST("hola");  // Realizar petición
      Serial.println(http.getSize());

      if (httpCode > 0) {
        Serial.printf("[HTTP] POST... code: %d\r\n", httpCode);

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
  }
}