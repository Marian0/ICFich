#include "utils.h"

#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib> 
#include <cassert>
#include <utility>
#include <cmath>
#include <algorithm>
#include <utility>
#include "Clase.h"

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

//Imprime un vector, separando sus valores con char separator
void utils::printVector(std::vector<int> &v, char separator){
    for (unsigned int i = 0; i < v.size(); i++){
        std::cout<<v[i];
        if(i < v.size()-1) //Si no es el ultimo caso
            std::cout<<separator;
    }
    std::cout<<'\n';
}

//Imprime un vector, separando sus valores con char separator
void utils::printVector(std::vector<bool> &v, char separator){
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

void utils::drawHistory(std::vector<std::vector<float> > &historial, GNUPlot &plotter, unsigned int id_ejercicio) {
    unsigned int n = historial.size();
    assert(n>0);
    unsigned int generaciones = historial[0].size();

    plotter("set pointsize 1");
    plotter("set grid back");
    plotter("set xzeroaxis lt -1");
    plotter("set yzeroaxis lt -1");
    plotter("set xrange [0:" + utils::intToStr(generaciones) + "]");
    switch(id_ejercicio){
        case 1: {
            plotter("set yrange [-520:520]");   //ejercicio 1a
            break;
        }
        case 2: {
            plotter("set yrange [-10:30]");     //ejercicio 1b
            break;
        }
        case 3: {
            plotter("set yrange [-21:2]");     //ejercicio 1c
            break;
        }
        case 4: {
            plotter("set yrange [0:0.05]");     //ejercicio 2 con 4 ciudades
            break;
        }
        case 5: {
            plotter("set yrange [0:0.01]");     //ejercicio 2 con 10 ciudades
            break;
        }
        default: {
            plotter("set yrange [-100:100]");
            break;
        }
    }
    plotter("set xlabel \"Generacion\"");
    plotter("set ylabel \"Fitness\"");
    plotter("set title \"Fitness durante N Generaciones\"");
    plotter("set multiplot");

    for (unsigned int i = 0; i < n; i++) {
        unsigned int m = historial[i].size();
        unsigned int rnd = i + 1;
        std::string str_to_plot = "plot \"-\" notitle pt " + utils::intToStr(rnd) + " lt " + utils::intToStr(rnd) + "\n";

        for (unsigned int j = 0; j < m; j++ ) {
            str_to_plot += utils::intToStr(j) + " " + utils::floatToStr( historial[i][j] ) + " \n";
        }
        str_to_plot += "e\n";
       plotter(str_to_plot);
    }
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


int utils::binary2int(std::vector<bool> &V, bool signo) {
    unsigned int n = V.size();
    int ret_val = 0;
    for (int i = n-1; i >= 1; i--) {
        ret_val += V[i]*pow(2,n-i-1); //sumo potencias de 2
    }
    if (signo == true) {
        if (V[0] == 1)
           ret_val *= -1;
    } else {
        ret_val += V[0]*pow(2,n-1); //si no usaba signo, me olvide de sumar una potencia de 2
    }
    return ret_val;
}


std::vector<bool> utils::int2binary(int value, bool signo) {
    std::vector<bool> ret_val;
    int v = abs(value); //trabajo con el valor absoluto
    while(v != 0) {
        int resto = v % 2;
        v /= 2;
        ret_val.push_back(resto);
    }
    if (signo) {
        if (value < 0) //es negativo, le agrego un 1 que significa que es negativo, si no, un cero
            ret_val.push_back(1);
        else
            ret_val.push_back(0);
    }
    //da vuelta porque el vector se armo al reves
    std::reverse(ret_val.begin(), ret_val.end());
    return ret_val;
}


//Funciones de fitness para el ejercicio 1:
// Se multiplica por -1 porque es un problema de minimización

//Función de fitness para el ejercicio 1a
float utils::fitness_1a(float value) {
    return -1*(-value*sin(sqrt(fabs(value))));
}

//Función de fitness para el ejercicio 1b
float utils::fitness_1b(float value) {
    return -1*(value + 5*sin(3*value) + 8*cos(5*value));
}

//Función de fitness para el ejercicio 1c
float utils::fitness_1c(float x, float y) {
    /*
    float sum_cuad = pow(x,2) + pow(y,2);
    float t_cuad = pow(sum_cuad, 0.25);
    float t_sin = pow(sin(50 * pow(sum_cuad,0.1)),2) + 1;

    return -1*(t_cuad*t_sin);
    */
    float cuad = pow(x,2) + pow(y,2);

    return -1 * pow(cuad, 0.25)*(pow(sin(50 * pow(cuad,0.1)),2) + 1);
}

//Convierte a bin, un vector de binarios representando muchas variables, en un vector de enteros.
// Cada variable tiene asignado tantos bits como dice paso
void utils::vectorBinary2Int(std::vector<bool> &bin, std::vector<int> &res, unsigned int paso){
    //Calculamos la cantidad de variables que tiene bin
    unsigned int cantidad_variables = bin.size()/paso;
    //Recorremos por cada variable
    for (unsigned int i = 0 ; i < cantidad_variables; i++) {
        //vector copia
        std::vector<bool> convertir;
        //Le asignamos el rango que estamos interesados
        convertir.assign(bin.begin()+paso*i, bin.begin()+paso*(i+1));
        //lo convertimos a entero
        int valor = binary2int(convertir,false);
        //Lo agregamos al vector
        res.push_back(valor);
    }
}

//Distancia entre vectores
float utils::vectorDistancia(std::vector<float> &X, std::vector<float> &Y ) {
    std::vector<float> temp;
    utils::vectorResta(X,Y,temp);
    return utils::vectorNorma(temp);
}

//Calcula la norma euclidea de un vector
float utils::vectorNorma(std::vector<float> &X){
    float sum = 0;
    for (unsigned int i = 0; i < X.size(); i++){
        sum += X[i]*X[i];
    }
    return sqrt(sum);
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

//Calcula las coordenadas (i,j) dentro de la matriz que tiene N filas y M columnas
std::pair<unsigned int, unsigned int> utils::posicionMatriz(unsigned int cantidad_filas,
                                                            unsigned int cantidad_columnas,
                                                            unsigned int bloque) {
    std::pair<unsigned int, unsigned int> ret_val;
    ret_val.first = floor(bloque / cantidad_columnas);
    ret_val.second = bloque % cantidad_columnas;
    return ret_val;
}

//Suma las dos matrices miembro a miembro. Devuelve la matriz resultado
std::vector<std::vector<unsigned int> > utils::sumarMatrices(std::vector<std::vector<bool> > &M1,
                                                     std::vector<std::vector<bool> > &M2) {
    unsigned int m1_filas       = M1.size();
    unsigned int m1_columnas    = M1[0].size();
    unsigned int m2_filas       = M2.size();
    unsigned int m2_columnas    = M2[0].size();
    assert(m1_filas == m2_filas);
    assert(m1_columnas == m2_columnas);
    std::vector<std::vector<unsigned int> > ret_val;
    for (unsigned int i = 0; i < m1_filas; i++) {
        for (unsigned int j = 0; j < m1_columnas; j++) {
            ret_val[i][j] = M1[i][j] + M2[i][j];
        }
    }
    return ret_val;
}

//Suma las dos matrices miembro a miembro. Devuelve la matriz resultado
std::vector<std::vector<unsigned int> > utils::sumarMatrices(std::vector<std::vector<unsigned int> > &M1,
                                                     std::vector<std::vector<bool> > &M2) {
    unsigned int m1_filas       = M1.size();
    unsigned int m1_columnas    = M1[0].size();
    unsigned int m2_filas       = M2.size();
    unsigned int m2_columnas    = M2[0].size();
    assert(m1_filas == m2_filas);
    assert(m1_columnas == m2_columnas);
    std::vector<std::vector<unsigned int> > ret_val;
    for (unsigned int i = 0; i < m1_filas; i++) {
        for (unsigned int j = 0; j < m1_columnas; j++) {
            ret_val[i][j] = M1[i][j] + (unsigned int) M2[i][j];
        }
    }
    return ret_val;
}

//Multiplica las dos matrices miembro a miembro. Devuelve una matriz
std::vector<std::vector<bool> > utils::multiplicarMatrices( std::vector<std::vector<bool> > &M1,
                                                            std::vector<std::vector<bool> > &M2) {
    unsigned int m1_filas       = M1.size();
    unsigned int m1_columnas    = M1[0].size();
    unsigned int m2_filas       = M2.size();
    unsigned int m2_columnas    = M2[0].size();
    assert(m1_filas == m2_filas);
    assert(m1_columnas == m2_columnas);
    std::vector<std::vector<bool> > ret_val;
    for (unsigned int i = 0; i < m1_filas; i++) {
        for (unsigned int j = 0; j < m1_columnas; j++) {
            ret_val[i][j] = M1[i][j] * M2[i][j];
        }
    }
    return ret_val;
}

//Para cada día, cuenta la separación que hay en la primer clase de ese dia y la última
//Devuelve un vector con la separación en cada día
std::vector<unsigned int> utils::distanciasEnUnDia(std::vector<std::vector<bool> > matriz_bool) {
    unsigned int cantidad_dias = matriz_bool.size();
    std::vector<unsigned int> distancias;
    //Para cada dia
    for(unsigned int i = 0; i < cantidad_dias; i++) {
        unsigned int pos_ini = 0;
        int pos_fin = matriz_bool[i].size()-1;

        //encuentra el indice del primer true del dia
        while(pos_ini < matriz_bool[i].size() and matriz_bool[i][pos_ini] == false)
            pos_ini++;

        //Encuentra el indice del ultimo true del dia
        while(pos_fin >= 0 and matriz_bool[i][pos_fin] == false)
            pos_fin--;

        //calcula la separacion
        unsigned int separacion;
        //si se pasaron de rosca, es que no hay clases en ese dia
        if(pos_fin < 0 or pos_ini >= matriz_bool[i].size())
            separacion = 0;
        else {
            //Se fija cuantos blancos hay entre el primer true y el ultimo
            for (unsigned int j = pos_ini; j < pos_fin; j++) {
                if (matriz_bool[i][j] == false)
                    separacion++;
            }
        }
        //la agrega al vector
        distancias.push_back(separacion);
    }
    return distancias;
}

//Lee todas las clases y las guarda en un vector
std::vector<Clase> utils::leerClases(std::string nombre_archivo) {
    //Leemos el archivo
    std::ifstream file (nombre_archivo.c_str());
    //Comprobamos que este abierto
    assert(file.is_open());
    //Valor a retornar, vector con todas las clases
    std::vector<Clase> ret_val;
    std::string line;
    //Hasta llegar al final
    //Lee una linea
    while (getline(file, line)) {
        std::stringstream iss;
        //Pasa la string a streamstring
        iss<<line;

        //Cometario, no se procesa
        if(!iss.str().empty() && iss.str()[0] == '#'){
            //std::cout<<iss.str()<<std::endl;
            continue;
        }

        //Separa la linea en 3
        std::string nom_mat;
        unsigned int id_mat;
        unsigned int cant_horas;
        unsigned int anio;


        //std::stringstream ss;
        for (unsigned int i = 0; i < 4; i++) {
            std::string s;
            //Lee un parametro
            getline(iss,s,',');

            //Segun que tipo de parametro sea...
            if (i == 0)
                nom_mat = s;
            if (i == 1)
                id_mat = utils::strToInt(s);
            if (i == 2)
                cant_horas = utils::strToInt(s);
            if (i == 3)
                anio = utils::strToInt(s);
        }
        //Construye una clase
        Clase clase (nom_mat, id_mat, cant_horas, anio);
        //La agrega al vector
        ret_val.push_back(clase);

    }

    file.close();
    return ret_val;
}

void utils::escribirSolucion(std::vector<std::vector<std::vector<int> > >matriz_int, std::vector<Clase> clases, std::string archivo_salida) {
    std::ofstream file(archivo_salida.c_str());
    assert(file.is_open());
    //por cada aÃ±o
    for (unsigned int i = 1; i < matriz_int.size(); i++) {
        file<<"\nAnio "<<i<<"\n";
        //por cada dia
        for (unsigned int j = 0; j < matriz_int[i].size(); j++) {
            //por cada bloque
            for (unsigned int k = 0; k < matriz_int[i][j].size(); k++) {
                unsigned id_clase = matriz_int[i][j][k];
                std::string nombre_clase = clases[id_clase].nombre;
                file<<nombre_clase<<'\t';
            }
            file<<'\n';
        }
        file<<'\n';
    }
    file<<'\n';
    file.close();
}

