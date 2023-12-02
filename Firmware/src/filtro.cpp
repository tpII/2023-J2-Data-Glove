// Libreia Math requerida para la funcion 'sqrt'
#include <math.h>
#include <filtro.h>

// Variables globales
volatile float SEq_1 = 1.0f, SEq_2 = 0.0f, SEq_3 = 0.0f, SEq_4 = 0.0f;
float a_x, a_y, a_z; // Medidas del acelerometro
float w_x, w_y, w_z; // Medidas del giroscopio en rad/s

void filterUpdate(float w_x, float w_y, float w_z, float a_x, float a_y, float a_z)
{
// Variables locales
float norm; // Vector normal
// Quaternion derivado de las medidas del giroscopio
float SEqDot_omega_1, SEqDot_omega_2, SEqDot_omega_3, SEqDot_omega_4; 
// Elementos de la funcion objetivo
float f_1, f_2, f_3; 
// Elementos del Jacobiano
float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33; 
// Direccion estimada del error del giroscopio
float SEqHatDot_1, SEqHatDot_2, SEqHatDot_3, SEqHatDot_4; 
// Variables auxiliares para evitar calculos repetidos
float halfSEq_1 = 0.5f * SEq_1;
float halfSEq_2 = 0.5f * SEq_2;
float halfSEq_3 = 0.5f * SEq_3;
float halfSEq_4 = 0.5f * SEq_4;
float twoSEq_1 = 2.0f * SEq_1;
float twoSEq_2 = 2.0f * SEq_2;
float twoSEq_3 = 2.0f * SEq_3;

// Normalizamos las medidas del acelerometro
norm = sqrt(a_x * a_x + a_y * a_y + a_z * a_z);
a_x /= norm;
a_y /= norm;
a_z /= norm;

// Calculamos la funcion objetivo (que debe minimizarse) con el Jacobiano
f_1 = twoSEq_2 * SEq_4 - twoSEq_1 * SEq_3 - a_x;
f_2 = twoSEq_1 * SEq_2 + twoSEq_3 * SEq_4 - a_y;
f_3 = 1.0f - twoSEq_2 * SEq_2 - twoSEq_3 * SEq_3 - a_z;
J_11or24 = twoSEq_3; // J_11 negado en la matriz de multiplicacion
J_12or23 = 2.0f * SEq_4;
J_13or22 = twoSEq_1; // J_12 negado en la matriz de multiplicacion
J_14or21 = twoSEq_2;
J_32 = 2.0f * J_14or21; // Negado en la matriz de multiplicacion
J_33 = 2.0f * J_11or24; // Negado en la matriz de multiplicacion

// Calculamos el gradiente de la matriz de multiplicacion
SEqHatDot_1 = J_14or21 * f_2 - J_11or24 * f_1;
SEqHatDot_2 = J_12or23 * f_1 + J_13or22 * f_2 - J_32 * f_3;
SEqHatDot_3 = J_12or23 * f_2 - J_33 * f_3 - J_13or22 * f_1;
SEqHatDot_4 = J_14or21 * f_1 + J_11or24 * f_2;

// Normalizamos el gradiente
norm = sqrt(SEqHatDot_1 * SEqHatDot_1 + SEqHatDot_2 * SEqHatDot_2 + SEqHatDot_3 * SEqHatDot_3 + SEqHatDot_4 * SEqHatDot_4);
SEqHatDot_1 /= norm;
SEqHatDot_2 /= norm;
SEqHatDot_3 /= norm;
SEqHatDot_4 /= norm;

// Calculamos el quaternion derivado de las medidas del giroscopio
SEqDot_omega_1 = -halfSEq_2 * w_x - halfSEq_3 * w_y - halfSEq_4 * w_z;
SEqDot_omega_2 = halfSEq_1 * w_x + halfSEq_3 * w_z - halfSEq_4 * w_y;
SEqDot_omega_3 = halfSEq_1 * w_y - halfSEq_2 * w_z + halfSEq_4 * w_x;
SEqDot_omega_4 = halfSEq_1 * w_z + halfSEq_2 * w_y - halfSEq_3 * w_x;

// Calculamos y luego integramos el quaternion derivado estimado
SEq_1 += (SEqDot_omega_1 - (beta * SEqHatDot_1)) * deltat;
SEq_2 += (SEqDot_omega_2 - (beta * SEqHatDot_2)) * deltat;
SEq_3 += (SEqDot_omega_3 - (beta * SEqHatDot_3)) * deltat;
SEq_4 += (SEqDot_omega_4 - (beta * SEqHatDot_4)) * deltat;

// Normalizamos el quaternion
norm = sqrt(SEq_1 * SEq_1 + SEq_2 * SEq_2 + SEq_3 * SEq_3 + SEq_4 * SEq_4);
SEq_1 /= norm;
SEq_2 /= norm;
SEq_3 /= norm;
SEq_4 /= norm;
}