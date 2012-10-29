#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "utils.h"
#include "Config.h"
#include "GNUPlot.h"
#include "AlgoritmoGenetico.h"
#include "gradiente.h"


//Variable global
Config config("configuracion1c.cfg"); //lectura de la configuracion

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

    std::cout<<"Bienvenidos al Ejercicio 1\n";
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
    float mejor_fitness_actual = 10.0;
    unsigned int w;
    for (w = 0; w < cantidad_generaciones; w++) {
        AG.reproduccion();

        mejor_fitness_actual = AG.evaluar();
        std::cout<<"Mejor fitness a iteracion "<<w<<" = "<<mejor_fitness_actual<<'\n';

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

    std::cout<<"Se termino luego de "<<w<<" generaciones.\nEl fitness logrado es de "<<mejor_fitness.back()<<'\n';

    AG.imprimirResumen();
    std::vector<bool> respuesta;

    AG.getMejorGenotipo(respuesta);

    if(id_funcion_fitness == 1 or id_funcion_fitness == 2)
        std::cout<<"\nSolucion = "<<utils::binary2int(respuesta)/escala;
    if(id_funcion_fitness == 3) {
        std::vector<int> soluciones;
        utils::vectorBinary2Int(respuesta,soluciones, 18);
        std::cout<<"\nSoluciones = ";
        for (unsigned int i = 0; i < soluciones.size(); i++)
            std::cout<<soluciones[i]/escala<<' ';

    }
    std::cout<<" con fitness = "<<mejor_fitness_actual<<'\n';

    //Vector de vector para graficacion
    std::vector<std::vector<float> > grafica;
    grafica.push_back(mejor_fitness);
    grafica.push_back(prom_fitness);
    grafica.push_back(peor_fitness);
    GNUPlot plotter;

    utils::drawHistory(grafica, plotter, id_funcion_fitness);

    std::cout<<"\nFin del Algoritmo Genetico. Ahora se realizara el metodo de gradiente desciendiente\n";


    //Gradiente
    std::vector<float> x_ini;
    switch(id_funcion_fitness){
    case 1: { //ejercicio 1a
        x_ini.push_back(359.0);
        break;
    }
    case 2: { //ejercicio 1b
        x_ini.push_back(2.0);
        break;
    }
    case 3: { //ejercicio 1c
        x_ini.push_back(0.1);
        x_ini.push_back(0.1);
        break;
    }
    }
    std::cout<<"Gradiente inicializado en "; utils::printVector((x_ini));
    //float tasa_inicial = 0.1; //tasa para 1a
    //float tasa_inicial = 0.01; //tasa para 1b
    float tasa_inicial = 0.01; //tasa para 1c

    float criterio_error = 0.0001;
    unsigned int maxit = 300;

    Gradiente gradiente(tasa_inicial, x_ini, id_funcion_fitness, criterio_error, maxit);

    unsigned int iteraciones;
    iteraciones = gradiente.descender();
    std::vector<float> solucion_segun_gradiente = gradiente.getSolucion();

    std::cout<<"\nGradiente descendiente:\nIteraciones = "<<iteraciones;
    std::cout<<"\nSolucion del gradiente = "; utils::printVector(solucion_segun_gradiente);



    getwchar();
    return 0;
}
