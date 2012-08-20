//Includes librerías C++
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cassert>

//Include clases
#include "Neurona.h"

//Include funciones externas
template<typename T>
extern T randomDecimal(T a, T b);
extern double signo(double);
extern double sigmoidea(double);


Neurona::Neurona(unsigned int dim, double min = -0.5, double max = 0.5, unsigned int funcion = 1, double constante_aprendizaje = 0.5) {
	std::srand(time(0));
	dimension = dim;
	this.constante_aprendizaje = constante_aprendizaje;
	this.id_funcion_activacion = funcion;
	//dim+1 porque una entrada pertenece al bias (umbral)
	for(unsigned int i = 0; i < dim+1; i++){
		W.push_back(randomDecimal<double>(min,max));
	}
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

	switch(this.id_funcion_activacion) {
		case FUNCION_SIGNO: {
			result = signo(result);
			break;
		}
		case FUNCION_SIGMOIDEA:
		default:
			result = sigmoidea(result);
	}


	return result;
}
