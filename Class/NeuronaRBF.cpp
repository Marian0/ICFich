//Includes librerías C++
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cassert>

//Include clases
#include "NeuronaRBF.h"

#include "utils.h"

NeuronaRBF::NeuronaRBF( unsigned int dim, float var, float min, float max) {
	this->dimension = dim;
	for(unsigned int i = 0; i < dim; i++){
		mu.push_back(utils::randomDecimal(min,max));
	}
    this->varianza = var;
}

//Obtiene la salida de la neurona para una entrada dada
float NeuronaRBF::getResponse(std::vector<float> X){
	//Verifico dimensiones
	
    assert(X.size() == this->mu.size());

	//Calculo la resta de vectores
	std::vector<float> numerador;
    utils::vectorResta(X, this->mu, numerador);

	float norma = utils::vectorNorma(numerador);
    
    float num = -1.0 * norma * norma;
    float den = 2.0*this->varianza;
	float resp =  exp(num/den);

    return resp;
}

//Obtiene por referencia el vector W
std::vector<float> NeuronaRBF::getMu() {
	return this->mu;
}
//Define por referencia el vector W
void NeuronaRBF::setMu(std::vector<float> & W){
	this->mu = W;
}

void NeuronaRBF::setVarianza(float nueva_varianza) {
    this->varianza = nueva_varianza;
}


