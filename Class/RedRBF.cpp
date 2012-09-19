#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

#include "RedRBF.h"
RedRBF::RedRBF(std::string nombre_archivo, std::string nombre_red, unsigned int funcion_activacion, float tasa_aprendizaje) {
    //Leo los parametros de la red desde un archivo
    std::ifstream file (nombre_archivo.c_str());
    assert(file.is_open());
    unsigned int c_e, c_rbf, c_n, k;
    file>>c_e>>c_rbf>>c_n>>k;
    file.close();
    
    std::cout<<"Parametros de la red: "<<c_e<<' '<<c_rbf<<' '<<c_n<<' '<<k<<'\n';

    //Asigno la cantidad de cada uno de los parametros de la red
    this->cantidad_entradas = c_e;
    this->cantidad_rbf = c_rbf;
    this->cantidad_n = c_n;
    this->cantidad_clases = k;
    
    //Creo las neuronas tipo rbf
    for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
        NeuronaRBF neuRBF(this->cantidad_entradas);
        this->neuronasRBF.push_back(neuRBF);
    }

    //Creo las neuronas tipo perceptron
    for (unsigned int i = 0; i < this->cantidad_n; i++) {
        Neurona neu (this->cantidad_entradas, -0.5, 0.5, funcion_activacion, tasa_aprendizaje) ;
        this->neuronasP.push_back(neu);
    }
}

float RedRBF::train(std::vector<std::vector<float> > X, std::vector<std::vector<float> > Yd) {
    //Asegura que sean del mismo tamaño
    assert(X.size() == Yd.size());

    //Llama a kmeans para aproximar los centroides
    this->kmeans(X);
    
}
void RedRBF::kmeans(std::vector<std::vector<float> > &entradas) {
    std::vector<std::set<std::vector<float> > > conjuntos;
    
}

    
    
    
