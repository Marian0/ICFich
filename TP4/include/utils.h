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
#include <utility>
#include "GNUPlot.h"

class utils{
public:
//Representacion de un numero cercano a cero
unsigned int const static EPSILON = 0.0000001;

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

//Imprime un vector, separando sus valores con char separator
void static printVector(std::vector<int> &v, char separator = ' ');

//Imprime un vector, separando sus valores con char separator
void static printVector(std::vector<bool> &v, char separator = ' ');

//----------------------------------
//Funciones de generacion de numeros aleatorios

//Genera un numero aleatorio en el rango [a,b].
float static randomDecimal(float a, float b);


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


void static drawPoints(std::vector<std::vector<float> > &V, GNUPlot &plotter, unsigned int color = 2, unsigned int tipo = 5);

unsigned int static getMinIdx(std::vector<float> &V );

//Convierte un vector de 1 y 0 a un entero, si signo=true => El primer bit indica el signo
int static binary2int(std::vector<bool> &V, bool signo=false);

//convierte un entero a un vector de 1 y 0. Si signo es true, agrega un 1 sii value es negativo. Si no, agrega un cero
std::vector<bool> static int2binary(int value, bool signo);

//Convierte a bin, un vector de binarios representando muchas variables, en un vector de enteros.
// Cada variable tiene asignado tantos bits como dice paso
void static vectorBinary2Int(std::vector<bool> &bin, std::vector<int> &res, unsigned int paso);

//--------------------------
//Funciones de fitness
//-x*sin(sqrt(abs(x))
float static fitness_1a(float value);
//(x + 5sin(3x) + 8cos(5x));
float static fitness_1b(float value);
//(x^2+y^2)^0.25 * sin( 50((x^2+y^2)^0.1)+1)
float static fitness_1c(float x, float y);



//----------------


//graficar historiales
void static drawHistory(std::vector<std::vector<float> > &historial, GNUPlot &plotter, unsigned int id_ejercicio);
};
#endif
