#ifndef WIFI
#define WIFI
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//Firma para la configuración del WiFi
void WiFiSetup(void);
//Firma para el envío de datos del WiFi
void WiFiSend(String);
#endif