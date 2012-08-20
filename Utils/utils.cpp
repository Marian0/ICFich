#ifndef __UTILS_CPP__
#define __UTILS_CPP__

#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib> 
#include <cassert>


/*
Entrada: Path al archivo csv
Salida: Devuelve un vector de vector<double>  con el contenido de un archivo CSV
	por referencia
*/

void parseCSV(std::string filename, std::vector<std::vector<double> > & X) {

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
template<typename T>
void printVector(std::vector<T> &v, char separator = ' '){
	for (unsigned int i = 0; i < v.size(); i++){
		std::cout<<v[i];
		if(i < v.size()-1) //Si no es el ultimo caso
			std::cout<<separator;
	}
}

//Imprime un vector de vectores. Sus parametros representan el vector,
// el separador entre elementos(v[i]), y el separador entre casos (v[i][j])
template<typename T>
void printVectorVector(std::vector<std::vector<T> > &v, char separator = ' ', 
						std::string newcase = "\n-------\n"){
	for (unsigned int i = 0; i < v.size(); i++){
		printVector(v[i],separator);
		std::cout<<newcase;
	}
}


//Genera un numero aleatorio en el rango [a,b].
// NOTA: solo funciona para numeros de punto flotante
template<typename T>
T randomDecimal(T a, T b) {
	T random = ((T) rand()) / (T) RAND_MAX;
	T diff = b - a;
	T r = random * diff;
	return a + r;
}

//Funciones de activacion

//Funcion signo
double signo(double x){
	if(x < 0)
		return -1;
	else
		return 1;
}

//Funcion sigmodea
double sigmodea(double x){
	//...
	return 1;
}

#endif
