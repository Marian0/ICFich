#ifndef __INDIVIDUO_H__
#define __INDIVIDUO_H__

#include <vector>


class Individuo {
    public:
    //Vector con los valores de la cadena de bits
    std::vector<bool> genotipo;

    //Especifica el factor de division del fenotipo
    float escala;

    //Id de la funcion de fitness a usar
    unsigned int funcion_fitness_id;
    
    //Ultimo valor obtenido de la función de fitness
    float fitness; 

    //Constructor vacio
    Individuo();

    //Constructor
    Individuo(unsigned int cantidad_genes, unsigned int funcion_fitness_id, float escala = 1.00);

    //Calcula el fitness actual y lo guarda en la propiedad fitness
    float calcularFitness();

    //Devuelve el fitness actual
    float getFitness();

    //Devuelve el Fenotipo del Individuo
    float getFenotipo();


};
#endif
