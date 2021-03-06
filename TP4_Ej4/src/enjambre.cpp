#include "include/enjambre.h"
#include "particula.h"
#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>
#include "utils.h"

Enjambre::Enjambre(std::vector<float> limites_inf, std::vector<float> limites_sup,
                   unsigned int maxit, unsigned int cant_part, unsigned int id_funcion_fitness,
                   float c1, float c2, unsigned int tamanio_vecindario, Red perceptron,
                   std::vector<std::vector<float> > patrones_entradas, std::vector<std::vector<float> > patrones_salidas) {
    //Compruebo que sean coherentes los limites
    assert(limites_sup.size() == limites_inf.size());

    this->dimension = limites_inf.size();
    this->cantidad_particulas = cant_part;
    this->limites_inferiores = limites_inf;
    this->limites_superiores = limites_sup;
    this->c1 = c1;
    this->c2 = c2;
    this->tamanio_vecindario = tamanio_vecindario;
    this->iteraciones_maximas = maxit;
    this->cantidad_iteraciones = 0;
    this->id_funcion_fitness = id_funcion_fitness;

    //Seteo los datos para el perceptron
    this->perceptron = perceptron;
    this->patrones_entradas = patrones_entradas;
    this->patrones_salidas = patrones_salidas;

    //Construye las particulas
    for (unsigned int p = 0; p < this->cantidad_particulas; p++) {
        Particula particle (this->limites_inferiores, this->limites_superiores);
        this->particulas.push_back(particle);

        //Obtenemos la posicion de la particula
        std::vector<float> posicion = particle.getPosicion();
        //Actualizo el fitness de la particula p
        this->fitness_particulas.push_back(this->fitness(posicion));
    }

    this->mejores_posiciones.resize(this->cantidad_particulas);
    //Actualizar mejores
    this->actualizarMejoresPosiciones();

}

float Enjambre::iterar() {
    this->cantidad_iteraciones++;
    if (this->cantidad_iteraciones >= this->iteraciones_maximas) {
        std::cout<<"Maximo de iteraciones alcanzadas.\n";
        return 0.0;
    }


    //Para cada particula, actualizamos sus mejores
    for (unsigned int p = 0; p < this->cantidad_particulas; p++) {

        //Obtenemos la posicion de la particula
        std::vector<float> posicion = this->particulas[p].getPosicion();

        //Obtenemos el fitness de la posicion actual
        float fitness_p = this->fitness(posicion); //primero lo calculamos
        this->fitness_particulas[p] = fitness_p;  //luego lo guardamos en el vector de fitness de todas

        //Calculamos el fitness de la posicion mejor de la particula personal
        //Obtenemos la mejor posicion de la particula
        /*
        std::vector<float> posicionMejor = this->particulas[p].getMejorPosicionPersonal();
        float fitness_p_y = this->fitness(posicionMejor);
        */
        float fitness_p_y = this->particulas[p].getMejorFitness();

        //Si la posicion actual es mejor que la mejor historica
        if (fitness_p  < fitness_p_y) {
            this->particulas[p].setMejorPosicionPersonal(posicion);
            this->particulas[p].setMejorFitness(fitness_p);
        }

        /*--------Esto es inutil, nunca se usa y se actualiza despues de setear las nuevas posiciones
        //Calculamos el fitness de la mejor posicion de su vecindario
        //Obtenemos la mejor posicion del vecindario de la particula
        unsigned int id_mejor_particula_p = this->mejores_posiciones[p];
        float fitness_p_yhat = this->fitness_particulas[id_mejor_particula_p];

        //Si la posicion actual es mejor que la mejor del vecindario
        //
        if (fitness_p_y < fitness_p_yhat)
            this->mejores_posiciones[p] = p;
            //this->particulas[p].setMejorVecindario(p);
        */
    }

    //Ahora actualizamos las posiciones de cada particula
    for (unsigned int p = 0; p < this->cantidad_particulas; p++) {
        unsigned int id_mejor_particula_p = this->mejores_posiciones[p];

        //Obtenemos la mejor posicion de su vecindario
        std::vector<float> posicionMejorVecindario = this->particulas[id_mejor_particula_p].getPosicion();

        //Actualizamos sus coordenadas
        this->particulas[p].actualizarPosicion(posicionMejorVecindario, c1, c2);
    }

    //Luego actualizamos los mejores de cada vecindario
    this->actualizarMejoresPosiciones();

    //el error es el mejor fitness, lo devolvemos
    return this->getMejorFitness();
}

//Devuelve el mejor del vecindario de la particula dada por id_particula
std::vector<float> Enjambre::getMejorVecindario(unsigned int id_particula) {
    unsigned int particula_a_buscar = this->mejores_posiciones[id_particula];
    std::vector<float> ret_val = this->particulas[ particula_a_buscar ].getPosicion();
    return ret_val;
}

//Calcula el fitness para la posicion dada
float Enjambre::fitness(std::vector<float> posicion) {
    float nuevo_fitness = 0.0;

    //Seteo los pesos del perceptron como la posicion de la particula a evaluar
    this->perceptron.setPesos(posicion);

    //Capturo el error del perceptron
    float error = this->perceptron.train(this->patrones_entradas, this->patrones_salidas, false);
    if (fabs(error) < 0.00000001) {
        utils::printVector(posicion);
        getwchar();
    }

    //El nuevo fitness es el error cuadratico medio del entrenamiento
    nuevo_fitness = error;

    return nuevo_fitness;
}

//Para cada particula, setea el mejor de su vecindario
void Enjambre::actualizarMejoresPosiciones() {
    for (unsigned int p = 0; p < this->cantidad_particulas; p++) {

        //Seteamos como mejor a la particula en si misma
        unsigned int id_mejor_vecindario = p;
        float mejor_fitness = this->fitness_particulas[p];

        //Recorremos el vecindario
        //Magia, no tocar
        unsigned int inicio = (p + this->cantidad_particulas - this->tamanio_vecindario) % this->cantidad_particulas;
        unsigned int pasos_total = 2*this->tamanio_vecindario + 1;
        unsigned int paso_actual = 0;
        for (paso_actual = 0; paso_actual < pasos_total; paso_actual++) {
            unsigned int posicion = (inicio + paso_actual) % this->cantidad_particulas;

            if (this->fitness_particulas[posicion] < mejor_fitness) {
                mejor_fitness = this->fitness_particulas[posicion];
                id_mejor_vecindario = posicion;
            }
        }
        this->mejores_posiciones[p] = id_mejor_vecindario;
    }
}

std::vector<float> Enjambre::getSolucion() {
    unsigned int mejor_id = this->getMejorId();
    std::cout<<"Fitness de la solucion = "<<this->particulas[mejor_id].getMejorFitness()<<'\n';
    std::vector<float> ret_val = this->particulas[mejor_id].getPosicion();
    return ret_val;
}

unsigned int Enjambre::getMejorId() {
    unsigned int mejor_id = 0;
    float mejor = this->fitness_particulas[0];
    for (unsigned int p = 1; p < this->cantidad_particulas; p++) {
        if (this->fitness_particulas[p] < mejor) {
            mejor = this->fitness_particulas[p];
            mejor_id = p;
        }
    }
    return mejor_id;
}

float Enjambre::getMejorFitness() {
    unsigned int mejor_id = this->getMejorId();
    return this->fitness_particulas[mejor_id];
}
