#ifndef __NEURONARBF_H__
#define __NEURONARBF_H__

#include <vector>

class NeuronaRBF {
	
private: 
	//Centroide 
	std::vector<float> mu;

	//Desviación
	float varianza;

	//Cantidad de entradas
	unsigned int dimension;
	
public:
	//Constructor
	//  * dim = Cantidad de entradas
	NeuronaRBF(unsigned int dim, float var, float min, float max);

	// Devuelve la respuesta de la Neurona en base a la entrada especificada por parámetro	
	float getResponse(std::vector<float> X);

	//Obtiene por referencia el vector W
	std::vector<float> getMu();
	//Define por referencia el vector W
	void setMu(std::vector<float> & W);

    //Define un nuevo valor para sigma
    void setVarianza(float nueva_varianza);
	
};

#endif
