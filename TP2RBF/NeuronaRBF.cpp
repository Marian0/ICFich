/*
 * =====================================================================================
 *
 *       Filename:  NeuronaRBF.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  30/09/12 00:06:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "NeuronaRBF.h"
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include "utils.h"
#include <cstdio>
NeuronaRBF::NeuronaRBF(unsigned int dim) {
    this->dimension = dim;
    for (unsigned int i = 0; i < this->dimension; i++) {
        float ran_num = (rand()%2001 - 1000) / 1000.0 ;
        this->media.push_back(ran_num);
    }
}

float NeuronaRBF::getResponse(std::vector<float> X) {
    assert(X.size() == this->media.size());

    //V guardara la resta entre X y la media
    std::vector<float> V;
    V.resize(X.size());
    //obtengo el vector resta: x_l - mu_j
    for (unsigned int i = 0; i < X.size(); i++) {
        V[i] = X[i] - this->media[i];
    }

    //obtengo la norma al cuadrado del vector V (suma de las componentes al cuadrado)
    float num = 0.0;
    for (unsigned int i = 0; i < V.size(); i++) {
        num += V[i]*V[i];
    }
   
    //calculo la respuesta total de esta neurona:
    // \phi_j (x_l) = exp( - (\norm x_l - \mu_j \norm) ^2 / (2 * \sigma_j^2))
    float den = 2.0*this->varianza;
   
    /*
    std::cout<<"Entrada = "; utils::printVector(X);
    std::cout<<"Media = "; utils::printVector(this->media);
    std::cout<<"V = "; utils::printVector(V);
    std::cout<<num<<'/'<<den<<'\n';
    std::getchar();
    */
    float respuesta = exp(-num/den);
    if (respuesta < 0.0001) return 0.0;
    else return respuesta;
}

void NeuronaRBF::setMedia(std::vector<float> new_media) {
    this->media = new_media;
}

void NeuronaRBF::setVarianza(float new_var) {
    this->varianza = new_var;
}

float NeuronaRBF::distancia(std::vector<float> X) {
    float dist = 0.0;
    for (unsigned int i = 0; i < X.size(); i++) {
        float delta = X[i] - this->media[i];
        dist += delta*delta;
    }
    return dist;
}


std::vector<float> NeuronaRBF::getMedia() {
    return this->media;
}

float NeuronaRBF::getVarianza() {
    return this->varianza;
}
