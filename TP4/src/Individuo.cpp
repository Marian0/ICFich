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
    //Relleno el individuo con contenido al azar
    for (unsigned int i = 0; i < cantidad_genes; i++) {
        //Genero un numero aleatorio
        bool gen = rand() % 2;

        //Asigno
        this->genotipo.push_back(gen);
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

    if (this->variables_fenotipo == 1) { //si tengo una sola variable en el fenotipo
        utils::printVector(this->genotipo);
        getwchar();
        int fenotipo;
        float f_valor;
        switch(this->funcion_fitness_id){
            case 1: {
                fenotipo = utils::binary2int(this->genotipo, true);
                f_valor = fenotipo/escala;

                nuevo_fitness = utils::fitness_1a(f_valor);
                break;
            }
            case 2: {
                fenotipo = utils::binary2int(this->genotipo, false);
                f_valor = fenotipo/escala;

                if (f_valor > 20.0) //individuo invalido, lo desecho
                    nuevo_fitness = 0.0;
                else
                    nuevo_fitness = utils::fitness_1b(f_valor);
                break;
            }
        }
    }
    //ejercicio 1c
    else if (this->variables_fenotipo == 2) {
        unsigned int g_size = this->genotipo.size();
        //Divido el genotipo en dos
        std::vector<bool> v1;
        std::vector<bool> v2;
        v1.assign(this->genotipo.begin(), this->genotipo.begin()+floor(g_size/2.0));
        v2.assign(this->genotipo.begin()+ceil(g_size/2.0), this->genotipo.end());

        //Decodifico cada una de las variables
        float f_1 = utils::binary2int(v1, true)/this->escala;
        float f_2 = utils::binary2int(v2, true)/this->escala;

        //Desecho el individuo si es invalido
        if (f_1 > 100 or f_2 > 100 or f_1 < -100 or f_2 < -100)
            nuevo_fitness = 0.0;
        else //recalculo el fitness
            nuevo_fitness = utils::fitness_1c(f_1, f_2);
    }

    //caso del agente viajero
    else if (this->variables_fenotipo > 2) {
        std::vector<int> recorrido;
        //Obtenemos los valores enteros
        //unsigned int paso = (unsigned int) (float(this->genotipo.size()) / float(this->variables_fenotipo));
        utils::vectorBinary2Int(this->genotipo, recorrido, 4);

        //Corroboramos que sea una solución válida

        //la validez tiene el efecto de escalar el resultado de la
        //   distancia para penalizar a los individuos que tienen repeticiones
        float validez = this->agente_viajero.solucionValida(recorrido);

        float distanciaRecorrida = this->agente_viajero.calcularRecorrido(recorrido);

        //std::cout<<"Distancia Recorrida = "<<distanciaRecorrida<<'\n';

        if (fabs(distanciaRecorrida) < 0.000001) { //si no se mueve
            nuevo_fitness = 0.0;
        } else {
            if (validez < 1.0) {
                float distanciaPenalizada = distanciaRecorrida + 10*distanciaRecorrida * (1.0-validez);
                //nuevo_fitness = 1/(distanciaPenalizada);
                nuevo_fitness = 0.0;
            } else {
                nuevo_fitness = 1/distanciaRecorrida;
            }
        }
    }

    this->fitness = nuevo_fitness;
    return nuevo_fitness;
}

float Individuo::getFenotipo() {
    int valor = utils::binary2int(this->genotipo, true);
    return valor/escala;
}
