#ifndef __REDRBF_H__
#define __REDRBF_H__

#include <vector>
#include <string>
#include "NeuronaSOM.h"

#define EPS 0.0001
class RedSOM {
    //Matriz de las neuronas
    std::vector<std::vector<NeuronaSOM> > neuronas;

    //Alto de la grilla
    unsigned int alto;
    //Ancho de la grilla
    unsigned int ancho;

    //guardo el numero de iteración de entrenamiento para calculos de las tasas
    //Numero de iteracion
    unsigned int iteracion;

    //maximo cantidad de iteraciones en convergencia
    unsigned int maxit_convergencia;

    //Vector que guardara el error instantaneo en cada iteracion
    std::vector<float> error_instantaneo;
    
    //Define la cantidad de clases posibles que puede tomar la red
    unsigned int cantidad_clases;

    //Sigma inicial
    float sigma0;
    //Sigma de iteración
    float sigma;
    //Cte aprendizaje inicial
    float cte_aprendizaje0;
    //Cte de aprendizaje de iteración
    float cte_aprendizaje;
    //Constantes tau
    float tau1;
    float tau2;

    //Valores usados para la etapa de convergencia
    float sigma_convergencia;
    float cte_aprendizaje_convergencia;

    //dimension de las entradas
    unsigned int dimension;

    //Objeto de GNUPlot para dibujar
    GNUPlot plotter; 

    public:
    
    //Constructor que toma un archivo y lee los parametros de la red. El resto de los parametros se le pasan desde donde se construye
    RedSOM(unsigned int dimension, unsigned int cantidad_clases, unsigned int alto, unsigned int ancho, float sigma0, float cte_aprendizaje0, float tau2, unsigned int maxit);
    
    //Realiza el entrenamiento. Primero llama a kmeans y luego a singleTrain con cada patron
    float train(std::vector<std::vector<float> > X, std::vector<std::vector<float> > YD, std::vector<std::vector<float> > &YC, bool entrena = true, bool actualizar_valores = true);
    
    //Para un caso en particular, calcula su salida y actualiza los pesos
    std::vector<unsigned int> singleTrain(std::vector<float> X, std::vector<float> YD, bool entrena = true);

    //Devuelve en X las coordenadas de los pesos de cada neurona para graficar
    void getPuntos(std::vector<std::vector<float> > & X);
    
    //Devuelve la cantidad de neuronas
    unsigned int getCantidadNeuronas();

    //Setea la constante de aprendizaje
    void setCteAprendizaje(float new_val);

    //Setea el sigma
    void setSigma(float new_val);
    
    //Calcula la funcion vecindario
    float vecindario(float distancia);

    //Define la clase de todas las neuronas en base a sus contadores
    void definirClaseNeuronas();
};

#endif
