//Includes librerías C++
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cassert>

//Include clases
#include "Neurona.h"

#include "utils.h"

Neurona::Neurona(unsigned int dim, float min, float max, unsigned int funcion, float constante_aprendizaje) {
	this->dimension = dim;
	this->constante_aprendizaje = constante_aprendizaje;
	this->id_funcion_activacion = funcion;
	//dim+1 porque una entrada pertenece al bias (umbral)
	for(unsigned int i = 0; i < dim+1; i++){
		W.push_back(utils::randomDecimal(min,max));
		Wn_1.push_back(0.0); //agrego ceros para que no intervenga en la 1er iteracion
	}
}

//Obtiene la salida de la neurona para una entrada dada
float Neurona::getResponse(std::vector<float> X, float parametro_sigmoidea){
	//Si el tamaño no coincide, cancela la ejecucion y muestra un mensaje 
	// de error informativo
	assert(X.size() == dimension);
	
	float result = -1*W[0]; //bias
	//Realiza el producto punto entre la entrada y los pesos
	for(unsigned int i = 0; i < X.size(); i++){
		result += W[i+1] * X[i];
	}

	switch(this->id_funcion_activacion) {
		case FUNCION_SIGNO: {
			result = utils::signo(result);
			break;
		}
		case FUNCION_SIGMOIDEA:
		default:
			result = utils::sigmoidea(result, parametro_sigmoidea);
	}
	
	return result;
}


//Devuelve la constante de aprendizaje
float Neurona::getConstanteAprendizaje() {
	return this->constante_aprendizaje;
}


//Obtiene por referencia el vector W
std::vector<float> Neurona::getW() {
	return this->W;
}
//Define por referencia el vector W
void Neurona::setW(std::vector<float> & W){
	this->Wn_1 = this->W; //Guardamos una iteración de pesos
	this->W = W;
}


//Obtiene por referencia el vector W
std::vector<float> Neurona::getWn_1(){
	return this->Wn_1;
}	
