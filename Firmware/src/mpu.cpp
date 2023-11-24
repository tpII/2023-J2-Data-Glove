#include <mpu.h>
#include <filtro.h>
#include <string>

Adafruit_MPU6050 mpu;
extern float SEq_1, SEq_2, SEq_3, SEq_4;
String MPUdata;

void MPUSetup(){
    if (!mpu.begin()) {
        Serial.println("No se pudo encontrar un sensor MPU6050. Reconecte el HW.");
        while (1);
    }
    Serial.println("Sensor MPU6050 encontrado!");

    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}

String MPURead(){
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    filterUpdate(g.gyro.x, g.gyro.y, g.gyro.z, a.acceleration.x, a.acceleration.y, a.acceleration.z);
    std::string seq_1 = std::to_string(SEq_1);
    std::string seq_2 = std::to_string(SEq_2);
    std::string seq_3 = std::to_string(SEq_3);
    std::string seq_4 = std::to_string(SEq_4);
    std::string dataStd = "SEq_1=" + seq_1 + "&SEq_2=" + seq_2 + "&SEq_3=" + seq_3 + "&SEq_4=" + seq_4;
    MPUdata = String(dataStd.c_str());
    return MPUdata;
}
