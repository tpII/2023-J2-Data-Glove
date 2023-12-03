
#ifndef FILTROINCLUIDO
#define FILTROINCLUIDO

// constantes
#define deltat 0.02f // tiempo de muestra 20ms
#define gyroMeasError 3.14159265358979f * (5.0f / 180.0f) // error del giroscopio en rad/s
#define beta sqrt(3.0f / 4.0f) * gyroMeasError * 2.0f // parámetro de ajuste que controla la velocidad de convergencia del filtro

void filterUpdate(float w_x, float w_y, float w_z, float a_x, float a_y, float a_z);
#endif