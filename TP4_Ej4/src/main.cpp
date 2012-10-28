#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "utils.h"
#include "Config.h"
#include "GNUPlot.h"
#include "enjambre.h"
#include "Red.h"


//Variable global
Config config("configuracion4.cfg"); //lectura de la configuracion

int main() {
    //inicializacion de semilla
    srand( (unsigned) std::time(NULL));

    //Leemos los valores de configuracion
    unsigned int    cantidad_de_particulas         = utils::strToInt(config.getValue("cantidad_de_particulas"));
    float           c1                             = utils::strToFloat(config.getValue("c1"));
    float           c2                             = utils::strToFloat(config.getValue("c2"));
    unsigned int    maxit                          = utils::strToInt(config.getValue("maxit"));
    unsigned int    id_funcion_fitness             = utils::strToInt(config.getValue("id_funcion_fitness"));
    unsigned int    entorno_size                   = utils::strToInt(config.getValue("entorno_size"));
    float           dimension                      = utils::strToFloat(config.getValue("dimension"));

    std::string     archivo_problema               = config.getValue("archivo_problema"); //Archivo a leer patrones ej xor.csv
    unsigned int    porcentaje_entrenamiento       = utils::strToInt(config.getValue("porcentaje_entrenamiento"));
    unsigned int    porcentaje_prueba              = utils::strToInt(config.getValue("porcentaje_prueba"));
    float           tasa_aprendizaje               = utils::strToFloat(config.getValue("tasa_aprendizaje"));
    float           parametro_sigmoidea            = utils::strToFloat(config.getValue("parametro_sigmoidea"));
    float           parametro_momento              = utils::strToFloat(config.getValue("parametro_momento"));


    std::cout<<"Bienvenidos al Ejercicio 4\n";
    std::cout<<"Cantidad de Partículas = "<<cantidad_de_particulas<<'\n';
    std::cout<<"C1 = "<<c1<<'\n';
    std::cout<<"C2 = "<<c2<<'\n';
    std::cout<<"Iteraciones Máximas = "<<maxit<<'\n';
    std::cout<<"ID funcion fitness = "<<id_funcion_fitness<<'\n';
    std::cout<<"Tamaño del Entorno Anillo = "<<entorno_size<<'\n';

    std::vector<std::vector<float> > patrones;

    //Inicializacion del perceptron multicapa
    utils::parseCSV(archivo_problema.c_str(), patrones);
    std::vector<std::vector<float> > entrenamiento, prueba, validacion;
    //Genera las particiones de entrenamiento y prueba
    utils::genParticiones(patrones, entrenamiento, validacion, prueba, porcentaje_entrenamiento, porcentaje_prueba, 0);

    std::vector<std::vector<float> > X, Yd;
    //Divido en X y Yd los casos de entrenamiento
    utils::splitVector(entrenamiento, X, Yd, 1);

    //Instancio la red
    Red perceptron("estructura4.txt","Red Perceptron", tasa_aprendizaje, Neurona::FUNCION_SIGMOIDEA, parametro_sigmoidea, parametro_momento);

    std::vector<float> limites_inf;
    std::vector<float> limites_sup;
    //Agregamos todos los limites
    for (unsigned int i = 0; i < dimension; i++) {
        limites_inf.push_back(-10);
        limites_sup.push_back( 10);
    }

    std::vector<float> historial_error;
    std::vector<float> historial_fitness;

    Enjambre enjambre (limites_inf, limites_sup, maxit, cantidad_de_particulas, id_funcion_fitness, c1, c2, entorno_size, perceptron, X, Yd);

    for (unsigned int i = 0; i < maxit; i++) {
        float error = 1-enjambre.iterar();
        std::cout<<"Iteracion "<<i<<' ';
        std::vector<float> nuevos_pesos = enjambre.getSolucion();
        //std::cout<<"\nError = "<<error;
        //std::cout<<"\nFitness = "<<enjambre.getMejorFitness();
        //std::cout<<"\nSolucion a iteracion "<<i<<" = "; utils::printVector(nuevos_pesos);

        historial_error.push_back(error);
        historial_fitness.push_back(enjambre.getMejorFitness());
    }

    GNUPlot plotter;
    std::vector<std::vector<float> > historiales;
    //historiales.push_back(historial_error);
    historiales.push_back(historial_fitness);
    utils::drawHistory(historiales, plotter, 6);

    std::vector<float> solucion = enjambre.getSolucion();
    std::cout<<"Solucion = "; utils::printVector(solucion);

    perceptron.setPesos(solucion);
    float error_entrenamiento = perceptron.train(X,Yd,false);
    std::cout<<"Error (entrenamiento) = "<<error_entrenamiento*100<<"\%\n";

    //Divido en X y Yd los casos de entrenamiento
    utils::splitVector(prueba, X, Yd, 1);
    float error_prueba = perceptron.train(X,Yd,false);
    std::cout<<"Error (prueba) = "<<error_prueba*100<<"\%\n";

    getwchar();
    return 0;
}
