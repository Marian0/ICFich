#ifndef __NEURONA_H__
#define __NEURONA_H__

#include <vector>

class Neurona {
	
private: 
	//Vector de pesos sinápticos
	std::vector<double> W;
	//Cantidad de entradas
	unsigned int dimension;
	//constante gamma de aprendizaje de la neurona
	double constante_aprendizaje;
	//Índice que determina la función de activación
	unsigned int id_funcion_activacion;

	//...

	
public:
	static const unsigned int FUNCION_SIGNO = 1;
	static const unsigned int FUNCION_SIGMOIDEA = 2;
	//Constructor
	//  * dim = Cantidad de entradas
	Neurona(unsigned int dim, double min, double max, unsigned int funcion, double constante_aprendizaje);

	// Devuelve la respuesta de la Neurona en base a la entrada especificada por parámetro	
	double getResponse(std::vector<double> X);

	//Obtiene por referencia el vector W
	void getW(std::vector<double> & W);
	//Define por referencia el vector W
	void setW(std::vector<double> & W);
};

#endif
