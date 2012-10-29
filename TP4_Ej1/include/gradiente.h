#ifndef GRADIENTE_H
#define GRADIENTE_H
#include <vector>
class Gradiente {
    std::vector<float> x;
    unsigned int id_ejercicio;
    float tasa_aprendizaje;
    float criterio_corte;
    unsigned int iteraciones_maximas;
public:
    Gradiente(float tasa, std::vector<float> x_ini, unsigned int id_ejercicio, float criterio_corte, unsigned int maxit);
    unsigned int descender();

    std::vector<float> getSolucion();

    std::vector<float> evaluarGradiente(std::vector<float> &X);
};

#endif // GRADIENTE_H
