#ifndef UTILS_H
#define UTILS_H

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

void parseCSV(std::string filename, std::vector<std::vector<double> > & X);

//Imprime un vector, separando sus valores con char separator
template<typename T>
void printVector(std::vector<T> &v, char separator = ' ');

//Imprime un vector de vectores. Sus parametros representan el vector,
// el separador entre elementos(v[i]), y el separador entre casos (v[i][j])
template<typename T>
void printVectorVector(std::vector<std::vector<T> > &v, char separator = ' ', 
	std::string newcase = "\n-------\n");


//Genera un numero aleatorio en el rango [a,b].
// NOTA: solo funciona para numeros de punto flotante
template<typename T>
T randomDecimal(T a, T b);


//Funciones de activacion

//Funcion signo
double signo(double x);

//Funcion sigmodea
double sigmoidea(double x, double param = 0.5);




#endif
