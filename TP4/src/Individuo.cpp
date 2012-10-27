#include <vector>
#include "Individuo.h"
#include "utils.h"

//Constructor vacio
Individuo::Individuo() { }

//Constructor
Individuo::Individuo(unsigned int cantidad_genes, unsigned int funcion_fitness_id, float escala, unsigned int variables_fenotipo) {
    this->funcion_fitness_id = funcion_fitness_id;
    this->escala = escala;
    this->variables_fenotipo = variables_fenotipo;

    for (unsigned int i = 0; i < variables_fenotipo; i++) {
        unsigned int id_nueva_ciudad = rand() % this->variables_fenotipo;
        std::vector<bool> id_nueva_ciudad_binary = utils::int2binary(id_nueva_ciudad, false);
        while (id_nueva_ciudad_binary.size() < 4 ) {
            id_nueva_ciudad_binary.insert(id_nueva_ciudad_binary.begin(),0);
        }
        this->genotipo.insert(this->genotipo.end(), id_nueva_ciudad_binary.begin(), id_nueva_ciudad_binary.end() );
    }


    //this->calcularFitness();
}

//Devuelve el fitness actual
float Individuo::getFitness(){
    return this->fitness;
}

//Calcula el fitness de este individuo
float Individuo::calcularFitness() {
    float nuevo_fitness = 0.0;

    std::vector<int> recorrido;
    //Obtenemos los valores enteros
    //unsigned int paso = (unsigned int) (float(this->genotipo.size()) / float(this->variables_fenotipo));
    utils::vectorBinary2Int(this->genotipo, recorrido, 4);

    //Corroboramos que sea una solución válida

    //la validez tiene el efecto de escalar el resultado de la
    //   distancia para penalizar a los individuos que tienen repeticiones
    float validez = this->agente_viajero.solucionValida(recorrido);

    this->distanciaRecorrida = this->agente_viajero.calcularRecorrido(recorrido);

    //std::cout<<"Distancia Recorrida = "<<distanciaRecorrida<<'\n';

    if (fabs(distanciaRecorrida) < 0.000001) { //si no se mueve
        nuevo_fitness = 0.0;
    } else {
        nuevo_fitness = validez/this->distanciaRecorrida;
    }


    this->fitness = nuevo_fitness;
    return nuevo_fitness;
}

float Individuo::getFenotipo() {
    int valor = utils::binary2int(this->genotipo, true);
    return valor/escala;
}
