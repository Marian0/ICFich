#include <cmath>
#include "NeuronaPerceptron.h"
#include <cstdlib>
#include <cassert>
#include "utils.h"
#include <iostream>
NeuronaPerceptron::NeuronaPerceptron(unsigned int dim, float eta) {
    this->dimension = dim;
    this->eta = eta;
    for (unsigned int i = 0; i < this->dimension; i++) {
        float ran_num = (rand()%2001 - 1000) / 1000.0 ;
        this->W.push_back(ran_num);
    }

}

float NeuronaPerceptron::getResponse( std::vector<float> X) {
    float suma = 0.0;
    for (unsigned int i = 0; i < X.size(); i++) {
        suma += X[i]*this->W[i];
    }
    return utils::signo(suma);
}

float NeuronaPerceptron::update ( std::vector<float> X, float Yd) {
    assert(X.size() == this->dimension);
    float respuesta = getResponse(X);
    float error = Yd - respuesta;
    float escalar = eta*(error);

    for (unsigned int j = 0; j < X.size(); j++) {
        this->W[j] = this->W[j] + escalar*X[j];
    }
    
    return error;
}		
