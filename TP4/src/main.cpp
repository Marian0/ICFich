#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "utils.h"
#include "Config.h"
#include "GNUPlot.h"
#include "Individuo.h"

//Variable global
Config config("configuracion1.cfg"); //lectura de la configuracion

int main(int argc, char *argv[]) {
    //inicializacion de semilla
    srand( (unsigned) std::time(NULL));

    //Leemos los valores de configuracion
    float           porcentaje_cruza    = utils::strToFloat(config.getValue("cruza"));
    float           porcentaje_mutacion = utils::strToFloat(config.getValue("mutacion"));
    unsigned int    maxit               = utils::strToInt(config.getValue("maxit"));
    unsigned int    tamanio_poblacion   = utils::strToInt(config.getValue("tamanio_poblacion"));
    float           elitismo            = utils::strToFloat(config.getValue("elitismo"));
    std::string     forma_seleccion     = config.getValue("forma_seleccion");
    float           error               = utils::strToFloat(config.getValue("error"));
    unsigned int    brecha_generacional = utils::strToInt(config.getValue("brecha_generacional"));
    unsigned int    ventanas_inicial    = utils::strToInt(config.getValue("ventanas_inicial"));
    unsigned int    k_competencia       = utils::strToInt(config.getValue("k_competencia"));
    float           fitness_deseado     = utils::strToFloat(config.getValue("fitness_deseado"));
    std::cout<<"Aleloooooooooo";
    getwchar();
    return 0;
}
