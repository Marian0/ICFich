#ifndef ENJAMBRE_H
#define ENJAMBRE_H
#include <vector>
#include "Particula.h"

class Enjambre {
    //Dimension del problema
    unsigned int dimension;

    //Cantidad de particulas en el enjambre
    unsigned int cantidad_particulas;

    //Vector que almacena todas las particulas
    std::vector<Particula> particulas;

    //Limites del problema
    std::vector<float> limites_inferiores;
    std::vector<float> limites_superiores;

    //Constantes de aceleración
    float c1;
    float c2;

    //Tamano del vecindario tipo anillo
    unsigned int tamanio_vecindario;

    //Fitness de cada una de las particulas
    std::vector<float> fitness_particulas;

    //Vector que almacena los id de mejores posiciones para cada particula
    std::vector<unsigned int> mejores_posiciones;

public:
    Enjambre();
    void iterar(unsigned int maxit);
    std::vector<float> getMejorVecindario(unsigned int id_particula);
    float fitness(unsigned int id_particula);

    void actualizarMejoresPosiciones();

};

#endif // ENJAMBRE_H
