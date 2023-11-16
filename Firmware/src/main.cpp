#include <Adafruit_MPU6050.h>
#include <filtro.h>
#include <string>

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

const char *ssid = "DataGlove";
const char *password = "laTercera";

Adafruit_MPU6050 mpu;
const char *serverAddress = "192.168.4.7";  // Reemplaza con la dirección IP de tu servidor Node.js
const int serverPort = 80;
extern float SEq_1, SEq_2, SEq_3, SEq_4;
String data;

void sendData(String);

void setup() {
  Serial.begin(115200);

  // Inicia el modo de punto de acceso
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Imprime la dirección IP asignada al ESP8266 en el modo AP
  Serial.println("Dirección IP del servidor AP: " + WiFi.softAPIP().toString());

  Serial.println("Fin del programa");

  if (!mpu.begin()) {
    Serial.println("No se pudo encontrar un sensor MPU6050.");
    while (1);
  }
  Serial.println("Sensor MPU6050 encontrado!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}

void loop() {
  // Maneja las solicitudes del servidor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  filterUpdate(g.gyro.x, g.gyro.y, g.gyro.z, a.acceleration.x, a.acceleration.y, a.acceleration.z);
  std::string seq_1 = std::to_string(SEq_1);
  std::string seq_2 = std::to_string(SEq_2);
  std::string seq_3 = std::to_string(SEq_3);
  std::string seq_4 = std::to_string(SEq_4);
  std::string dataStd = "SEq_1=" + seq_1 + "&SEq_2=" + seq_2 + "&SEq_3=" + seq_3 + "&SEq_4=" + seq_4;
  data = String(dataStd.c_str());
  sendData(data);
  delay(25);
}

void sendData(String data) {
  // Crea una instancia de WiFiClient
  WiFiClient client;

  // Intenta conectarse al servidor Node.js
  if (client.connect(serverAddress, serverPort)) {
    // Cuerpo de la solicitud POST (puedes ajustar esto según tus necesidades)

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

/*
    // Espera la respuesta del servidor
    while (client.connected() && !client.available()) {
      delay(10);
    }

    // Lee y muestra la respuesta del servidor
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    */

    // Cierra la conexión
    client.stop();
  } else {
    Serial.println("Error al conectar al servidor");
  }
}

