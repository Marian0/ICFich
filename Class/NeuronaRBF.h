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
	Neurona(unsigned int dim, float min = -0.5, float max= 0.5, unsigned int funcion = 1, float constante_aprendizaje = 0.5);

	// Devuelve la respuesta de la Neurona en base a la entrada especificada por parámetro	
	float getResponse(std::vector<float> X, float parametro_sigmoidea);

	//Obtiene por referencia el vector W
	std::vector<float> getW();
	//Define por referencia el vector W
	void setW(std::vector<float> & W);
	
	//Devuelve la constante de aprendizaje
	float getConstanteAprendizaje();

	//Obtiene por referencia el vector W
	std::vector<float> getWn_1();
};

#endif
