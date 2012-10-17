#include <vector>
#include "Individuo.h"

//Constructor
Individuo::Individuo(unsigned int cantidad_genes, unsigned int funcion_fitness_id) {
    this->genotipo.resize(cantidad_genes,false);
    this->funcion_fitness_id = funcion_fitness_id;
}

//Devuelve el fitness actual
float Individuo::getFitness(){
    switch(this->funcion_fitness_id){

    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        return 1.1;
    }
}


