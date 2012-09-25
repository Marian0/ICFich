#ifndef __NEURONARBF_H__
#define __NEURONARBF_H__

#include <vector>

class NeuronaRBF {
	
private: 
	//Centroide 
	std::vector<float> mu;

	//Desviación
	float sigma;

	//Cantidad de entradas
	unsigned int dimension;
	
public:
	static const unsigned int FUNCION_SIGNO = 1;
	static const unsigned int FUNCION_SIGMOIDEA = 2;
    
	//Constructor
	//  * dim = Cantidad de entradas
	NeuronaRBF(unsigned int dim, float sigma, float min, float max);

	// Devuelve la respuesta de la Neurona en base a la entrada especificada por parámetro	
	float getResponse(std::vector<float> X);

	//Obtiene por referencia el vector W
	std::vector<float> getMu();
	//Define por referencia el vector W
	void setMu(std::vector<float> & W);
	
};

#endif
