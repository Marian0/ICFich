#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cassert>

#include "Neurona.h"
#include "utils.cpp"


Neurona::Neurona(unsigned int dim, double min = -0.5, double max = 0.5, unsigned int funcion = 1) {
	std::srand(time(0));
	dimension = dim;

	//dim+1 porque una entrada pertenece al bias (umbral)
	for(unsigned int i = 0; i < dim+1; i++){
		W.push_back(randomDecimal<double>(min,max));
	}
}


//Guardar pesos sinápticos
void Neurona::saveW() {
	
}

//Leer pesos sinápticos
void Neurona::readW() {
	
}

//Entrenar con un conjunto de entradas X y una salida deseada yd
void Neurona::train(std::vector<double> X, double yd) {
	
}

//Obtiene la salida de la neurona para una entrada dada
double Neurona::getResponse(std::vector<double> X){
	//Si el tamaño no coincide, cancela la ejecucion y muestra un mensaje 
	// de error informativo
	assert(X.size() != dimension);
	
	double result = -1*W[0]; //bias
	//Realiza el producto punto entre la entrada y los pesos
	for(unsigned int i = 0; i < X.size(); i++){
		result += W[i+1] * X[i];
	}
	return result;
}
