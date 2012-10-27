#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "utils.h"
#include "Config.h"
#include "GNUPlot.h"
#include "enjambre.h"


//Variable global
Config config("configuracion3c.cfg"); //lectura de la configuracion

int main() {
    //inicializacion de semilla
    srand( (unsigned) std::time(NULL));

    //Leemos los valores de configuracion
    unsigned int    cantidad_de_particulas       = utils::strToInt(config.getValue("cantidad_de_particulas"));
    float           c1      = utils::strToFloat(config.getValue("c1"));
    float           c2   = utils::strToFloat(config.getValue("c2"));
    unsigned int    maxit      = utils::strToInt(config.getValue("maxit"));
    unsigned int    id_funcion_fitness   = utils::strToInt(config.getValue("id_funcion_fitness"));
    unsigned int    entorno_size          = utils::strToInt(config.getValue("entorno_size"));


    std::cout<<"Bienvenidos al Ejercicio 3\n";
    std::cout<<"Cantidad de Partículas = "<<cantidad_de_particulas<<'\n';
    std::cout<<"C1 = "<<c1<<'\n';
    std::cout<<"C2 = "<<c2<<'\n';
    std::cout<<"Iteraciones Máximas = "<<maxit<<'\n';
    std::cout<<"ID funcion fitness = "<<id_funcion_fitness<<'\n';
    std::cout<<"Tamaño del Entorno Anillo = "<<entorno_size<<'\n';
    std::vector<std::vector<float> > limites;

    if (id_funcion_fitness == 1)
        utils::parseCSV("limites3a.csv", limites);
    else if (id_funcion_fitness == 2)
        utils::parseCSV("limites3b.csv", limites);
    else if (id_funcion_fitness == 3)
        utils::parseCSV("limites3c.csv", limites);



    std::vector<float> limites_inf = limites[0];
    std::vector<float> limites_sup = limites[1];

    std::cout<<"Limites Inferiores = "; utils::printVector(limites_inf);
    std::cout<<"Limites Superiores = "; utils::printVector(limites_sup);

    Enjambre enjambre (limites_inf, limites_sup, maxit, cantidad_de_particulas, id_funcion_fitness, c1, c2, entorno_size);
    for (unsigned int i = 0; i < maxit; i++) {
        enjambre.iterar();

    }

    std::vector<float> solucion = enjambre.getSolucion();
    std::cout<<"Solucion = "; utils::printVector(solucion);


//    GNUPlot plotter;

//    utils::drawHistory(grafica, plotter, id_funcion_fitness);

    getwchar();
    return 0;
}
