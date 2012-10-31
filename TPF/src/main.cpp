#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "utils.h"
#include "Config.h"
#include "GNUPlot.h"
#include "AlgoritmoGenetico.h"

//Variable global
Config config("configuracion.cfg"); //lectura de la configuracion

int main() {
    //inicializacion de semilla
    srand( (unsigned) std::time(NULL));

    //Leemos los valores de configuracion
      float           probabilidad_cruza      = utils::strToFloat(config.getValue("cruza"));
      float           probabilidad_mutacion   = utils::strToFloat(config.getValue("mutacion"));
      unsigned int    tamanio_poblacion       = utils::strToInt(config.getValue("tamanio_poblacion"));
      unsigned int    variables_fenotipo      = utils::strToInt(config.getValue("variables_fenotipo"));
      unsigned int    cantidad_generaciones   = utils::strToInt(config.getValue("cantidad_generaciones"));
      unsigned int    cantidad_genes          = utils::strToInt(config.getValue("cantidad_genes"));
      float           escala                  = utils::strToFloat(config.getValue("escala"));
      unsigned int    elitismo                = utils::strToInt(config.getValue("elitismo"));
      unsigned int    id_funcion_fitness      = utils::strToInt(config.getValue("id_funcion_fitness"));
      std::string     forma_seleccion         = config.getValue("forma_seleccion");
      float           error                   = utils::strToFloat(config.getValue("error"));
      unsigned int    brecha_generacional     = utils::strToInt(config.getValue("brecha_generacional"));
      unsigned int    k_competencia           = utils::strToInt(config.getValue("k_competencia"));
      float           fitness_deseado         = utils::strToFloat(config.getValue("fitness_deseado"));
      unsigned int    bits_por_clase           = utils::strToInt(config.getValue("bits_por_clase"));


    std::cout<<"Bienvenidos al Trabajo Final - Inteligencia Computacional - 2012\n";
    std::cout<<"Problema de Organización de Materias en una Facultad\n";
    std::cout<<"Nellmeldin Fernando - Peyregne Mariano\n";





    getwchar();
    return 0;
}
