//Includes librerías C++
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cassert>

//Include clases
#include "NeuronaRBF.h"

#include "utils.h"

Neurona::NeuronaRBF( unsigned int dim ) {
	this->dimension = dim;
	//dim+1 porque una entrada pertenece al bias (umbral)
	for(unsigned int i = 0; i < dim; i++){
		mu.push_back(utils::randomDecimal(min,max));
	}
}

//Obtiene la salida de la neurona para una entrada dada
float NeuronaRBF::getResponse(std::vector<float> X){
	//Verifico dimensiones
	assert(X.size() == this->mu.size());

	//Calculo la resta de vectores
	std::vector<float> numerador;
	utils::vectorEscalar(this->mu, -1, numerador );
	utils::vectorSuma(numerador, X, numerador);

	float norma = utils::vectorNorma(numerador);
	norma = -1 * norma * norma;

	return exp(norma/ (2*this->sigma*this->sigma));
}

//Obtiene por referencia el vector W
std::vector<float> NeuronaRBF::getMu() {
	return this->mu;
}
//Define por referencia el vector W
void NeuronaRBF::setMu(std::vector<float> & W){
	this->mu = W;
}