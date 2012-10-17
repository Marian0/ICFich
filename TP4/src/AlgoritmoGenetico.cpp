#include <algorithm>
#include "utils.h"
#include "AlgoritmoGenetico.h"
#include "Individuo.h"
#include <cstdio>

AlgoritmoGenetico::AlgoritmoGenetico(   unsigned int tam_pob, unsigned int cant_genes,
                                        unsigned int max_gen, float pcruza, float pmutacion,
                                        unsigned int id_funcion_fitness,
                                        unsigned int met_sel, unsigned int k_competencia, int n_ventanas) {

    //Copia las propiedades del algoritmo
    this->tamanio_poblacion = tam_pob;
    this->cantidad_genes = cant_genes;
    this->generaciones_maximo = max_gen;
    this->probabilidad_cruza = pcruza;
    this->probabilidad_mutacion = pmutacion;
    this->metodo_seleccion = met_sel;
    this->k_competencia = k_competencia;
    this->n_ventanas = n_ventanas;
    this->id_funcion_fitness = id_funcion_fitness;

    //Crea todos los Individuos
    for (unsigned int i = 0; i < this->tamanio_poblacion; i++) {
        Individuo new_ind(this->cantidad_genes, this->id_funcion_fitness);
        this->poblacion.push_back(new_ind);
    }

}

//Crea la nueva generacion
void AlgoritmoGenetico::reproduccion() {
    std::vector<Individuo> nueva_poblacion;

    //codigo aca


    this->poblacion = nueva_poblacion;
}

//Realiza la selección de la poblacion, y guarda en nuevos_padres los Individuos elegidos.
//Segun el metodo de seleccion definido, llama a Ruleta, Ventanas o Competencia
void AlgoritmoGenetico::seleccion(std::vector<Individuo> &nuevos_padres) {
    nuevos_padres.clear();


    for (int i = tamanio_poblacion; i > 0; ) { //notar que i no se actualiza, se decrementa dentro de cada case

        switch(this->metodo_seleccion) {
        case SELECCION_RULETA: {
            Individuo progenitor;
            ruleta(progenitor);
            nuevos_padres.push_back(progenitor);
            //Actualizo i, se agrego un solo progenitor nuevo
            i--;
            break;
        }
        case SELECCION_VENTANAS: {
            std::vector<Individuo> progenitores;
            seleccion(progenitores);
            nuevos_padres.insert(nuevos_padres.end(), progenitores.begin(), progenitores.end());
            //Actualizo i, se agregaron tantos padres nuevos como hay en progenitores
            i = i-progenitores.size();
            break;
        }
        case SELECCION_COMPETENCIA: {
            Individuo progenitor;
            competencia(progenitor);
            nuevos_padres.push_back(progenitor);
            //Actualizo i, se agrego un solo progenitor nuevo
            i--;
            break;
        }
        }
    }

}

//Metodo de la ruleta para la seleccion
void AlgoritmoGenetico::ruleta(Individuo &nuevo_padre) {
    //Calculo la suma de fitness total
    float sum_fitness = 0.0;
    for (unsigned int i = 0; i < this->tamanio_poblacion; i++) {
        sum_fitness += this->poblacion[i].getFitness();
    }

    //Calculo la probabilidad de cada individuo
    std::vector<float> probabilidades_individuos;
    for (unsigned int i = 0; i < this->tamanio_poblacion; i++) {
        float p_i = this->poblacion[i].getFitness() / sum_fitness;
        probabilidades_individuos.push_back(p_i);
    }

    //Calculo la probabilidad acumulada de cada individuo
    std::vector<float> probabilidades_acumuladas;
    float acum = 0.0;
    for (unsigned int i = 0; i < this->tamanio_poblacion; i++) {
        acum += probabilidades_individuos[i]; //acumulo
        float q_i = acum;
        probabilidades_acumuladas.push_back(q_i);
    }

    //Genero la posicion en la ruleta
    float posicion = utils::randomDecimal(0.0, 1.0);
    unsigned int buscador = 0;
    while ((probabilidades_acumuladas[buscador+1] < posicion) and (buscador < probabilidades_acumuladas.size()-1))
        buscador++;

    nuevo_padre = this->poblacion[buscador];

}

//Metodo de ventanas para la seleccion
void AlgoritmoGenetico::ventanas(std::vector<Individuo> &nuevos_padres) {
    nuevos_padres.clear();
    std::vector<Individuo> poblacion_ordenada = this->poblacion;

    //Ordena los individuos de mayor a menor fitness
    std::sort(poblacion_ordenada.begin(), poblacion_ordenada.end(), AlgoritmoGenetico::ordenarIndividuos);

    unsigned int n_ventanas_efectivo;
    if (n_ventanas == -1)
        n_ventanas_efectivo = this->tamanio_poblacion;
    else
        n_ventanas_efectivo = this->n_ventanas;

    for (unsigned int i = 0; i < n_ventanas_efectivo; i++) {
        std::vector<Individuo> poblacion_ventaneada;
        //Copio una ventana de la poblacion
        poblacion_ventaneada.assign(poblacion_ordenada.begin(), poblacion_ordenada.end()-i);

        //Genero un indice al azar
        int idx = rand() % poblacion_ventaneada.size();

        //Inserto el nuevo progenitor
        nuevos_padres.push_back(poblacion_ventaneada[idx]);
    }
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