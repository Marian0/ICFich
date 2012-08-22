#include <vector>
#include <string>
#include <cassert>
#include "Red.h"
#include "utils.h"

Red::Red(std::vector<std::vector<bool> > adyacencias, std::vector<std::vector<bool> > adyacencias_entradas, std::string identificador ) {
	unsigned int n,m,ne,me;

    n = adyacencias.size();             //Filas
	assert(n > 0);                      //Control de que sea una matriz
	m = adyacencias[0].size();          //Columnas
	assert(m > 0 && m == n);            //Control de que sea una matriz y cuadrada
	
	ne = adyacencias_entradas.size();   //Filas de adyacencia
	assert(ne > 0);                     //Control de que sea una matriz
	me = adyacencias_entradas[0].size();
	assert(me > 0 && me == n);          //Control de que sea una matriz y que esten definidas 
                                        // la misma cantidad de neuronas en adyacencias_entradas 
	

    //Guardamos el atributo de identificador de la red.
	this->identificador = identificador;
	this->multicapa = false;
	this->adyacencias = adyacencias;
	this->adyacencias_entradas = adyacencias_entradas;

    //Instanciamos las neuronas, para ello debemos contar su dimensión.
	for (unsigned int i = 0; i < n; i++) {
		unsigned int dimension = 0;
		//Cuento la dimension debida a Neuronas
		for (unsigned int j = 0; j < n; j++) {
			if (adyacencias[j][i]) {
                //comprobar i != j ?
				dimension++;
				this->multicapa = true; //Detectamos que es multicapa ( ver despues, el tema de la identidad, osea que tiene una sola capa pero es recursivo)
			}
		}
		
		//Cuento la dimension debida a entradas
		for (unsigned int j = 0; j < ne; j++) {
			if (adyacencias_entradas[j][i]) {
				dimension++;
			}
		}
		Neurona neu(dimension, -0.5, 0.5, Neurona::FUNCION_SIGNO, 0.5);
		this->neuronas.push_back(neu);
	}
}

//Devuelve el error en el entrenamiento
//Comprueba la estructura y forma de la red para utilizar uno u otro algoritmo de entrenamiento
void Red::train(std::vector<double> X, std::vector<double> YD) {	
	if (this->multicapa) {
		return;
	} else {
		singleTrain(X,YD);
	}
}
void Red::train(std::vector<std::vector<double> > X, std::vector<std::vector<double> > YD){ }


void Red::singleTrain(std::vector<double> X, std::vector<double> YD) {
	unsigned int n = this->neuronas.size();
	
	unsigned int ne = this->adyacencias_entradas.size(); //Filas de adyacencia

	assert(ne == X.size()); //Verificamos que se envien la cantidad de entradas necesarias para el entrenamiento de la RED

    //Se recorre cada una de las neuronas
	for (unsigned int i = 0; i < n ; i++) {
		std::vector<double> entradan;
	    
        //Recorrer cada entrada y filtrar las entradas que van para cada neurona
		for (unsigned int j = 0; j < ne; j++) {
			if (this->adyacencias_entradas[j][i]) {
				entradan.push_back(X[j]);
			}
		}
		
        //Estimular una neurona y obtener su respuesta
		double respuesta = this->neuronas[i].getResponse(entradan);
		
		//Obtengo los pesos sinápticos actuales
		std::vector<double> Wi = this->neuronas[i].getW();
	
        //Calculo de los nuevos pesos

        //Parte Escalar
		respuesta = (YD[i] - respuesta) * ( this->neuronas[i].getConstanteAprendizaje()/2 );

        //Temporal para el producto 
        entradan.insert(entradan.begin(), -1);
		std::vector<double> vesc; 
		utils::vectorEscalar(entradan, respuesta, vesc);

        //Temporal para la suma
		std::vector<double> Wnuevo; 
		utils::vectorSuma(Wi, vesc, Wnuevo);

        //Actualizar pesos
		this->neuronas[i].setW( Wnuevo );
		
	}
}

void backpropagation() {}; //soon


void Red::getNeuronas(std::vector<Neurona> &N) {
	N = this->neuronas;
}
