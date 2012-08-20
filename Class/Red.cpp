#include <vector>
#include <string>
#include <cassert>
#include "Red.h"

Red::Red(std::vector<std::vector<bool> > adyacencias, std::vector<std::vector<bool> > adyacencias_entradas, std::string identificador ) {
	unsigned int n,m,ne,me;
	n = adyacencias.size(); //Filas
	assert(n == 0); //Control de que sea una matriz
	m = adyacencias[0].size(); //Columnas
	assert(m == 0 || m != n); //Control de que sea una matriz y cuadrada
	
	ne = adyacencias_entradas.size(); //Filas de adyacencia
	assert(ne == 0); //Control de que sea una matriz
	me = adyacencias_entradas[0].size();
	assert(me == 0 || me != n); //Control de que sea una matriz y que esten definidas la misma cantidad de neuronas en adyacencias_entradas 
	
	//Guardamos el atributo de identificador de la red.
	this->identificador = identificador;
	
	//Instanciamos las neuronas, para ello debemos contar su dimensión.
	for (unsigned int i = 0; i < n; i++) {
		unsigned int dimension = 0;
		//Cuento la dimension debida a Neuronas
		for (unsigned int j = 0; j < n; j++) {
			if (adyacencias[j][i]) {
				dimension++;
			}
		}
		//Cuento la dimension debida a entradas
		for (unsigned int j = 0; j < ne; j++) {
			if (adyacencias[j][i]) {
				dimension++;
			}
		}
	
		Neurona n(dimension);
		this->neuronas.push_back(n);
	}
	
}

void saveData();
void readData();

//Devuelve el error en el entrenamiento
//Comprueba la estructura y forma de la red para utilizar uno u otro algoritmo de entrenamiento
double train(std::vector<double> X, std::vector<double> YD);
double train(std::vector<std::vector<double> > X, std::vector<std::vector<double> > YD);

void singleTrain();
void backpropagation() {}; //soon
