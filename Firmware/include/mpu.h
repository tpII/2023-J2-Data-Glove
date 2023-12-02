#ifndef MPU
#define MPU
#include <Adafruit_MPU6050.h>

#define ROTARY_ANGLE_SENSOR A0

void MPUSetup(void);
String MPURead(void);
#endif