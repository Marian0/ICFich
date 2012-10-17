#include <vector>
#include "Individuo.h"

//Constructor
Individuo::Individuo(unsigned int cantidad_genes) {
    //Relleno el individuo con contenido al azar
    for (unsigned int i = 0; i < cantidad_genes; i++) {
        //Genero un numero aleatorio
        float value = utils::randomDecimal(0.0, 1.0);
        bool gen;
        //Si mayor a 0.5, es true, si no, false
        if (value > 0.5)
            gen = true;
        else
            gen = false;
        //Asigno
        this->genotipo.push_back(gen)
    }
}

//Devuelve el fitness actual
float Individuo::getFitness(){
    return this->fitness;
}


