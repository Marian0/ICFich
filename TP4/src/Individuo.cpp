#include <vector>
#include "Individuo.h"
#include "utils.h"

//Constructor vacio
Individuo::Individuo() { }

//Constructor
Individuo::Individuo(unsigned int cantidad_genes, unsigned int funcion_fitness_id) {
    this->funcion_fitness_id = funcion_fitness_id;
    //Relleno el individuo con contenido al azar
    for (unsigned int i = 0; i < cantidad_genes; i++) {
        //Genero un numero aleatorio
        bool gen = rand() % 2;

        //Asigno
        this->genotipo.push_back(gen);
    }
}

//Devuelve el fitness actual
float Individuo::getFitness(){

    return this->fitness;
}

//Calcula el fitness de este individuo
float Individio::calcularFitness() {
    float nuevo_fitness;
    /*
    switch(this->funcion_fitness_id){

    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        return 1.1;
    }*/

    this->fitness = nuevo_fitness;
    return nuevo_fitness;
}


