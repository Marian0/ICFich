#include "include/enjambre.h"
#include "Particula.h"
#include <vector>
#include <assert>


Enjambre::Enjambre(std::vector<float> limites_inf, std::vector<float> limites_sup,
                   unsigned int cant_part,
                   float c1, float c2, unsigned int tamanio_vecindario) {
    //Compruebo que sean coherentes los limites
    assert(limites_sup.size() == limites_inf.size());

    this->dimension = limites_inf.size();
    this->cantidad_particulas = cant_part;
    this->limites_inferiores = limites_inf;
    this->limites_superiores = limites_sup;
    this->c1 = c1;
    this->c2 = c2;
    this->tamanio_vecindario = tamanio_vecindario;

    //Construye las particulas
    for (unsigned int p = 0; p < this->cantidad_particulas; p++) {
        Particula particle (this->limites_inferiores, this->limites_superiores);
        this->particulas.push_back(particle);

        //Obtenemos la posicion de la particula
        std::vector<float> posicion = particle.getPosicion();
        //Actualizo el fitness de la particula p
        this->fitness_particulas.push_back(this->fitness(posicion));
    }

    //Actualizar mejores
    this->actualizarMejoresPosiciones();


    for (unsigned int i = 0; i < this->cantidad_particulas; i++) {
        this->particulas[i].setMejorVecindario(this->mejores_posiciones[i]);
    }

}

void Enjambre::iterar(unsigned int maxit) {
    unsigned int i;
    //Recorremos hasta maxit
    for (i = 0; i < maxit; i++) {
        //Para cada particula, actualizamos sus mejores
        for (unsigned int p = 0; p < this->cantidad_particulas; p++) {

            //Obtenemos la posicion de la particula
            std::vector<float> posicion = this->particulas[id_particula].getPosicion();
            //Obtenemos la mejor posicion de la particula
            std::vector<float> posicionMejor = this->particulas[id_particula].getMejorPosicion();

            //Obtenemos la mejor posicion del vecindario de la particula
            unsigned int id_mejor_particula_p = this->mejores_posiciones[p];


            //Calculamos los fitness para cada una de las posiciones
            //Obtenemos el fitness de la posicion actual
            float fitness_p = this->fitness_particulas[p];

            //Calculamos el fitness de la posicion mejor de la particula personal
            float fitness_p_y = this->fitness(posicionMejor);

            //Calculamos el fitness de la mejor posicion de su vecindario
            float fitness_p_yhat = this->fitness_particulas[id_mejor_particula_p];

            //Si la posicion actual es mejor que la mejor historica
            if (fitness_p  < fitness_p_y)
                this->particulas[p].setMejorPosicion(posicion);

            //Si la posicion actual es mejor que la mejor del vecindario
            if (fitness_p_y < fitness_p_yhat)
                this->particulas[p].setMejorPosicionVecindario(posicion);
        }

        //Ahora actualizamos las posiciones de cada particula
        for (unsigned int p = 0; p < this->cantidad_particulas; p++) {

            //Obtenemos la mejor posicion de su vecindario
            std::vector<float> posicionMejorVecindario = this->particulas[id_mejor_particula_p].getPosicion();

            //Actualizamos sus coordenadas
            this->particulas[p].actualizarPosicion(posicionMejorVecindario, c1, c2);
        }

        //Calculamos los nuevos fitness
        for (unsigned int p = 0; p < this->cantidad_particulas; p++) {
            //Obtenemos la posicion de la particula
            std::vector<float> posicion = this->particulas[id_particula].getPosicion();
            //Actualizo el fitness de la particula p
            this->fitness_particulas[p] = this->fitness(posicion);
        }

        //Luego actualizamos los mejores de cada vecindario
        this->actualizarMejoresPosiciones();
    }
}

//Devuelve el mejor del vecindario de la particula dada por id_particula
std::vector<float> Enjambre::getMejorVecindario(unsigned int id_particula) {
    unsigned int particula_a_buscar = this->mejores_posiciones[id_particula];
    std::vector<float> ret_val = this->particulas[ particula_a_buscar ].getPosicion();
    return ret_val;
}

//Calcula el fitness para la posicion dada
float Enjambre::fitness(std::vector<float> posicion) {
    return 1.0;
}

//Para cada particula, setea el mejor de su vecindario
void Enjambre::actualizarMejoresPosiciones() {
    for (unsigned int p = 0; p < this->cantidad_particulas; p++) {

        //Seteamos como mejor a la particula en si misma
        unsigned int id_mejor_vecindario = p;
        float mejor_fitness = this->fitness_particulas[p];

        //Recorremos el vecindario
        //Magia, no tocar
        unsigned int inicio = (p - this->tamanio_vecindario) % this->cantidad_particulas;
        unsigned int pasos_total = 2*this->tamanio_vecindario + 1;
        unsigned int paso_actual = 0;
        for (paso_actual = 0; i < pasos_total; paso_actual++) {
            //tener Fe en esto
            unsigned int posicion = (inicio + paso_actual) % this->cantidad_particulas;

            if (this->fitness_particulas[posicion] < mejor_fitness) {
                mejor_fitness = this->fitness_particulas[posicion];
                id_mejor_vecindario = posicion;
            }
        }
        this->mejores_posiciones[p] = id_mejor_vecindario;
    }
}
