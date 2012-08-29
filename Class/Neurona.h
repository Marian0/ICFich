#ifndef __NEURONA_H__
#define __NEURONA_H__

#include <vector>

class Neurona {
	
private: 
	//Vector de pesos sinápticos
	std::vector<float> W;
	//Cantidad de entradas
	unsigned int dimension;
	//constante gamma de aprendizaje de la neurona
	float constante_aprendizaje;
	//Índice que determina la función de activación
	unsigned int id_funcion_activacion;

	//...

	
public:
	static const unsigned int FUNCION_SIGNO = 1;
	static const unsigned int FUNCION_SIGMOIDEA = 2;
	//Constructor
	//  * dim = Cantidad de entradas
	Neurona(unsigned int dim, float min = -0.5, float max= 0.5, unsigned int funcion = 1, float constante_aprendizaje = 0.5);

	// Devuelve la respuesta de la Neurona en base a la entrada especificada por parámetro	
	float getResponse(std::vector<float> X);

	//Obtiene por referencia el vector W
	std::vector<float> getW();
	//Define por referencia el vector W
	void setW(std::vector<float> & W);
	
	//Devuelve la constante de aprendizaje
	float getConstanteAprendizaje();
};

#endif
