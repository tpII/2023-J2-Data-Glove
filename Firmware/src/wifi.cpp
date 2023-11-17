#include <wifi.h>

const char *ssid = "DataGlove";
const char *password = "laTercera";

const char *serverAddress = "192.168.4.7";
const int serverPort = 80;

void WiFiSetup(){
    // Inicia el modo de punto de acceso
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    // Imprime la dirección IP asignada al ESP8266 en el modo AP
    Serial.println("Dirección IP del servidor AP: " + WiFi.softAPIP().toString());
}

void WiFiSend(String data) {
  WiFiClient client;

  // Intenta conectarse al servidor Node.js
  if (client.connect(serverAddress, serverPort)) {
    // Construye la solicitud POST
    String request = "POST /data HTTP/1.1\r\n";
    request += "Host: " + String(serverAddress) + "\r\n";
    request += "Content-Type: application/x-www-form-urlencoded\r\n";
    request += "Content-Length: " + String(data.length()) + "\r\n";
    request += "\r\n";
    request += data;

    // Envía la solicitud POST al servidor
    client.print(request);

    Serial.println("Solicitud POST enviada");

    // Cierra la conexión
    client.stop();
  } else {
    Serial.println("Error al conectar al servidor");
  }
}
