#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include "utils.h"
#include "Config.h"
#include "GNUPlot.h"
#include "AlgoritmoGenetico.h"
#include "Clase.h"


//Variable global
Config config("configuracion.cfg"); //lectura de la configuracion basica
//Config config("configuracion_1er_real.cfg"); //lectura de la configuracion de caso real


int main() {
    //inicializacion de semilla
    srand( (unsigned) std::time(NULL));

    //Leemos los valores de configuracion
    float           probabilidad_cruza      = utils::strToFloat(config.getValue("cruza"));
    float           probabilidad_mutacion_permutacion   = utils::strToFloat(config.getValue("mutacion_permutacion"));
    float           probabilidad_mutacion_movimiento   = utils::strToFloat(config.getValue("mutacion_movimiento"));
    unsigned int    tamanio_poblacion       = utils::strToInt(config.getValue("tamanio_poblacion"));
    unsigned int    variables_fenotipo      = utils::strToInt(config.getValue("variables_fenotipo"));
    unsigned int    cantidad_generaciones   = utils::strToInt(config.getValue("cantidad_generaciones"));
    unsigned int    cantidad_genes          = utils::strToInt(config.getValue("cantidad_genes"));
    float           escala                  = utils::strToFloat(config.getValue("escala"));
    unsigned int    elitismo                = utils::strToInt(config.getValue("elitismo"));
    unsigned int    id_funcion_fitness      = utils::strToInt(config.getValue("id_funcion_fitness"));
    std::string     forma_seleccion         = config.getValue("forma_seleccion");
    std::string     archivo_problema        = config.getValue("archivo_problema");
    unsigned int    brecha_generacional     = utils::strToInt(config.getValue("brecha_generacional"));
    unsigned int    k_competencia           = utils::strToInt(config.getValue("k_competencia"));
    float           fitness_deseado         = utils::strToFloat(config.getValue("fitness_deseado"));
    unsigned int    bits_por_clase          = utils::strToInt(config.getValue("bits_por_clase"));
    unsigned int    aulas_disponibles       = utils::strToInt(config.getValue("aulas_disponibles"));
    std::string     modo_fitness            = config.getValue("modo_fitness");
    float           pot_cantidad_repeticiones         = utils::strToFloat(config.getValue("pot_cantidad_repeticiones"));
    float           pot_sobrepaso_aulas               = utils::strToFloat(config.getValue("pot_sobrepaso_aulas"));
    float           pot_solapamientos_adyacentes      = utils::strToFloat(config.getValue("pot_solapamientos_adyacentes"));
    float           pot_superposicion                 = utils::strToFloat(config.getValue("pot_superposicion"));
    float           mult_cantidad_repeticiones        = utils::strToFloat(config.getValue("mult_cantidad_repeticiones"));
    float           mult_sobrepaso_aulas              = utils::strToFloat(config.getValue("mult_sobrepaso_aulas"));
    float           mult_solapamientos_adyacentes     = utils::strToFloat(config.getValue("mult_solapamientos_adyacentes"));
    float           mult_superposicion                = utils::strToFloat(config.getValue("mult_superposicion"));

    //Variables para contar tiempo
    clock_t t_ini, t_fin;


    std::cout<<"Bienvenidos al Trabajo Final - Inteligencia Computacional - 2012\n";
    std::cout<<"Problema de Organización de Materias en una Facultad\n";
    std::cout<<"Nellmeldin Fernando - Peyregne Mariano\n";
    std::cout<<"Lectura de las clases.\n";
    std::vector<Clase> clases = utils::leerClases(archivo_problema);
    archivo_problema = archivo_problema + "_poblacion_"+utils::intToStr(tamanio_poblacion);

    unsigned int metodo_seleccion;
        if(forma_seleccion.compare("ruleta") == 0)
            metodo_seleccion = AlgoritmoGenetico::SELECCION_RULETA;
        else if(forma_seleccion.compare("ventanas") == 0)
            metodo_seleccion = AlgoritmoGenetico::SELECCION_VENTANAS;
        else if(forma_seleccion.compare("competencia") == 0)
            metodo_seleccion = AlgoritmoGenetico::SELECCION_COMPETENCIA;
        else
            std::cout<<"Metodo de seleccion no definido\n";

    unsigned int metodo_fitness;
    if(modo_fitness.compare("producto") == 0)
        metodo_fitness = Individuo::FITNESS_PRODUCTO;
    else if(modo_fitness.compare("suma") == 0)
        metodo_fitness = Individuo::FITNESS_SUMA;
    else
        std::cout<<"Modo de fitness no definido\n";


    //Inicializamos el contador
    t_ini = clock();

    AlgoritmoGenetico AG(tamanio_poblacion, cantidad_genes, escala, variables_fenotipo,
                         cantidad_generaciones, probabilidad_cruza, probabilidad_mutacion_movimiento, probabilidad_mutacion_permutacion,
                         elitismo, brecha_generacional, id_funcion_fitness, clases, aulas_disponibles,
                         metodo_seleccion, k_competencia, bits_por_clase, metodo_fitness,
                         pot_cantidad_repeticiones, pot_sobrepaso_aulas, pot_solapamientos_adyacentes, pot_superposicion,
                         mult_cantidad_repeticiones, mult_sobrepaso_aulas, mult_solapamientos_adyacentes, mult_superposicion);

    std::vector<float> mejor_fitness;
    std::vector<float> prom_fitness;
    std::vector<float> peor_fitness;

    mejor_fitness.push_back(AG.getMejorFitness());
    peor_fitness.push_back(AG.getPeorFitness());
    std::vector<float> fitness_iniciales;
    AG.getFitness(fitness_iniciales);
    prom_fitness.push_back(utils::promedio(fitness_iniciales));

    unsigned int w = 0;
    for (; w < cantidad_generaciones; w++) {
        AG.reproduccion();

        float mejor_fitness_actual = AG.evaluar();
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
        if (mejor_fitness_actual >= fitness_deseado) {
           break;
        }
        /*
        if (w >= 100 and w % 100 == 0) {//guarda un archivo cada 100 generaciones
            std::string archivo_salida = archivo_problema + "_tabla_gen_" + utils::intToStr(w) + ".xls";

            std::vector<bool> respuesta;
            AG.getMejorGenotipo(respuesta);
            std::vector<int> respuesta_fenotipo;
            utils::vectorBinary2Int(respuesta, respuesta_fenotipo, bits_por_clase);
            std::vector<std::vector<std::vector<int> > > solucion = AG.getSolucion();

            std::cout<<"Escribiendo archivo "<<archivo_salida<<".\n";
            utils::escribirSolucion(solucion, respuesta_fenotipo, clases, archivo_salida);

            std::string archivo_resumen = archivo_problema + "_resumen_gen_" + utils::intToStr(w) + ".txt";
            std::cout<<"Escribiendo archivo "<<archivo_resumen<<".\n";
            AG.imprimirResumen(archivo_resumen);
        }*/
    }
    t_fin = clock();
    std::cout<<"\n\nTiempo del Metodo = "<<(double)(t_fin - t_ini) / CLOCKS_PER_SEC<<"\n\n\n";

    std::cout<<"\nSe termino luego de "<<w<<" generaciones.\nEl fitness logrado es de "<<mejor_fitness.back()<<'\n';

    AG.imprimirResumen();
    std::string archivo_resumen = archivo_problema + "_resumen_gen_" + utils::intToStr(w) + ".txt";
    std::cout<<"Escribiendo archivo "<<archivo_resumen<<".\n";
    AG.imprimirResumen(archivo_resumen);
    std::vector<bool> respuesta;
    std::vector<int> respuesta_fenotipo;

    AG.getMejorGenotipo(respuesta);

    utils::vectorBinary2Int(respuesta, respuesta_fenotipo, bits_por_clase);

    std::cout<<"\nSolucion = "; utils::printVector(respuesta_fenotipo);

    //Vector de vector para graficacion
    std::vector<std::vector<float> > grafica;
    grafica.push_back(mejor_fitness);
    grafica.push_back(prom_fitness);
    grafica.push_back(peor_fitness);

    GNUPlot plotter;
    utils::drawHistory(grafica, plotter, id_funcion_fitness);

    std::vector<std::vector<std::vector<int> > > solucion = AG.getSolucion();

    std::cout<<"Escribiendo archivo SALIDA.xls.\n";
    std::string archivo_SALIDA = archivo_problema + "_SALIDA.xls";
    utils::escribirSolucion(solucion, respuesta_fenotipo, clases, archivo_SALIDA);

    std::cout<<"Escribiendo vectores para graficar en GRAFICA.xls\n";
    std::string archivo_GRAFICA = archivo_problema + "_GRAFICA.xls";
    utils::escribirGraficas(grafica, archivo_GRAFICA);

    getwchar();
    return 0;
}
