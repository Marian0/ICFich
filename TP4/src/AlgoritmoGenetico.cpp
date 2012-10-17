#include "AlgoritmoGenetico.h"
#include "Individuo.h"

AlgoritmoGenetico::AlgoritmoGenetico(   unsigned int tam_pob, unsigned int cant_genes,
                                        unsigned int max_gen, float pcruza, float pmutacion,
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
