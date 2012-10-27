#ifndef PARTICULA_H
#define PARTICULA_H

#include <vector>

class Particula
{
    //Posicion y velocidad de la particula actual
    std::vector<float> posicion;
    std::vector<float> velocidad;
    unsigned int dimension;
    unsigned int id_mejor_posicion_entorno;

    //Mejores posiciones
    std::vector<float> mejor_posicion_personal;

    //Limites dimensionales
    std::vector<float> limite_inferior;
    std::vector<float> limite_superior;

public:
    //Constructor
    Particula(std::vector<float> limites_inferior, std::vector<float> limites_superior);

    //Actualiza la posicion de la partícula en base al cálculo de su velocidad
    void actualizarPosicion(std::vector<float> mejor_posicion_entorno,
                  float c1, float c2 );


    void setMejorPosicionPersonal(std::vector<float> mejor_posicion_personal );
    std::vector<float> getMejorPosicionPersonal();

    std::vector<float> getPosicion();
};

#endif // PARTICULA_H
