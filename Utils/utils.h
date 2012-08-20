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
class utils{
public:

void parseCSV(std::string filename, std::vector<std::vector<double> > & X);

//Imprime un vector, separando sus valores con char separator
void printVector(std::vector<double> &v, char separator = ' ');

//Imprime un vector de vectores. Sus parametros representan el vector,
// el separador entre elementos(v[i]), y el separador entre casos (v[i][j])
void printVectorVector(std::vector<std::vector<double> > &v, char separator = ' ', 
	std::string newcase = "\n-------\n");


//Genera un numero aleatorio en el rango [a,b].
// NOTA: solo funciona para numeros de punto flotante
double static randomDecimal(double a, double b);


//Funciones de activacion

//Funcion signo
double static signo(double x);

//Funcion sigmodea
double static sigmoidea(double x, double param = 0.5);


//Funciones de Algebra de Vectores

//Producto de un vector por un escalar
void static vectorEscalar(std::vector<double> &X, double value, std::vector<double> &Z);

//Suma de dos vectores
void static vectorSuma(std::vector<double> &X, std::vector<double> &Y, std::vector<double> &Z);

//Resta de dos vectores
void vectorResta(std::vector<double> &X, std::vector<double> &Y, std::vector<double> &Z);

//Producto punto entre dos vectores
void vectorPunto(std::vector<double> &X, std::vector<double> &Y, std::vector<double> &Z);


};
#endif
