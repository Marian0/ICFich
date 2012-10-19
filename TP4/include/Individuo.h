#ifndef __INDIVIDUO_H__
#define __INDIVIDUO_H__

#include <vector>


class Individuo {
    public:
    //Vector con los valores de la cadena de bits
    std::vector<bool> genotipo;

    //Valor decodificado del genotipo
    float fenotipo;

    unsigned int funcion_fitness_id;
    
    //Ultimo valor obtenido de la función de fitness
    float fitness; 

    //Constructor vacio
    Individuo();

    //Constructor
    Individuo(unsigned int cantidad_genes, unsigned int funcion_fitness_id);

    //Calcula el fitness actual y lo guarda en la propiedad fitness
    float calcularFitness();

    //Devuelve el fitness actual
    float getFitness();


};
#endif
