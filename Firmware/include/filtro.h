
#ifndef FILTROINCLUIDO
#define FILTROINCLUIDO

// System constants
#define deltat 0.02f // sampling period in seconds (shown as 1 ms)
#define gyroMeasError 3.14159265358979f * (5.0f / 180.0f) // gyroscope measurement error in rad/s (shown as 5 deg/s)
#define beta sqrt(3.0f / 4.0f) * gyroMeasError // compute beta

// estimated orientation quaternion elements with initial conditions (A checkear)

void filterUpdate(float w_x, float w_y, float w_z, float a_x, float a_y, float a_z);
#endif