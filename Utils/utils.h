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

void static parseCSV(std::string filename, std::vector<std::vector<double> > & X);
	
void static saveCSV(std::string filename, std::vector<std::vector<double> > & X);

//Imprime un vector, separando sus valores con char separator
void static printVector(std::vector<double> &v, char separator = ' ');

//Imprime un vector de vectores. Sus parametros representan el vector,
// el separador entre elementos(v[i]), y el separador entre casos (v[i][j])
void static printVectorVector(std::vector<std::vector<double> > &v, char separator = ' ', 
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
void static vectorResta(std::vector<double> &X, std::vector<double> &Y, std::vector<double> &Z);

//Producto punto entre dos vectores
void static vectorPunto(std::vector<double> &X, std::vector<double> &Y, std::vector<double> &Z);

//Calcula la norma de un vector
double static vectorNorma(std::vector<double> &X);

//Se parar un vector en 2
void static splitVector( std::vector<std::vector<double> > &V, std::vector<std::vector<double> > &X, std::vector<std::vector<double> > &Y, unsigned int size_y = 1);

//Devuelve la cantidad de patrones especificados con el desvio especificado tomando como base el patron P.
std::vector<std::vector<double> > static genPatrones( std::vector<std::vector<double> > & P, unsigned int cantidad_final, double desvio, unsigned int size_y = 1);

//En base a los patrones pasados por referencia, el % de entrenamiento, prueba y validacion, genera las particiones aleatorias correspondientes
void static genParticiones(
		std::vector<std::vector<double> > P, 
	    std::vector<std::vector<double> > & Entrenamiento,
		std::vector<std::vector<double> > & Validacion,
		std::vector<std::vector<double> > & Prueba,
		unsigned int porcentaje_entrenamiento,
		unsigned int porcentaje_prueba);

//Funciones de conversion de tipos
//Convierte un double a string
std::string static doubleToStr(double & input);

//Convierte un int a string
std::string static intToStr(int input);

//Convierte un string a int
int static strToInt(std::string s);

//Convierte un string a double
double static strToDouble(std::string s);

};
#endif
