#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "utils.h"
#include "Config.h"
#include "GNUPlot.h"


//Variable global
Config config("configuracion.cfg"); //lectura de la configuracion

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

//    unsigned int metodo_seleccion;
//    if(forma_seleccion.compare("ruleta") == 0)
//        metodo_seleccion = AlgoritmoGenetico::SELECCION_RULETA;
//    else if(forma_seleccion.compare("ventanas") == 0)
//        metodo_seleccion = AlgoritmoGenetico::SELECCION_VENTANAS;
//    else if(forma_seleccion.compare("competencia") == 0)
//        metodo_seleccion = AlgoritmoGenetico::SELECCION_COMPETENCIA;
//    else
//        std::cout<<"Metodo de seleccion no definido\n";


//    //Instanciamos el algoritmo genetico
//    AlgoritmoGenetico AG (tamanio_poblacion, cantidad_genes, escala, variables_fenotipo, cantidad_generaciones,
//                          probabilidad_cruza, probabilidad_mutacion, elitismo, brecha_generacional,
//                          id_funcion_fitness, metodo_seleccion, k_competencia);

//    //Definimos vectores para graficación
//    std::vector<float> mejor_fitness, prom_fitness, peor_fitness;

//    //Guardo el mejor fitness de la poblacion
//    mejor_fitness.push_back(AG.getMejorFitness());

//    //Calculo y guardo el fitness promedio de la poblacion
//    std::vector<float> vector_tmp_fitness;
//    AG.getFitness(vector_tmp_fitness);
//    prom_fitness.push_back(utils::promedio(vector_tmp_fitness));

//    //Guardo el Peor fitness de la poblacion
//    peor_fitness.push_back(AG.getPeorFitness());

//    unsigned int w;
//    for (w = 0; w < cantidad_generaciones; w++) {
//        AG.reproduccion();

//        float mejor_fitness_actual = AG.evaluar();
//        std::cout<<"Mejor fitness a iteracion "<<w<<" = "<<mejor_fitness_actual<<'\n';

//        //Guardo el mejor fitness de la poblacion
//        mejor_fitness.push_back(AG.getMejorFitness());
//        //Calculo y guardo el fitness promedio de la poblacion
//        std::vector<float> vector_tmp_fitness;
//        AG.getFitness(vector_tmp_fitness);
//        prom_fitness.push_back(utils::promedio(vector_tmp_fitness));
//        //Guardo el Peor fitness de la poblacion
//        peor_fitness.push_back(AG.getPeorFitness());


//        //Criterio de finalización
//        if (mejor_fitness_actual > fitness_deseado) {
//            break;
//        }
//    }

//    std::cout<<"Se termino luego de "<<w<<" generaciones.\nEl fitness logrado es de "<<mejor_fitness.back()<<'\n';

//    AG.imprimirResumen();
//    std::vector<bool> respuesta;

//    AG.getMejorGenotipo(respuesta);

//    if(id_funcion_fitness == 1 or id_funcion_fitness == 2)
//        std::cout<<"\nSolucion = "<<utils::binary2int(respuesta);
//    if(id_funcion_fitness == 3) {
//        std::vector<int> soluciones;
//        utils::vectorBinary2Int(respuesta,soluciones, 7);
//        std::cout<<"\nSoluciones = ";
//        for (unsigned int i = 0; i < soluciones.size(); i++)
//            std::cout<<soluciones[i]<<' ';
//        std::cout<<std::endl;
//    }

//    //Vector de vector para graficacion
//    std::vector<std::vector<float> > grafica;
//    grafica.push_back(mejor_fitness);
//    grafica.push_back(prom_fitness);
//    grafica.push_back(peor_fitness);
//    GNUPlot plotter;

//    utils::drawHistory(grafica, plotter, id_funcion_fitness);

    getwchar();
    return 0;
}
