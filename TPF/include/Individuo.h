#ifndef __INDIVIDUO_H__
#define __INDIVIDUO_H__
#include <vector>
#include "Clase.h"
class Individuo {
    public:

    unsigned int cantidad_repeticiones;
    unsigned int sobrepaso_aulas;
    unsigned int solapamientos_adyacentes;
    unsigned int superposicion;

    float pot_cantidad_repeticiones;
    float pot_sobrepaso_aulas;
    float pot_solapamientos_adyacentes;
    float pot_superposicion;

    float mult_cantidad_repeticiones;
    float mult_sobrepaso_aulas;
    float mult_solapamientos_adyacentes;
    float mult_superposicion;

    unsigned int modo_fitness;
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
    static const unsigned int FITNESS_PRODUCTO = 1;
    static const unsigned int FITNESS_SUMA = 2;


    //Constructor vacio
    Individuo();

    //Constructor
    Individuo(unsigned int cantidad_genes, unsigned int funcion_fitness_id, std::vector<Clase> clases,
                         unsigned int aulas_disponibles, float escala, unsigned int variables_fenotipo,
                         unsigned int modo_fitness,
                         float pot_repeticiones = 1, float pot_aulas = 1, float pot_solapamiento = 2, float pot_superposicion = 5,
                         float mult_repeticiones = 0.1, float mult_aulas = 0.1, float mult_solapamiento = 0.3, float mult_superposicion = 0.5);

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
