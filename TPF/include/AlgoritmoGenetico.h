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

    //Guarda el indice donde esta el maximo fitness
    unsigned int id_maximo_fitness;

    //Guarda el indice donde esta el minimo fitness
    unsigned int id_minimo_fitness;

    //Parametros para la reproduccion
    //Define los porcentajes para cada operacion
    float probabilidad_cruza;
    float probabilidad_mutacion;

    //Parametros para los algortimos de seleccion
    //Define la cantidad de padres a competir en competencia
    unsigned int k_competencia;

    //Define la cantidad de elites que se van a quedar en la siguiente poblacion
    unsigned int n_elitismo;

    //Define la cantidad de individuos que se van a pasar, aleatoriamente, a la proxima generacion
    unsigned int n_brecha_generacional;

    //Define el metodo de seleccion a usar, de acuerdo a SELECCION_*
    unsigned int metodo_seleccion;

    unsigned int id_funcion_fitness;

    //Contadores para saber la cantidad de operaciones que se hacen
    unsigned int cantidad_mutaciones;
    unsigned int cantidad_cruzas;

    //Escala con la que se dividen los fenotipos
    float escala;
    //Cantidad de variables en el fenotipo
    unsigned int variables_fenotipo;

    public:

    static const unsigned int SELECCION_RULETA = 1;
    static const unsigned int SELECCION_VENTANAS = 2;
    static const unsigned int SELECCION_COMPETENCIA = 3;
    
    //Constructor
    AlgoritmoGenetico(unsigned int tam_pob, unsigned int cant_genes, float escala, unsigned int variables_fenotipo, unsigned int max_gen,
                      float pcruza, float pmutacion, unsigned int elitismo, unsigned int brecha_generacional, unsigned int id_funcion_fitness,
                      unsigned int metodo_seleccion = SELECCION_COMPETENCIA ,
                      unsigned int k_competencia = 5
                      );

    //Crea la nueva generacion
    void reproduccion();

    //Evalua la poblacion y devuelve el mejor fitness
    float evaluar();

    //Realiza la selección de la poblacion, y guarda en nuevos_padres los Individuos elegidos, segun el metodo de seleccion definido, llama a Ruleta, Ventanas o Competencia
    void seleccion(std::vector<Individuo> &nuevos_padres, unsigned int cantidad_a_generar);
    
    //Metodo de la ruleta para la seleccion
    void ruleta(std::vector<Individuo> &nuevos_padres, unsigned int cantidad_a_generar);
    
    //Metodo de ventanas para la seleccion
    void ventanas(std::vector<Individuo> &nuevos_padres, unsigned int cantidad_a_generar);
    
    //Metodo de competencias para la seleccion
    void competencia(std::vector<Individuo> &nuevos_padres, unsigned int cantidad_a_generar);
    
    //Realiza la cruza entre un padre y una madre, y guarda en hijos el resultado
    void cruza(Individuo & padre, Individuo & madre, std::vector<Individuo> &hijos);
    
    //Realiza la mutación de un padre en un hijo
    void mutacion(Individuo &individuo_a_mutar);

    //Devuelve por referencia un vector de los fitness de toda la poblacion
    void getFitness(std::vector<float> &fitness_todos);

    //Devuelve el mejor fitness de la poblacion
    float getMejorFitness();

    //Devuelve el peor fitness de la poblacion
    float getPeorFitness();

    //Devuelve el mejor genotipo de la poblacion
    void getMejorGenotipo(std::vector<bool> &mejor_genotipo);

    //Devuelve el mejor Fenotipo
    float getMejorSolucion();

    //Muestra un resumen de todas las iteraciones del algoritmo
    void imprimirResumen();

    //Funcion que sirve para ordenar individuos de mayor a menor
    static bool ordenarIndividuos(Individuo a, Individuo b) {
        return a.fitness > b.fitness;
    }
};
#endif
