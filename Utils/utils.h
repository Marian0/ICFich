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


class utils{
public:
//Representacion de un numero cercano a cero
unsigned int const static EPSILON = 0.000001;
//Funciones de trabajo con archivos

//Lee el contenido de un archivo y lo guarda en un vector
void static parseCSV(std::string filename, std::vector<std::vector<float> > & X);

//Guarda el contenido de un vector en un archivo
void static saveCSV(std::string filename, std::vector<std::vector<float> > & X);


//----------------------------------
//Funciones de impresion de vectores

//Imprime un vector, separando sus valores con char separator
void static printVector(std::vector<float> &v, char separator = ' ');

//Imprime un vector de vectores. Sus parametros representan el vector,
// el separador entre elementos(v[i]), y el separador entre casos (v[i][j])
void static printVectorVector(std::vector<std::vector<float> > &v, char separator = ' ', 
	std::string newcase = "\n-------\n");

//Imprime un vector, separando sus valores con char separator
void static printVector(std::vector<unsigned int> &v, char separator = ' ');

//Imprime un vector de vectores. Sus parametros representan el vector,
// el separador entre elementos(v[i]), y el separador entre casos (v[i][j])
void static printVectorVector(std::vector<std::vector<unsigned int> > &v, char separator = ' ', 
	std::string newcase = "\n-------\n");

//----------------------------------
//Funciones de generacion de numeros aleatorios

//Genera un numero aleatorio en el rango [a,b].
float static randomDecimal(float a, float b);

//Genera una posicion aleatoria en forma circular centrada en (xi,yi) y con el desvio dado
void static randomCircular(float xi, float yi, float desvio, float & newx, float & newy);


//----------------------------------
//Funciones de activacion

//Funcion signo
float static signo(float x);

//Funcion sigmodea
float static sigmoidea(float x, float param = 5);

//Funcion derivada de sigmoidea
float static sigmoideaPrima(float x, float param = 5);

//----------------------------------
//Funciones de Algebra de Vectores

//Producto de un vector por un escalar
void static vectorEscalar(std::vector<float> &X, float value, std::vector<float> &Z);

//Suma de dos vectores
void static vectorSuma(std::vector<float> &X, std::vector<float> &Y, std::vector<float> &Z);

//Resta de dos vectores
void static vectorResta(std::vector<float> &X, std::vector<float> &Y, std::vector<float> &Z);

//Producto punto entre dos vectores
float static vectorPunto(std::vector<float> &X, std::vector<float> &Y);

//Producto punto entre dos vectores
bool static vectorParecido(std::vector<float> &X, std::vector<float> &Y, float criterio = 0.6);

//Calcula la norma de un vector
float static vectorNorma(std::vector<float> &X);

//Se parar un vector en 2
void static splitVector( std::vector<std::vector<float> > &V, std::vector<std::vector<float> > &X, std::vector<std::vector<float> > &Y, unsigned int size_y = 1);


//---------------------------------
//Funciones para la creacion de conjuntos y subconjuntos

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

//Genera un conjunto a partir de una posicion inicial y de un tamanio dado
std::vector<std::vector<float> > static genSet( std::vector<std::vector<float> > P,
                                                 unsigned int tamanio, unsigned int inicio);

//Convierte un entero a un array de -1 y 1
std::vector<float> static int2binary(unsigned int val, unsigned int max_clases);

//Convierte unos valores {0, 1, 2, 3...} en una codificacion {[1,-1,-1,-1,...], [-1,1,-1,-1,...], ... }
void static convertirEntrada(std::vector<std::vector<float> > &in, std::vector<std::vector<float> > &out);


//--------------------------------
//Funciones de conversion de tipos

//Convierte un float a string
std::string static floatToStr(float input);

//Convierte un int a string
std::string static intToStr(int input);

//Convierte un string a int
int static strToInt(std::string s);

//Convierte un string a float
float static strToFloat(std::string s);

//-------------------------------
//Funciones estadisticas

//Calcula el promedio de un vector
float static promedio(std::vector<float> &V);

//Calcula la desviación estandar
float static devest(std::vector<float> &V, float media);

//Funcion para graficar el conjuto de patrones
void static drawPlot(
		std::vector<std::vector<float> > & X,
		std::vector<std::vector<float> > & YD,
		std::vector<std::vector<float> > & YC,
		GNUPlot & plotter
	);

//Funcion que toma un vector de valores, los interpreta binarios y lo transforma a entero
unsigned int static binary2int( std::vector<float> & input );

};
#endif
