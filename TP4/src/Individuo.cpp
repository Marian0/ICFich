#include <vector>
#include "Individuo.h"

//Constructor
Individuo::Individuo(unsigned int cantidad_genes) {
    this->genotipo.resize(cantidad_genes,false);
}

//Devuelve el fitness actual
float Individuo::getFitness(){
    return this->fitness;
}


