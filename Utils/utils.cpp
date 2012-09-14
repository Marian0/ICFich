#include "utils.h"

#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib> 
#include <cassert>
#include <cmath>
#include <algorithm>
//Constante para comparaciones con cero

/*
Entrada: Path al archivo csv
Salida: Devuelve un vector de vector<float>  con el contenido de un archivo CSV
	por referencia
*/

void utils::parseCSV(std::string filename, std::vector<std::vector<float> > & X) {

	std::ifstream file;
	file.open(filename.c_str());
	assert(file.is_open()); //muestra error si no se pudo abrir el archivo
	
	float value;
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
		
		X.push_back(std::vector<float>()); 		//agrega un vector vacio	
		while(getline(iss,s,',')){ //separa la linea entre las comas
			//transforma de string a float
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

void utils::saveCSV(std::string filename, std::vector<std::vector<float> > & X) {
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
void utils::printVector(std::vector<float> &v, char separator){
	for (unsigned int i = 0; i < v.size(); i++){
		std::cout<<v[i];
		if(i < v.size()-1) //Si no es el ultimo caso
			std::cout<<separator;
	}
    std::cout<<'\n';
}

//Imprime un vector de vectores. Sus parametros representan el vector,
// el separador entre elementos(v[i]), y el separador entre casos (v[i][j])
void utils::printVectorVector(std::vector<std::vector<float> > &v, char separator, 
						std::string newcase){
	//std::cout<<"Inicio Vector debug: size(" << v.size() << ")"<<std::endl;
	for (unsigned int i = 0; i < v.size(); i++){
		utils::printVector(v[i],separator);
		std::cout<<newcase;
	}
	//std::cout<<"Fin Vector debug: size(" << v.size() << ")"<<std::endl;
}


//Imprime un vector, separando sus valores con char separator
void utils::printVector(std::vector<unsigned int> &v, char separator){
	for (unsigned int i = 0; i < v.size(); i++){
		std::cout<<v[i];
		if(i < v.size()-1) //Si no es el ultimo caso
			std::cout<<separator;
	}
    std::cout<<'\n';
}

//Imprime un vector de vectores. Sus parametros representan el vector,
// el separador entre elementos(v[i]), y el separador entre casos (v[i][j])
void utils::printVectorVector(std::vector<std::vector<unsigned int> > &v, char separator, 
						std::string newcase){
	for (unsigned int i = 0; i < v.size(); i++){
		utils::printVector(v[i],separator);
		std::cout<<newcase;
	}
}

//Genera un numero aleatorio en el rango [a,b].
// NOTA: solo funciona para numeros de punto flotante

float utils::randomDecimal(float a, float b) {
	float random = ((float) rand()) / (float) RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void utils::randomCircular(float xi, float yi, float desvio, float &newx, float &newy){
    float modulo_old = sqrt(xi*xi + yi*yi);
    float angulo = randomDecimal(0, 2*3.14);
    float delta = randomDecimal(0, desvio);
    float modulo = modulo_old*delta; //le sumo el desvio%
    newx = xi+modulo*cos(angulo);
    newy = yi+modulo*sin(angulo);
}

//Funciones de activacion

//Funcion signo
float utils::signo(float x){
	if(x < 0)
		return -1;
	else
		return 1;
}

//Funcion sigmodea
float utils::sigmoidea(float x, float param){
	float result = (1-exp(-param*x))/(1+exp(-param*x));
	return result;
}

//Derivada de la funcion sigmoidea, en la forma:
//(2)/(1+e^(-bx)) - 1
float utils::sigmoideaPrima(float x) {
    return 0.5*(1 + x)*(1 - x);
}


void utils::vectorEscalar(std::vector<float> &X, float value, std::vector<float> &Z){
	Z.resize(X.size());
	for(unsigned int i = 0; i < X.size(); i++){
		Z[i] = X[i]*value;
	}
}

void utils::vectorSuma(std::vector<float> &X, std::vector<float> &Y, std::vector<float> &Z){
	assert(X.size() == Y.size());
	std::vector<float> temp;
	
	temp.resize(X.size());
	for(unsigned int i = 0; i < X.size(); i++){
		temp[i] = X[i] + Y[i];
	}
	Z = temp;
}

void utils::vectorResta(std::vector<float> &X, std::vector<float> &Y, std::vector<float> &Z){
	std::vector<float> temp;
	utils::vectorEscalar(Y,-1,temp);
	utils::vectorSuma(X, temp ,Z);
}

float utils::vectorPunto(std::vector<float> &X, std::vector<float> &Y){
	assert(X.size() == Y.size());
	float suma = 0;
	for(unsigned int i = 0; i < X.size(); i++){
		suma  += X[i]*Y[i];
	}
	return suma;
}

bool utils::vectorParecido(std::vector<float> &X, std::vector<float> &Y, float criterio) {
	float moduloX = utils::vectorNorma(X);
	float moduloY = utils::vectorNorma(Y);
	
    //Los 2 son el vector cero.
	if ( moduloX < utils::EPSILON && moduloY < utils::EPSILON){
		return true;
    }

    std::vector<float> resta;
	utils::vectorResta(X,Y,resta);
	
	//Son 2 vectores iguales
	if ( fabs( utils::vectorNorma(resta) ) < utils::EPSILON ) {
		return true;
    }

	//Criterio producto punto
    float punto = utils::vectorPunto(X,Y);
    float parecido = fabs(punto/(moduloX*moduloY));

    if (parecido > criterio) {
		return true;
    }

	return false;
}


//Calcula la norma euclidea de un vector
float utils::vectorNorma(std::vector<float> &X){
    float sum = 0;
    for (unsigned int i = 0; i < X.size(); i++){
        sum += X[i]*X[i];
    }
    return sqrt(sum);
}


//Separar un vector en 2
void  utils::splitVector( std::vector<std::vector<float> > &V, 
	std::vector<std::vector<float> > &X,
 	std::vector<std::vector<float> > &Y, unsigned int size_y) {
	//Limpiamos por las dudas
	X.clear();
	Y.clear();
	
	unsigned int n = V.size();
	
	for (unsigned int i = 0 ; i < n; i++) {
		unsigned int m = V[i].size();
		assert(m > size_y); //Sino, para que lo queres dividir?
		std::vector<float> Xtemp, Ytemp;
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

//Devuelve la cantidad de patrones especificados con el desvio especificado tomando como base el patron P.
std::vector<std::vector<float> > utils::genPatrones( std::vector<std::vector<float> > & P,
	unsigned int cantidad_final, float desvio, unsigned int size_y) {
	//Cantidad de patrones P
	unsigned int n = P.size();
	
	//Temporal para la salida de los Patrones generados
	std::vector<std::vector<float> > Salida;
	
    //Fábrica de patrones de cantidad especificada
	for (unsigned int k = 0 ; k < cantidad_final ; k++) {
		//Obtengo el patron actual
		std::vector<float> T = P[k%n];
		
        //Solo para el caso 2d
        if (T.size() == 3) { //x,y + salida esperada
            //Lo desvio en forma circular
            float xnuevo, ynuevo;
            utils::randomCircular(T[0], T[1], desvio, xnuevo, ynuevo);
            T[0] = xnuevo;
            T[1] = ynuevo;

        } else { 
            //Lo desvío en forma cuadrada
            unsigned int m = T.size();
            for (unsigned int j = 0; j < m ; j++) {
                if (j < m-size_y) {
                    //Se trata de una entrada X => La dispersamos
                    float rango = fabs( T[j] * desvio );
                    T[j] = T[j] + utils::randomDecimal(-rango, rango);				
                } else {
                    break; //Las y las dejamos inalteradas 
                }
            }
        }
		Salida.push_back(T);		
	}
	return Salida;
}


//En base a los patrones pasados por referencia, el % de entrenamiento, prueba y validacion, genera las particiones aleatorias correspondientes
void utils::genParticiones( std::vector<std::vector<float> > P, 
	std::vector<std::vector<float> > & Entrenamiento,
	std::vector<std::vector<float> > & Validacion,
	std::vector<std::vector<float> > & Prueba,
	unsigned int porcentaje_entrenamiento,	unsigned int porcentaje_prueba,
	unsigned int indice_prueba) {
		
		//Pedimos que los porcentajes sean válidos
		assert(porcentaje_entrenamiento + porcentaje_prueba <= 100);
		//Limpiamos las salidas
		Entrenamiento.clear();
		Validacion.clear();
		Prueba.clear();
		//Mezcla
    	random_shuffle(P.begin(), P.end());
		
		//Calculo cantidades
		unsigned int n_patrones = P.size();
		unsigned int n_ent = std::ceil((float)porcentaje_entrenamiento/100 * n_patrones);
		unsigned int n_prueba = std::floor((float)porcentaje_prueba/100 * n_patrones);

		//Pedimos que el indice sea un offset válido
		indice_prueba = indice_prueba % (n_patrones - n_prueba); //Limite inicial
		//comprobar con @fern17 
		assert(indice_prueba+n_prueba < n_patrones); //Va < o <= ?

		
		unsigned int i = 0;
		for (; i < n_patrones; i++){
			if (i >= indice_prueba && i < (indice_prueba+n_prueba)) {
				Prueba.push_back(P[i]);
			} 
			else if (n_ent > 0) { 
				Entrenamiento.push_back(P[i]);
				n_ent--;
			} else {
				Validacion.push_back(P[i]);
			}
		}
}


//Genera un subconjunto a partir de un conjunto mas grande, con la cantidad de elementos dados en tamanio
std::vector<std::vector<float> > utils::genSet( 
	std::vector<std::vector<float> > P,
    unsigned int tamanio,
    unsigned int inicio
    ) {
    //aseguro que el tamanio del subconjunto sea menor al del conjunto grande
    unsigned int n = P.size();
    assert(tamanio < n); 
    
    //Toma los primeros valores
    std::vector<std::vector<float> > ret_val (P.begin() + inicio, P.begin()+tamanio + inicio );
    return ret_val;
}

std::string utils::floatToStr(float input) {
	std::stringstream str;
	str<<input;
	return str.str();
}

int utils::strToInt(std::string s){
    return atoi(s.c_str());
}
		
std::string utils::intToStr(int input) {
	std::stringstream str;
	str<<input;
	return str.str();
}


float utils::strToFloat(std::string s){
    return atof(s.c_str());
}

float utils::promedio(std::vector<float> &V) {
    if (V.empty())
        return 0.0; //para que no devuelva nan
    
    float suma = 0.0;
    for (unsigned int i = 0; i < V.size(); i++) {
        suma += V[i];
    }
    return suma/((float) V.size());
}

float utils::devest(std::vector<float> &V, float media) {
    if (V.empty())
        return 0.0; //para que no devuelva NaN

    float suma = 0.0;
    for (unsigned int i = 0; i < V.size(); i++) {
        float temp = V[i] - media;
        suma += temp*temp;
    }
    return suma/V.size();
}
