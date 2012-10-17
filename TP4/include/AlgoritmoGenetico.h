#ifndef __ALGORITMOGENETICO_H__
#define __ALGORITMOGENETICO_H__

#include <vector>
#include "Cromosoma.h"

class AlgoritmoGenetico {
    //Vector que tiene todos los cromosomas de esta poblacion
    std::vector<Cromosoma> poblacion;
    
    //Define el tamaño máximo de población a manejar
    unsigned int tamanio_poblacion;

    //Define la cantidad de genes que tiene cada cromosoma
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

    
    public:
    static const unsigned int SELECCION_RULETA = 1;
    static const unsigned int SELECCION_VENTANAS = 2;
    static const unsigned int SELECCION_COMPETENCIA = 3;
    
    //Constructor
    AlgoritmoGenetico(unsigned int tam_pob, unsigned int cant_genes, unsigned int max_gen, float pcruza, float pmutacion, unsigned int metodo_seleccion = 3, unsigned int k_competencia = 5, int n_ventanas = -1);

    //Realiza la selección de la poblacion, y guarda en nuevos_padres los Cromosomas elegidos, segun el metodo de seleccion definido, llama a Ruleta, Ventanas o Competencia
    void seleccion(std::vector<Cromosoma> &nuevos_padres);
    
    //Metodo de la ruleta para la seleccion
    void ruleta(std::vector<Cromosoma> &nuevos_padres);
    
    //Metodo de ventanas para la seleccion
    void ventanas(std::vector<Cromosoma> &nuevos_padres);
    
    //Metodo de competencias para la seleccion
    void competencia(Cromosoma &nuevo_padre);
    
    //Realiza la cruza entre un padre y una madre, y guarda en hijos el resultado
    void cruza(Cromosoma padre, Cromosoma madre, std::vector<Cromosoma> &hijos);
    
    //Realiza la mutación de un padre en un hijo
    void mutacion(Cromosoma padre, Cromosoma & hijo);

};
#endif
