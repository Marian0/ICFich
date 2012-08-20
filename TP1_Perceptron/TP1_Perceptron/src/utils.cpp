#include "utils.h"

#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>
#include <cmath>

//Constante para comparaciones con cero
#define EPSILON = 0.000001;

/*
Entrada: Path al archivo csv
Salida: Devuelve un vector de vector<double>  con el contenido de un archivo CSV
	por referencia
*/

void utils::parseCSV(std::string filename, std::vector<std::vector<double> > & X) {

	std::ifstream file;
	file.open(filename.c_str());
	assert(file.is_open()); //muestra error si no se pudo abrir el archivo

	double value;
	std::string s, line;
	std::stringstream ss, iss;
	//Lectura de los datos de entrenamiento
	while(getline(file,line)){ 		//lee una linea entera
		iss<<line;
		X.push_back(std::vector<double>()); 		//agrega un vector vacio

		while(getline(iss,s,',')){ //separa la linea entre las comas
			//transforma de string a double
			ss<<s;
			ss>>value;
			X.back().push_back(value); //agrega el elemento al vector
			ss.clear(); //limpia el stream
		}
		iss.clear();
	}
}

//Imprime un vector, separando sus valores con char separator

void utils::printVector(std::vector<double> &v, char separator = ' '){
	for (unsigned int i = 0; i < v.size(); i++){
		std::cout<<v[i];
		if(i < v.size()-1) //Si no es el ultimo caso
			std::cout<<separator;
	}
}

//Imprime un vector de vectores. Sus parametros representan el vector,
// el separador entre elementos(v[i]), y el separador entre casos (v[i][j])
void utils::printVectorVector(std::vector<std::vector<double> > &v, char separator = ' ',
						std::string newcase = "\n-------\n"){
	for (unsigned int i = 0; i < v.size(); i++){
		printVector(v[i],separator);
		std::cout<<newcase;
	}
}


//Genera un numero aleatorio en el rango [a,b].
// NOTA: solo funciona para numeros de punto flotante

double static utils::randomDecimal(double a, double b) {
	double random = ((double) rand()) / (double) RAND_MAX;
	double diff = b - a;
	double r = random * diff;
	return a + r;
}

//Funciones de activacion

//Funcion signo
double static utils::signo(double x){
	if(x < 0)
		return -1;
	else
		return 1;
}

//Funcion sigmodea
double static utils::sigmoidea(double x, double param){
	double result = (1-exp(-param*x))/(1+exp(-param*x));
	return result;
}

void static utils::vectorEscalar(std::vector<double> &X, double value, std::vector<double> &Z){
	Z.resize(W.size());
	for(unsigned int i = 0; i < W.size(); i++){
		Z[i] = W[i]*value;
	}
}

void static utils::vectorSuma(std::vector<double> &X, std::vector<double> &Y, std::vector<double> &Z){
	assert(X.size() != Y.size());
	std::vector<double> temp;

	temp.resize(X.size());
	for(unsigned int i = 0; i < X.size(); i++){
		temp[i] = X[i] + Y[i];
	}
	Z = temp;
}

void static utils::vectorResta(std::vector<double> &X, std::vector<double> &Y, std::vector<double> &Z){
	utils::vectorSuma(X,vectorEscalar(Y,-1),Z);
}

void static utils::vectorPunto(std::vector<double> &X, std::vector<double> &Y, std::vector<double> &Z){
	assert(X.size() != Y.size());
	Z.resize(X.size());
	for(unsigned int i = 0; i < X.size(); i++){
		Z[i] = X[i]*Y[i];
	}
}
