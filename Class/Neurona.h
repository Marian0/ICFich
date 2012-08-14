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

	//...

	
public:
	static const unsigned int FUNCION_SIGNO = 1;
	static const unsigned int FUNCION_SIGMODEA = 2;
	//Constructor
	//  * dim = Cantidad de entradas
	Neurona(unsigned int dim, double min, double max, unsigned int funcion);

	//Guardar pesos sinápticos
	void saveW();
	//Leer pesos sinápticos
	void readW();

	//Entrenar con un conjunto de entradas X y una salida deseada yd
	void train(std::vector<double> X, double yd);
	
	double getResponse(std::vector<double> X);

};

#endif
