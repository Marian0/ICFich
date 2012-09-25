#ifndef __REDRBF_H__
#define __REDRBF_H__

#include <vector>
#include <string>
#include "Neurona.h"
#include "NeuronaRBF.h"

#define EPS 0.1
class RedRBF {
    //Vector que almancena la primer capa de neuronas gaussianas
    std::vector<NeuronaRBF> neuronasRBF;
    //Vector que almacena la segunda capa de neuronas tipo perceptron
    std::vector<Neurona> neuronasP;
    //Cantidad de neuronas gaussianas
    unsigned int cantidad_rbf;
    //Cantidad de neuronas tipo perceptron
    unsigned int cantidad_n;
    //Cantidad de entradas
    unsigned int cantidad_entradas;
    //Cantidad de salidas 
    unsigned int cantidad_salidas;
    
    //Parametro de la funcion sigmoidea
    float parametro_sigmoidea;

    //Nombre de la red
    std::string nombre_red;

    //Vector que guardara el error instantaneo en cada iteracion
    std::vector<float> error_instantaneo;

    public:
    //Constructor que toma un archivo y lee los parametros de la red. El resto de los parametros se le pasan desde donde se construye
    RedRBF(std::string nombre_archivo, std::string nombre_red, float tasa_aprendizaje, float sigma, unsigned int funcion_activacion, float par_sigmoidea );
    
    //Realiza el entrenamiento. Primero llama a kmeans y luego a singleTrain con cada patron
    float train(std::vector<std::vector<float> > X, std::vector<std::vector<float> > YD, bool entrena);

    //Para un caso en particular, calcula su salida y actualiza los pesos
    bool singleTrain(std::vector<float> X, std::vector<float> YD, bool entrena = true);

    //Realiza el algoritmo de k-means
    void kmeans(std::vector<std::vector<float> > entradas);

    std::vector<std::vector<float> > getMus();
};

#endif
