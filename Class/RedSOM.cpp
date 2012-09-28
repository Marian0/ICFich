#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <algorithm>

#include "utils.h"
#include "RedSOM.h"


RedSOM::RedSOM(unsigned int dimension, unsigned int alto, unsigned int ancho, float sigma0, float cte_aprendizaje0, float tau2 ){
	//Verificamos que las dimensiones sean mayores a cero
	assert(alto > 0 && ancho > 0);

	//Inicializamos atributos y matriz de neuronas (mapa)
	this->alto = alto;
	this->ancho = ancho;

	this->sigma0 = sigma0;
	this->sigma = sigma0;
	this->cte_aprendizaje0 = cte_aprendizaje0;
	this->cte_aprendizaje = cte_aprendizaje0;
	this->tau1 = 1000 / log(sigma0); //pag 453
	this->tau2 = tau2;
	this->iteracion = 0;
	this->dimension = dimension;

	for (unsigned int i = 0; i < this->alto ; i++) {
		std::vector<NeuronaSOM> filaNeurona;
		for (unsigned int j = 0; j < this->ancho; j++) {
			filaNeurona.push_back(  NeuronaSOM(dimension, -0.5, 0.5 ) );
		}
		this->neuronas.push_back(filaNeurona);
	}

}
float RedSOM::train(std::vector<std::vector<float> > X, std::vector<std::vector<float> > YD, bool entrena) {
	unsigned int npatrones = X.size();
	assert(npatrones == YD.size());
	//Calculo las constantes en base a la iteracion
	this->cte_aprendizaje = this->cte_aprendizaje0 * exp( (-1 * this->iteracion) / this->tau2 );
	this->sigma = this->sigma0 * exp(  (-1 * this->iteracion) / this->tau2 );

	for (unsigned int i = 0; i < npatrones; i++) {
		this->singleTrain(X[i], YD[i], entrena);
	}



}


std::vector<unsigned int> RedSOM::singleTrain(std::vector<float> X, std::vector<float> YD, bool entrena) {

	//Inicializo para encontrar la menor distancia W(neurona) <-> Patron(X)
	unsigned int iactivacion = 0;
	unsigned int jactivacion = 0;
	float dmenor = this->neuronas[iactivacion][jactivacion].getDistancia(X);

	//Encuentro la neurona de menor distancia
	for (unsigned int i = 0; i < this->alto ; i++) {
		for (unsigned int j = 0; j < this->ancho; j++) {
			float distancia = this->neuronas[i][j].getDistancia(X);
			if (distancia < dmenor) {
				dmenor = distancia;
				iactivacion = i;
				jactivacion = j;
			}
			if (entrena) {
				//Acualizamos los pesos de la red
				std::vector<float> Wactual = this->neuronas[i][j].getW();
				std::vector<float> dif_X_W;

				utils::vectorResta(X,Wactual, dif_X_W);

				float coef_vecindario = this->vecindario(distancia);
				coef_vecindario = coef_vecindario*this->cte_aprendizaje;

				std::vector<float> Wnuevo;
				utils::vectorEscalar(dif_X_W, coef_vecindario, Wnuevo);

				this->neuronas[i][j].setW(Wnuevo);
			}
		}
	}
	std::vector<unsigned int> output;
	output.push_back(jactivacion);
	output.push_back(iactivacion);

	return output;
}


float RedSOM::vecindario(float distancia) {
	return exp( (distancia*distancia)  / (-1 * this->sigma * this->sigma ));
}