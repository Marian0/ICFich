#ifndef __INDIVIDUO_H__
#define __INDIVIDUO_H__
#include <vector>
#include "Clase.h"
class Individuo {
    public:

    unsigned int cantidad_repeticiones;
    unsigned int sobrepaso_aulas;
    unsigned int solapamientos_adyacentes;
    unsigned int basura;
    //Vector con los valores de la cadena de bits
    std::vector<bool> genotipo;

    //Especifica el factor de division del fenotipo
    float escala;

    //Id de la funcion de fitness a usar
    unsigned int funcion_fitness_id;
    
    //Ultimo valor obtenido de la función de fitness
    float fitness; 

    //Dice cuantas variables tiene el fenotipo
    unsigned int variables_fenotipo;

    //Guarda las caracteristicas de cada clase
    std::vector<Clase> clases;

    //Matriz de bool que representa los bloques ocupados en este individuo
    std::vector<std::vector<std::vector<bool> > > matriz_bool;
    //Matriz de int que representa el bloque que ocupa cada materia en este individuo
    std::vector<std::vector<std::vector<int> > > matriz_int;

    unsigned int aulas_disponibles;

    //Constructor vacio
    Individuo();

    //Constructor
    Individuo(unsigned int cantidad_genes, unsigned int funcion_fitness_id,  std::vector<Clase> clases, unsigned int aulas_disponibles, float escala = 1.00, unsigned int variables_fenotipo = 1);

    //Calcula el fitness actual y lo guarda en la propiedad fitness
    float calcularFitness();

    //Devuelve el fitness actual
    float getFitness();

    //Devuelve el Fenotipo del Individuo
    float getFenotipo();

    //Llama a los calculos de las dos matrices
    void calcularMatrices();
    //Calcula la matriz int
    void calcularMatrizInt();
    //Calcula la matriz bool
    void calcularMatrizBool();

    //Devuelve todos los valores que sirvieron para calcular el fitness
    std::vector<unsigned int> getValoresFitness();
};
#endif
