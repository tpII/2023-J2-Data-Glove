#include <mpu.h>
#include <filtro.h>
#include <string>

// Creamos un objeto para interactuar con el MPU
Adafruit_MPU6050 mpu;

// Definimos las variables que modifica el archivo filtro.cpp
extern float SEq_1, SEq_2, SEq_3, SEq_4;

// Definimos un String para devoler las medidas del MPU
String MPUdata;

void MPUSetup(){
    // Inicializamos la comunicacion con el MPU
    if (!mpu.begin()) {
        Serial.println("No se pudo encontrar un sensor MPU6050. Reconecte el HW.");
        while (1);
    }
    Serial.println("Sensor MPU6050 encontrado!");

    // Seteamos los rangos que utilizaremos para las mediciones
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}

String MPURead(){
    // Definimos los eventos o lecturas, de aceleracion, giro y temperatura
    sensors_event_t a, g, temp;

    // Le indicamos al MPU que lea los datos y nos devuelve las medidas
    mpu.getEvent(&a, &g, &temp);

    // Los datos en crudo obtenidos los enviamos al filtro
    filterUpdate(g.gyro.x, g.gyro.y, g.gyro.z, a.acceleration.x, a.acceleration.y, a.acceleration.z);
    std::string seq_1 = std::to_string(SEq_1);
    std::string seq_2 = std::to_string(SEq_2);
    std::string seq_3 = std::to_string(SEq_3);
    std::string seq_4 = std::to_string(SEq_4);

    // Construimos el mensaje que devolveremos para enviar al WebServer
    std::string dataStd = "SEq_1=" + seq_1 + "&SEq_2=" + seq_2 + "&SEq_3=" + seq_3 + "&SEq_4=" + seq_4;
    MPUdata = String(dataStd.c_str());
    
    return MPUdata;
}
