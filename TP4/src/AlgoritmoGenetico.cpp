#include <algorithm>
#include "utils.h"
#include "AlgoritmoGenetico.h"
#include "Individuo.h"

AlgoritmoGenetico::AlgoritmoGenetico(   unsigned int tam_pob, unsigned int cant_genes,
                                        unsigned int max_gen, float pcruza, float pmutacion,
                                        unsigned int id_funcion_fitness,
                                        unsigned int met_sel, unsigned int k_competencia, int n_ventanas) {

}

//Realiza la selección de la poblacion, y guarda en nuevos_padres los Individuos elegidos, segun el metodo de seleccion definido, llama a Ruleta, Ventanas o Competencia
void AlgoritmoGenetico::seleccion(std::vector<Individuo> &nuevos_padres) {
    nuevos_padres.clear();
    for (int i = tamanio_poblacion; i > 0; i--) {
        switch(AlgoritmoGenetico::);
    }

}

//Metodo de la ruleta para la seleccion
void AlgoritmoGenetico::ruleta(std::vector<Individuo> &nuevos_padres) {

}

//Metodo de ventanas para la seleccion
void AlgoritmoGenetico::ventanas(std::vector<Individuo> &nuevos_padres){

}



//Metodo de competencias para la seleccion
void AlgoritmoGenetico::competencia(Individuo &nuevo_padre) {
    unsigned int npoblacion = this->poblacion.size();
    assert(npoblacion > 0);


    //Creo un vector de índices para referenciar a la poblacion
    std::vector<int> vector_id_poblacion;

    for (unsigned int i = 0; i < npoblacion; i++ ) {
        vector_id_poblacion.push_back(i);
    }

    //Mezclo el array de indices
    std::random_shuffle(vector_id_poblacion.begin(), vector_id_poblacion.end() );

    unsigned int id_max_fitness = 0;
    float max_fitness = this->poblacion[ vector_id_poblacion[0] ].getFitness();

    for (unsigned int i = 1; i < this->k_competencia ; i++ ) {
        if (i > npoblacion) {
            //El k de competencia es mayor que la poblacion
            break;
        }

        float fitness_i = this->poblacion[ vector_id_poblacion[i] ].getFitness();

        if (fitness_i > max_fitness) {
            max_fitness = fitness_i;
            id_max_fitness = i;
        }

    }

    nuevo_padre = this->poblacion[id_max_fitness];
}

//Realiza la cruza entre un padre y una madre, y guarda en hijos el resultado
void AlgoritmoGenetico::cruza(Individuo & padre, Individuo & madre, std::vector<Individuo> &hijos) {
    unsigned int posicion_cruza = abs(trunc(utils::randomDecimal(0, this->cantidad_genes )));

    Individuo hijo1( this->cantidad_genes, this->id_funcion_fitness );
    Individuo hijo2( this->cantidad_genes, this->id_funcion_fitness );
    hijo1.genotipo.clear();
    hijo2.genotipo.clear();

    hijo1.genotipo.insert(hijo1.genotipo.begin(), padre.genotipo.begin(), padre.genotipo.begin() + posicion_cruza);
    hijo2.genotipo.insert(hijo2.genotipo.begin(), madre.genotipo.begin(), madre.genotipo.begin() + posicion_cruza);

    hijo1.genotipo.insert(hijo1.genotipo.begin() + posicion_cruza, madre.genotipo.begin() + posicion_cruza, madre.genotipo.end() );
    hijo2.genotipo.insert(hijo2.genotipo.begin() + posicion_cruza, padre.genotipo.begin() + posicion_cruza, padre.genotipo.end() );

    hijos.clear();

    hijos.push_back(hijo1);
    hijos.push_back(hijo2);
}

//Realiza la mutación de un padre en un hijo
void AlgoritmoGenetico::mutacion(Individuo &individuo_a_mutar) {
    unsigned int i_random = abs(trunc( utils::randomDecimal(0, this->cantidad_genes) ));
    individuo_a_mutar.genotipo[i_random] = ! individuo_a_mutar.genotipo[i_random];
}
