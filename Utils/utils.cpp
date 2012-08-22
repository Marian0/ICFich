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
		
		//std::cout<<iss.str()<<std::endl;
		if(!iss.str().empty() && iss.str()[0] == '#'){
			//comentario, no lo procesa pero si lo imprime
			std::cout<<iss.str()<<std::endl;
			iss.str(""); //limpia la string asociada
			iss.clear(); //limpia el stream
			continue;
		}
		
		X.push_back(std::vector<double>()); 		//agrega un vector vacio	
		while(getline(iss,s,',')){ //separa la linea entre las comas
			//transforma de string a double
			ss<<s;
			ss>>value;
			X.back().push_back(value); //agrega el elemento al vector
			ss.str(""); //limpia la string asociada
			ss.clear(); //limpia el stream
		}
		iss.str(""); //limpia la string asociada
		iss.clear(); //limpia el stream
	}
	file.close();
}

//Guarda la matriz pasada por parametro en un archivo CSV

void utils::saveCSV(std::string filename, std::vector<std::vector<double> > & X) {
	std::ofstream file (filename.c_str(), 
		std::ofstream::out|std::ofstream::trunc); //trunc explicito para borrar contenido
	
	assert(file.is_open()); //muestra error si no se pudo abrir el archivo
	
	unsigned int n = X.size();
	
	for (unsigned int i = 0 ; i < n ; i++) {
		unsigned int m = X[i].size();
		
		for (unsigned int j = 0 ; j < m ; j++) {
			file<<X[i][j];
			if (j+1 < m)
				file<<", ";
		}
		file<<std::endl;
	}
	file.close();
}

//Imprime un vector, separando sus valores con char separator
void utils::printVector(std::vector<double> &v, char separator){
	for (unsigned int i = 0; i < v.size(); i++){
		std::cout<<v[i];
		if(i < v.size()-1) //Si no es el ultimo caso
			std::cout<<separator;
	}
}

//Imprime un vector de vectores. Sus parametros representan el vector,
// el separador entre elementos(v[i]), y el separador entre casos (v[i][j])
void utils::printVectorVector(std::vector<std::vector<double> > &v, char separator, 
						std::string newcase){
	for (unsigned int i = 0; i < v.size(); i++){
		utils::printVector(v[i],separator);
		std::cout<<newcase;
	}
}


//Genera un numero aleatorio en el rango [a,b].
// NOTA: solo funciona para numeros de punto flotante

double utils::randomDecimal(double a, double b) {
//    std::srand(time(NULL));
	double random = ((double) rand()) / (double) RAND_MAX;
	double diff = b - a;
	double r = random * diff;
	return a + r;
}

//Funciones de activacion

//Funcion signo
double utils::signo(double x){
	if(x < 0)
		return -1;
	else
		return 1;
}

//Funcion sigmodea
double utils::sigmoidea(double x, double param){
	double result = (1-exp(-param*x))/(1+exp(-param*x));
	return result;
}

void utils::vectorEscalar(std::vector<double> &X, double value, std::vector<double> &Z){
	Z.resize(X.size());
	for(unsigned int i = 0; i < X.size(); i++){
		Z[i] = X[i]*value;
	}
}

void utils::vectorSuma(std::vector<double> &X, std::vector<double> &Y, std::vector<double> &Z){
	assert(X.size() == Y.size());
	std::vector<double> temp;
	
	temp.resize(X.size());
	for(unsigned int i = 0; i < X.size(); i++){
		temp[i] = X[i] + Y[i];
	}
	Z = temp;
}

void utils::vectorResta(std::vector<double> &X, std::vector<double> &Y, std::vector<double> &Z){
	std::vector<double> temp;
	utils::vectorEscalar(Y,-1,temp);
	utils::vectorSuma(X, temp ,Z);
}

void utils::vectorPunto(std::vector<double> &X, std::vector<double> &Y, std::vector<double> &Z){
	assert(X.size() == Y.size());
	Z.resize(X.size());
	for(unsigned int i = 0; i < X.size(); i++){
		Z[i] = X[i]*Y[i];
	}
}

std::string utils::doubleToStr(double & input) {
	std::stringstream str;
	str<<input;
	return str.str();
}


//Separar un vector en 2
void  utils::splitVector( std::vector<std::vector<double> > &V, std::vector<std::vector<double> > &X, std::vector<std::vector<double> > &Y, unsigned int size_y) {
	//Limpiamos por las dudas
	X.clear();
	Y.clear();
	
	unsigned int n = V.size();
	
	for (unsigned int i = 0 ; i < n; i++) {
		unsigned int m = V[i].size();
		assert(m > size_y); //Sino, para que lo queres dividir?
		std::vector<double> Xtemp, Ytemp;
		for (unsigned int j = 0; j < m ; j++) {
			if (j < m-size_y) {
				Xtemp.push_back(V[i][j]);
			} else {
				Ytemp.push_back(V[i][j]);
			}
		}
		X.push_back(Xtemp);
		Y.push_back(Ytemp);
	}
	
}
