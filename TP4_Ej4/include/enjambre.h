#ifndef ENJAMBRE_H
#define ENJAMBRE_H
#include <vector>
#include "particula.h"
#include "Red.h"

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

    unsigned int cantidad_iteraciones;
    unsigned int iteraciones_maximas;

    //Id del problema
    unsigned int id_funcion_fitness;

    //Perceptron Multi Capa
    Red perceptron;
    std::vector<std::vector<float> > patrones_entradas;
    std::vector<std::vector<float> > patrones_salidas;

public:

    Enjambre(std::vector<float> limites_inf, std::vector<float> limites_sup,
             unsigned int maxit, unsigned int cant_part, unsigned int id_funcion_fitness,
             float c1, float c2, unsigned int tamanio_vecindario, Red perceptron,
             std::vector<std::vector<float> > patrones_entradas, std::vector<std::vector<float> > patrones_salidas);
    void iterar();
    std::vector<float> getMejorVecindario(unsigned int id_particula);
    float fitness(std::vector<float> posicion);

    void actualizarMejoresPosiciones();
    std::vector<float> getSolucion();
    unsigned int getMejorId();
    float getMejorFitness();

};

#endif // ENJAMBRE_H
