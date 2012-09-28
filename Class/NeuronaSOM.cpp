//Includes de C
#include <vector>
#include <cassert>

//Includes de otras clases
#include "NeuronaSOM.h"
#include "utils.h"

//Constructor
NeuronaSOM::NeuronaSOM(unsigned int dim, float min, float max) {
    this->dim = dim;

    //Iniciamos aleatoriamente los pesos
    for (unsigned int i = 0; i < this->dim; i++) {
        this->W.push_back(utils::randomDecimal(min, max));
    }
}

//Calcula la distancia entre esta neurona y el patron X
float NeuronaSOM::getDistancia(std::vector<float> X) {
    //Compruebo que tengan la misma dimension
    assert(X.size == this->W.size());

    //Calcula la distancia
    return utils::vectorDistancia(this->W, X);
}

std::vector<float> NeuronaSOM::getW() {
    return this->W;
}

void NeuronaSOM::setW(std::vector<float> newW) {
    this->W = newW;
}
