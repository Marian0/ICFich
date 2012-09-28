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
	this->tau1 = 1000 / log(sigma0); //Haykin pag 453
	this->tau2 = tau2;
	this->iteracion = 0;
	this->dimension = dimension;

    std::cout<<"\nEstructura de la Red SOM:\n";
    std::cout<<"Alto x ancho = "<<this->alto<<" x "<<this->ancho<<'\n';
    std::cout<<"Sigma0 = "<<this->sigma0<<"\t Eta0 = "<<this->cte_aprendizaje0<<'\n';
    std::cout<<"tau1 = "<<this->tau1<<"\t Tau2 = "<<this->tau2<<'\n';
    std::cout<<"Cantidad de Entradas = "<<this->dimension<<'\n';

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
	this->cte_aprendizaje = this->cte_aprendizaje0 * exp( (-1.0 * this->iteracion) / this->tau2 );
	this->sigma = this->sigma0 * exp(  (-1.0 * this->iteracion) / this->tau1 );

	for (unsigned int i = 0; i < npatrones; i++) {
		this->singleTrain(X[i], YD[i], entrena);
	}

    this->iteracion++; //aumento la cantidad de iteraciones para cambiar los valores la proxima vez
    return 1.0;
}


std::vector<unsigned int> RedSOM::singleTrain(std::vector<float> X, std::vector<float> YD, bool entrena) {

	//Inicializo para encontrar la menor distancia W(neurona) <-> Patron(X)
	unsigned int iactivacion = 0;
	unsigned int jactivacion = 0;
	float dmenor = this->neuronas[iactivacion][jactivacion].getDistancia(X);

	//Encuentro la neurona de menor distancia
	for (unsigned int i = 0; i < this->alto ; i++) {
		for (unsigned int j = 0; j < this->ancho; j++) {
            //obtengo la distancia de la neurona i,j al patron X
			float distancia = this->neuronas[i][j].getDistancia(X);
            //Si esta distancia es menor a la que ya tengo almacenada
			if (distancia < dmenor) {
				dmenor = distancia;
				iactivacion = i;
				jactivacion = j;
			}
		}
    }
   
    if (entrena) {

        //Guardo la i,j de activacion en un vector
        std::vector<float> v1; //j = x, i = y
        v1.push_back(jactivacion);
        v1.push_back(iactivacion);
        
        for (unsigned int i = 0; i < this->alto; i++) {
            for (unsigned int j = 0; j < this->ancho; j++) {
                //creo un vector con los i,j actuales
                std::vector<float> v2;
                v2.push_back(j);
                v2.push_back(i);
                //calculo la distancia de v2 a v1
                float distancia = utils::vectorDistancia(v1,v2);
                
                //Calculo el coeficiente de vecindario
                float coef_vecindario = this->vecindario(distancia);
                
                //Acualizamos los pesos de la red
                //W_j(n+1) = W_j(n) + eta*vecindario*(x - W_j)
                std::vector<float> Wactual = this->neuronas[i][j].getW();
                std::vector<float> dif_X_W;

                utils::vectorResta(X, Wactual, dif_X_W);

                //Definimos la parte escalar
                float parte_escalar   = coef_vecindario*this->cte_aprendizaje;
                
                //std::cout<<i<<' '<<j<<':'; std::cout<<coef_vecindario<<'\n';
                //Calculamos el segundo miembro = eta*vecindario*(X - W_j)
                std::vector<float> segundo_miembro;
                utils::vectorEscalar(dif_X_W, parte_escalar, segundo_miembro);

                //Realizamos la suma del peso actual mas la correccion
                std::vector<float> Wnuevo;
                utils::vectorSuma(Wactual, segundo_miembro, Wnuevo);

                //Actualizamos los pesos de la neurona i,j
                this->neuronas[i][j].setW(Wnuevo);
       
            }
        }
    }
/*
    if (entrena) {
        //Acualizamos los pesos de la red
        //W_j(n+1) = W_j(n) + eta*vecindario*(x - W_j)
        std::vector<float> Wactual = this->neuronas[i][j].getW();
        std::vector<float> dif_X_W;

        utils::vectorResta(X, Wactual, dif_X_W);

        //Definimos la parte escalar
        float coef_vecindario = this->vecindario(distancia);
        float parte_escalar   = coef_vecindario*this->cte_aprendizaje;
        
        //std::cout<<i<<' '<<j<<':'; std::cout<<coef_vecindario<<'\n';
        //Calculamos el segundo miembro = eta*vecindario*(X - W_j)
        std::vector<float> segundo_miembro;
        utils::vectorEscalar(dif_X_W, parte_escalar, segundo_miembro);

        //Realizamos la suma del peso actual mas la correccion
        std::vector<float> Wnuevo;
        utils::vectorSuma(Wactual, segundo_miembro, Wnuevo);

        //Actualizamos los pesos de la neurona i,j
        this->neuronas[i][j].setW(Wnuevo);
    }
*/
    //Guardamos el output
    std::vector<unsigned int> output;
	output.push_back(jactivacion);
	output.push_back(iactivacion);

	return output;
}

//Calcula la funcion vecindario de la red SOM
float RedSOM::vecindario(float distancia) {
	return exp( (distancia*distancia)  / (-2.0 * this->sigma * this->sigma ));
}

//Devuelve un conjunto de coordenadas en el espacio de los patrones de todas las neuronas
void RedSOM::getPuntos(std::vector<std::vector<float> > & X){
    X.clear();
    for (unsigned int i = 0; i < this->alto; i++) {
        for (unsigned int j = 0; j < this->ancho ; j++ ) {
            std::vector<float> V = this->neuronas[i][j].getW();
            X.push_back(V);
        }
    }
}
