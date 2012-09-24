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
    //fixes de overflow y underflow
    //La sigmoidea da 1 y -1 para valores alejados del origen
    if(x < -param) return -1.0;
    if(x > param) return  1.0;
    
	float result = (1-exp(-param*x))/(1+exp(-param*x));
    //std::cout<<"X = "<<x<<"Result = "<<result<<'\n';
	return result;
}

//Derivada de la funcion sigmoidea, en la forma:
//(2)/(1+e^(-bx)) - 1
float utils::sigmoideaPrima(float x, float param) {
    double num = exp(-param*x);
    double den = 1+exp(-param*x);
    den *= den;
    //std::cout<<"Sigmoidea prima: "; std::cout<<num<<'/'<<den<<'='<<num/den<<'\n';

    double ret_val = num/den;
    return (float) ret_val;

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
        if (T.size() - size_y == 2) { //x,y + salida esperada
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

//Convierte unos valores {0, 1, 2, 3...} en una codificacion {[1,-1,-1,-1,...], [-1,1,-1,-1,...], ... }
void utils::convertirSalida(std::vector<std::vector<float> > &in, std::vector<std::vector<float> > &out) {
    if (in[0].size() > 1) { //la salida ya viene codificada, no tengo que hacer nada
        std::vector<std::vector<float> > V = in;
        out = V;
        return;
    }
    out.clear();
    
    unsigned int cant_casos = in.size();
    std::vector<float> inn;
    inn.resize(cant_casos);
    //Convierto un vector de vector de float (con un solo elemento) en un vector de float
    for (unsigned int i = 0; i < cant_casos; i++) {
        inn[i] = in[i][0];
    }

    unsigned int maximo_clases = 1+(unsigned int) *(std::max_element(inn.begin(), inn.end())); //obtiene la ultima clase, para saber cuantas tengo

    for (unsigned int i = 0; i < cant_casos; i++) {
        unsigned int val = (unsigned int) inn[i]; //warning, danger casting
        out.push_back(utils::int2binary(val, maximo_clases));
    }
}

//Convierte un entero a un array de -1 y 1
std::vector<float> utils::int2binary(unsigned int val, unsigned int max_clases) {
    std::vector<float> temp;
    temp.resize(max_clases, -1.0);
    temp[val] = 1.0;
    return temp;
}
//Funcion para graficar el conjuto de patrones
void utils::drawPlot(
		std::vector<std::vector<float> > & X,
		std::vector<std::vector<float> > & YD,
		std::vector<std::vector<float> > & YC,
		GNUPlot & plotter
	) {
	//Verifico la cantidad de entradas sean iguales
	unsigned int nX = X.size();
	unsigned int nYC = YC.size();
	unsigned int nYD = YD.size();
	assert( nX > 0 && nX == nYD && nX == nYC ); 


	//Verifico que X sea graficable (2D)
	assert( X[0].size() == 2 ); 

	//Vefifico que tengan la misma dimension las salidas
	unsigned int dimension_salida = YD[0].size();
	assert( dimension_salida == YC[0].size() );

	//Calculo cantidad de clases 2^n
	//unsigned int cantidad_clases = pow(2, YC[0].size() );
    unsigned int cantidad_clases = YC[0].size();

	//Vectores temporales para guardar los comandos de gnuplot para graficación.
	std::vector<std::string> str2plot_good; //Bien clasificados
	std::vector<std::string> str2plot_bad;  //Mal Clasificados
    
    str2plot_good.resize(cantidad_clases);
    str2plot_bad.resize(cantidad_clases);

    //Inicializo el vector de string para graficación
	for (unsigned int i = 0; i < cantidad_clases; i++) {
		str2plot_good[i] = "plot \"-\" notitle pt " + utils::intToStr(i+3) + " lt 3\n";
		str2plot_bad[i] = "plot \"-\" notitle pt " + utils::intToStr(i+3) + " lt 1\n";
	}

	//Lenght minimo para decir que es una clase vacía (no graficar)
	unsigned int lenght_minimo = str2plot_good[0].size() + 5;

	//Recorro los patrones
	for (unsigned int i = 0; i < nX; i++) {
		// std::cout<<"la calculada ";
		
		unsigned int salida_real = utils::binary2int(YC[i]);
		//unsigned int salida_deseada = utils::binary2int(YD[i]);
		unsigned int salida_deseada = utils::binary2int(YD[i]);
		// std::cout<<"la clase real"<<salida_real<<" la deseada"<<salida_deseada<<std::endl; 
		// getchar();

		std::string punto = utils::floatToStr( X[i][0] ) + " " + utils::floatToStr( X[i][1] ) + " \n";
		if (salida_real - salida_deseada == 0) {
			//Esta bien clasificado
			str2plot_good[salida_deseada] += punto;
		} else {
			str2plot_bad[salida_deseada] += punto;
		}
	}

	std::string output = "set multiplot\n";
	//Cerramos los strings de graficacion
	for (unsigned int i = 0; i < cantidad_clases; i++) {
		//Los strings que hayan añadido algun punto tendran mayor size que el lenght minimo
		if ( str2plot_good[i].size() > lenght_minimo )
			output += str2plot_good[i] + "e\n";

		if ( str2plot_bad[i].size() > lenght_minimo )
			output += str2plot_bad[i] + "e\n";
	}
	//Enviamos al pipe de graficacion :D
	plotter(output);
}

//Funcion que dado un arreglo de valores, devuelve el id del mayor
//Se usa para determinar cual de las neuronas es la que más se activo pasandole el vector de las salidas.
unsigned int utils::binary2int( std::vector<float> & input ) {

	float maximo = input[0];
	unsigned int imax = 0;
    for (unsigned int i = 0; i < input.size(); i++) {
        if (input[i] > maximo) {
            maximo = input[i];
            imax = i;
        }
    }
    return imax;
}


//Distancia entre vectores 
float utils::vectorDistancia(std::vector<float> &X, std::vector<float> &Y ) {
	std::vector<float> temp;
	utils::vectorResta(X,Y,temp);
	return utils::vectorNorma(temp);
}

//Dado un vector, devuelve el índice de menor elemento del vector 
unsigned int utils::getMinIdx(std::vector<float> &V ) {
	unsigned int n = V.size();
	assert(n > 0);
	unsigned int imin = 0;
	float min = V[imin];

	for (unsigned int i = 1; i < n; i++) {
		if (V[i] < min) {
			min = V[i];
			imin = i;
		}
	}
	return imin;
}


void utils::drawPoints(std::vector<std::vector<float> > &V, GNUPlot &plotter, unsigned int color, unsigned int tipo) {
	unsigned int cantidad = V.size();
	assert(cantidad > 0);
	assert(V[0].size() == 2);

	std::string to_plot = "plot \"-\" notitle pt " + utils::intToStr(tipo) + " lt " + utils::intToStr(color) + "\n";
	
	// float minx = V[0][0];
	// float miny = V[0][1];
	// float maxx = V[0][0];
	// float maxy = V[0][1];

	for (unsigned int i = 0; i < cantidad; i++) {
		to_plot += utils::floatToStr( V[i][0] ) + " " + utils::floatToStr( V[i][1] ) + " \n";	
		// if (V[i][0] > maxx)
		// 	maxx = V[i][0];

		// if (V[i][1] > maxy)
		// 	maxy = V[i][1];

		// if (V[i][0] < minx)
		// 	minx = V[i][0];
		
		// if (V[i][1] < miny)
		// 	miny = V[i][1];


	}	
	
	// plotter("set yrange [" + utils::floatToStr(miny) + ":" + utils::floatToStr(maxy) + "]");
	// plotter("set xrange [" + utils::floatToStr(minx) + ":" + utils::floatToStr(maxx) + "]");

	// plotter("set multiplot\n");
	plotter(to_plot);
	plotter("e\n");


}