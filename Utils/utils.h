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
Salida: Devuelve un vector de vector<float>  con el contenido de un archivo CSV
por referencia
*/
class utils{
public:

void static parseCSV(std::string filename, std::vector<std::vector<float> > & X);
	
void static saveCSV(std::string filename, std::vector<std::vector<float> > & X);

//Imprime un vector, separando sus valores con char separator
void static printVector(std::vector<float> &v, char separator = ' ');

//Imprime un vector de vectores. Sus parametros representan el vector,
// el separador entre elementos(v[i]), y el separador entre casos (v[i][j])
void static printVectorVector(std::vector<std::vector<float> > &v, char separator = ' ', 
	std::string newcase = "\n-------\n");


//Genera un numero aleatorio en el rango [a,b].
// NOTA: solo funciona para numeros de punto flotante
float static randomDecimal(float a, float b);
void static randomCircular(float, float, float, float&, float&);

//Funciones de activacion

//Funcion signo
float static signo(float x);

//Funcion sigmodea
float static sigmoidea(float x, float param = 50);


//Funciones de Algebra de Vectores

//Producto de un vector por un escalar
void static vectorEscalar(std::vector<float> &X, float value, std::vector<float> &Z);

//Suma de dos vectores
void static vectorSuma(std::vector<float> &X, std::vector<float> &Y, std::vector<float> &Z);

//Resta de dos vectores
void static vectorResta(std::vector<float> &X, std::vector<float> &Y, std::vector<float> &Z);

//Producto punto entre dos vectores
void static vectorPunto(std::vector<float> &X, std::vector<float> &Y, std::vector<float> &Z);

//Calcula la norma de un vector
float static vectorNorma(std::vector<float> &X);

//Se parar un vector en 2
void static splitVector( std::vector<std::vector<float> > &V, std::vector<std::vector<float> > &X, std::vector<std::vector<float> > &Y, unsigned int size_y = 1);

//Devuelve la cantidad de patrones especificados con el desvio especificado tomando como base el patron P.
std::vector<std::vector<float> > static genPatrones( std::vector<std::vector<float> > & P, unsigned int cantidad_final, float desvio, unsigned int size_y = 1);

//En base a los patrones pasados por referencia, el % de entrenamiento, prueba y validacion, genera las particiones aleatorias correspondientes
void static genParticiones(
		std::vector<std::vector<float> > P, 
	    std::vector<std::vector<float> > & Entrenamiento,
		std::vector<std::vector<float> > & Validacion,
		std::vector<std::vector<float> > & Prueba,
		unsigned int porcentaje_entrenamiento,
		unsigned int porcentaje_prueba,
		unsigned int indice_prueba);

std::vector<std::vector<float> > static genSet( std::vector<std::vector<float> > P,
                                                 unsigned int tamanio, unsigned int inicio);
//Funciones de conversion de tipos
//Convierte un float a string
std::string static floatToStr(float input);

//Convierte un int a string
std::string static intToStr(int input);

//Convierte un string a int
int static strToInt(std::string s);

//Convierte un string a float
float static strToFloat(std::string s);

//Calcula el promedio de un vector
float static promedio(std::vector<float> &V) {
};
#endif
