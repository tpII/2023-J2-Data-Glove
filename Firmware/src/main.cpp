#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266HTTPClient.h>

const char *ssid = "TeleCentro-74e9"; // The name of the Wi-Fi network that will be created
const char *password = "MWYXMDMZKZNJ";   // The password required to connect to it, leave blank for an open network
String url = "http://google.com";

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
}

void loop()
{
  HTTPClient http;
  WiFiClient client;

  if (http.begin(client, url)) //Iniciar conexión
  {
    Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();  // Realizar petición

    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();  // Obtener respuesta
        Serial.println(payload);  // Mostrar respuesta por serial
      }
    }
    else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  else {
    Serial.printf("[HTTP} Unable to connect\n");
  }

  delay(30000);
}