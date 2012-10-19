#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "utils.h"
#include "Config.h"
#include "GNUPlot.h"
#include "AlgoritmoGenetico.h"

//Variable global
Config config("configuracion1.cfg"); //lectura de la configuracion

int main(int argc, char *argv[]) {
    //inicializacion de semilla
    srand( (unsigned) std::time(NULL));

    //Leemos los valores de configuracion
    float           porcentaje_cruza        = utils::strToFloat(config.getValue("cruza"));
    float           porcentaje_mutacion     = utils::strToFloat(config.getValue("mutacion"));
    unsigned int    maxit                   = utils::strToInt(config.getValue("maxit"));
    unsigned int    tamanio_poblacion       = utils::strToInt(config.getValue("tamanio_poblacion"));
    unsigned int    cantidad_generaciones   = utils::strToInt(config.getValue("cantidad_generaciones"));
    unsigned int    cantidad_genes          = utils::strToInt(config.getValue("cantidad_genes"));
    unsigned int    elitismo                = utils::strToInt(config.getValue("elitismo"));
    unsigned int    id_funcion_fitness      = utils::strToInt(config.getValue("id_funcion_fitness"));
    std::string     forma_seleccion         = config.getValue("forma_seleccion");
    float           error                   = utils::strToFloat(config.getValue("error"));
    unsigned int    brecha_generacional     = utils::strToInt(config.getValue("brecha_generacional"));
    unsigned int    n_ventanas              = utils::strToInt(config.getValue("n_ventanas"));
    unsigned int    k_competencia           = utils::strToInt(config.getValue("k_competencia"));
    float           fitness_deseado         = utils::strToFloat(config.getValue("fitness_deseado"));

    std::cout<<"Bienvenidos al Ejercicio 1\n";
    std::cout<<"Tamanio de poblacion = "<<tamanio_poblacion<<'\n';
    std::cout<<"Metodo de seleccion = "<<forma_seleccion<<'\n';
    std::cout<<"Porcentaje Cruza = "<<porcentaje_cruza<<'\n';
    std::cout<<"Porcentaje Mutacion = "<<porcentaje_mutacion<<'\n';
    std::cout<<"Elitismo = "<<elitismo<<'\n';

    unsigned int metodo_seleccion;
    if(forma_seleccion.compare("ruleta") == 0)
        metodo_seleccion = AlgoritmoGenetico::SELECCION_RULETA;
    else if(forma_seleccion.compare("ventanas") == 0)
        metodo_seleccion = AlgoritmoGenetico::SELECCION_VENTANAS;
    else if(forma_seleccion.compare("competencia") == 0)
        metodo_seleccion = AlgoritmoGenetico::SELECCION_COMPETENCIA;
    else
        std::cout<<"Metodo de seleccion no definido\n";



    AlgoritmoGenetico AG (tamanio_poblacion, cantidad_genes, cantidad_generaciones,
                          porcentaje_cruza, porcentaje_mutacion, elitismo, id_funcion_fitness,
                          metodo_seleccion, k_competencia, n_ventanas);

    std::vector<float> mejor_fitness;
    mejor_fitness.push_back(AG.getMejorFitness());

    unsigned int w;
    for (w = 0; w < cantidad_generaciones; w++) {
        AG.reproduccion();

        float mejor_fitness_actual = AG.evaluar();
        std::cout<<"Mejor fitness a iteracion "<<w<<" = "<<mejor_fitness_actual<<'\n';
        mejor_fitness.push_back(mejor_fitness_actual);

        if (mejor_fitness_actual > fitness_deseado) {
            break;
        }
    }

    std::cout<<"Se termino luego de "<<w<<" generaciones.\nEl fitness logrado es de "<<mejor_fitness.back()<<'\n';



    getwchar();
    return 0;
}
