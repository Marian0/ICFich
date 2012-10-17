#ifndef __ALGORITMOGENETICO_H__
#define __ALGORITMOGENETICO_H__

#include <vector>
#include "Individuo.h"

class AlgoritmoGenetico {
    //Vector que tiene todos los Individuos de esta poblacion
    std::vector<Individuo> poblacion;
    
    //Define el tamaño máximo de población a manejar
    unsigned int tamanio_poblacion;

    //Define la cantidad de genes que tiene cada Individuo
    unsigned int cantidad_genes;

    //Cuenta la generacion en la que nos ubicamos
    unsigned int generacion_actual;
    //Maximo de generaciones a simular
    unsigned int generaciones_maximo;

    //Parametros para la reproduccion
    //Define los porcentajes para cada operacion
    float probabilidad_cruza;
    float probabilidad_mutacion;

    //Parametros para los algortimos de seleccion
    //Define la cantidad de padres a competir en competencia
    unsigned int k_competencia;
    //Define la cantidad de ventanas a usar en el metodo de ventanas. Si es -1 => se usa la misma cantidad de ventanas que el tamaño de la prox gen
    int n_ventanas;

    unsigned int metodo_seleccion;

    public:
    static const unsigned int SELECCION_RULETA = 1;
    static const unsigned int SELECCION_VENTANAS = 2;
    static const unsigned int SELECCION_COMPETENCIA = 3;
    
    //Constructor
    AlgoritmoGenetico(unsigned int tam_pob, unsigned int cant_genes, unsigned int max_gen, float pcruza, float pmutacion, unsigned int metodo_seleccion = SELECCION_COMPETENCIA , unsigned int k_comp = 5, int n_vent = -1);

    //Crea la nueva generacion
    void reproduccion();

    //Realiza la selección de la poblacion, y guarda en nuevos_padres los Individuos elegidos, segun el metodo de seleccion definido, llama a Ruleta, Ventanas o Competencia
    void seleccion(std::vector<Individuo> &nuevos_padres);
    
    //Metodo de la ruleta para la seleccion
    void ruleta(Individuo &nuevo_padre);
    
    //Metodo de ventanas para la seleccion
    void ventanas(std::vector<Individuo> &nuevos_padres);
    
    //Metodo de competencias para la seleccion
    void competencia(Cromosoma &nuevo_padre);
    
    //Realiza la cruza entre un padre y una madre, y guarda en hijos el resultado
    void cruza(Individuo padre, Individuo madre, std::vector<Individuo> &hijos);
    
    //Realiza la mutación de un padre en un hijo
    void mutacion(Individuo &individuo_a_mutar);

    //Funcion que sirve para ordenar individuos de mayor a menor
    bool ordenarIndividuos(Individuo a, Individuo b) {
        return a.fitness > b.fitness;
    }

};
#endif
