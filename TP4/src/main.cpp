#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "utils.h"
#include "Config.h"
#include "GNUPlot.h"
#include "AlgoritmoGenetico.h"


//Variable global
Config config("configuracion2.cfg"); //lectura de la configuracion

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

    std::cout<<"Bienvenidos al Ejercicio 2\n";
    std::cout<<"Tamanio de poblacion = "<<tamanio_poblacion<<'\n';
    std::cout<<"Metodo de seleccion = "<<forma_seleccion<<'\n';
    std::cout<<"Probabilidad Cruza = "<<probabilidad_cruza<<'\n';
    std::cout<<"Probabilidad Mutacion = "<<probabilidad_mutacion<<'\n';
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


    //Instanciamos el algoritmo genetico
    AlgoritmoGenetico AG (tamanio_poblacion, cantidad_genes, escala, variables_fenotipo, cantidad_generaciones,
                          probabilidad_cruza, probabilidad_mutacion, elitismo, brecha_generacional,
                          id_funcion_fitness, metodo_seleccion, k_competencia);

    //Leo el agente viajero
    //AG.cargarAgenteViajero("ciudades_facil.txt"); //el recorrido es 0-1-2-3
    AG.cargarAgenteViajero("ciudades.txt"); //el recorrido es ...

    //Evaluo el fitness inicial
    AG.evaluar();

    //Definimos vectores para graficación
    std::vector<float> mejor_fitness, prom_fitness, peor_fitness;

    //Guardo el mejor fitness de la poblacion
    mejor_fitness.push_back(AG.getMejorFitness());

    //Calculo y guardo el fitness promedio de la poblacion
    std::vector<float> vector_tmp_fitness;
    AG.getFitness(vector_tmp_fitness);
    prom_fitness.push_back(utils::promedio(vector_tmp_fitness));

    //Guardo el Peor fitness de la poblacion
    peor_fitness.push_back(AG.getPeorFitness());

    unsigned int w;
    for (w = 0; w < cantidad_generaciones; w++) {
        AG.reproduccion();

        float mejor_fitness_actual = AG.evaluar();
        std::cout<<"Mejor fitness a iteracion "<<w<<" = "<<mejor_fitness_actual<<'\r';

        //Guardo el mejor fitness de la poblacion
        mejor_fitness.push_back(AG.getMejorFitness());
        //Calculo y guardo el fitness promedio de la poblacion
        std::vector<float> vector_tmp_fitness;
        AG.getFitness(vector_tmp_fitness);
        prom_fitness.push_back(utils::promedio(vector_tmp_fitness));
        //Guardo el Peor fitness de la poblacion
        peor_fitness.push_back(AG.getPeorFitness());


        //Criterio de finalización
        if (mejor_fitness_actual > fitness_deseado) {
            break;
        }
    }


    std::cout<<"\nSe termino luego de "<<w<<" generaciones.\nEl fitness logrado es de "<<mejor_fitness.back()<<'\n';

    AG.imprimirResumen();
    std::vector<bool> respuesta;
    std::vector<int> respuesta_fenotipo;

    AG.getMejorGenotipo(respuesta);
    utils::vectorBinary2Int(respuesta, respuesta_fenotipo, 4);

    std::cout<<"\nSolucion = "; utils::printVector(respuesta_fenotipo);

    //Vector de vector para graficacion
    std::vector<std::vector<float> > grafica;
    grafica.push_back(mejor_fitness);
    grafica.push_back(prom_fitness);
    grafica.push_back(peor_fitness);
    GNUPlot plotter;

    utils::drawHistory(grafica, plotter, id_funcion_fitness);

    getwchar();
    return 0;
}
