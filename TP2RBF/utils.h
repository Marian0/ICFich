#ifndef __UTILS_H__
#define __UTILS_H__
#include <vector>
#include <string>
#include <cmath>

class utils {
public:
    void static parseCSV(std::string filename, std::vector<std::vector<float> > & X);
    void static saveCSV(std::string filename, std::vector<std::vector<float> > & X);
    void static printVector(std::vector<float> &v, char separator = ' ');

    void static printVectorVector(std::vector<std::vector<float> > &v, char separator = ' ', std::string newcase = "\n");

    float static signo(float x);

    void  static splitVector( std::vector<std::vector<float> > &V, std::vector<std::vector<float> > &X,
	    std::vector<std::vector<float> > &Y, unsigned int size_y);

    void static convertirSalida(std::vector<std::vector<float> > &in, std::vector<std::vector<float> > &out);

    std::vector<float> static int2binary(unsigned int val, unsigned int max_clases);
    
    void static genParticiones( std::vector<std::vector<float> > P, 
	std::vector<std::vector<float> > & Entrenamiento,
	std::vector<std::vector<float> > & Validacion,
	std::vector<std::vector<float> > & Prueba,
	unsigned int porcentaje_entrenamiento,	unsigned int porcentaje_prueba,
	unsigned int indice_prueba);
    
    std::string static intToStr(int input);
    std::string static floatToStr(float input);
    
    void static vectorEscalar(std::vector<float> &X, float value, std::vector<float> &Z);
    void static vectorSuma(std::vector<float> &X, std::vector<float> &Y, std::vector<float> &Z);
    void static vectorResta(std::vector<float> &X, std::vector<float> &Y, std::vector<float> &Z);
    float static vectorPunto(std::vector<float> &X, std::vector<float> &Y);
    float static promedio(std::vector<float> &V);
    float static sigmoidea(float x, float param);
    float static sigmoideaPrima(float x, float param);
    float static randomDecimal(float a, float b);
};
#endif
