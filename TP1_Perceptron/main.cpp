#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include "Neurona.h"
#include "utils.h"
#include "Red.h"
#include "Config.h"

#include "GNUPlot.h"

//Variable global
Config config("configuracion.cfg"); //lectura de la configuracion

int main (int argc, char *argv[]) {
	//inicializacion de semilla
	srand( (unsigned) std::time(NULL)); 
	
	//Leemos los valores de configuracion
	std::string archivo_problema = config.getValue("archivo_problema"); //Archivo a leer patrones ej xor.csv
	unsigned int cantidad_casos = utils::strToInt(config.getValue("cantidad_casos"));
	unsigned int cantidad_conjuntos = utils::strToInt(config.getValue("cantidad_conjuntos"));
	double desvio = utils::strToDouble(config.getValue("desvio"));
	double tasa_aprendizaje = utils::strToDouble(config.getValue("tasa_aprendizaje"));
	unsigned int porcentaje_entrenamiento = utils::strToInt(config.getValue("porcentaje_entrenamiento"));
	unsigned int porcentaje_prueba = utils::strToInt(config.getValue("porcentaje_prueba"));
	unsigned int criterio_max_epocas = utils::strToInt(config.getValue("criterio_max_epocas"));
	double criterio_error = utils::strToDouble(config.getValue("criterio_error"));
	
	//Inicializamos y configuramos el Graficador
	GNUPlot plotter;	
	plotter("set pointsize 3");
	plotter("set grid back");	
	plotter("set xzeroaxis lt -1");
	plotter("set yzeroaxis lt -1");	
	plotter("set xrange [-2:100]");
	plotter("set yrange [-0.1:0.1]");
	plotter("set multiplot");
	
	
	//Vectores temporales para trabajar
	std::vector<std::vector<double > > patron, entrenamiento, prueba, validacion;
	//Vectores temporales para guardar historial errores
	std::vector<std::vector<float> > error_history_entrenamiento, error_history_validacion;
	//Vector temporal para guardar el historico de los pesos sinápticos W
	std::vector<std::vector<Neurona> > neurona_history;
	
	//Leo los patrones en patron
	utils::parseCSV(archivo_problema.c_str(), patron);
	
	//Genero los casos de pruebas en numero y desvío definidos
	patron = utils::genPatrones( patron , cantidad_casos, desvio);
	
	std::string plot1 = "plot \"-\" notitle pt 1 lt 1\n";
	std::string plot2 = "plot \"-\" notitle pt 8 lt 3\n";
	
	for (unsigned int i = 0; i < cantidad_conjuntos; i++) {
		//Inicializacion de un perceptron simple:
		//Se crean la matriz de adyacencias para las neuronas y las entradas
		//Matriz Neuronas = 1x1 con false, porque no se conecta a sí misma
		//Matriz Entradas = 2x1 con true, hay 2 entradas que se conectan a una sola neurona (true)
		
		//Definición de una Matriz de adyacencias para las neuronas
		std::vector<bool> fila;
		fila.push_back(false); //una sola neurona
		std::vector<std::vector<bool> > adyacencias;
		adyacencias.push_back(fila);
		
		//Definición de una Matriz de adyacencias para las entradas
		std::vector<bool> fila_entradas;
		fila_entradas.push_back(true); 
		std::vector<std::vector<bool> > adyacencias_entradas;
		//Se crean 2 entradas para la unica neurona
		adyacencias_entradas.push_back(fila_entradas); 
		adyacencias_entradas.push_back(fila_entradas);
		
		//Instancio la red
		Red perceptron(adyacencias,adyacencias_entradas,"Red Perceptron", tasa_aprendizaje, Neurona::FUNCION_SIGMOIDEA);
		
		
		//Genero una particion de entrenamiento, prueba y validacion
		utils::genParticiones(patron, entrenamiento, validacion, prueba, porcentaje_entrenamiento, porcentaje_prueba);
	
		std::vector<std::vector<double> > X, Yd; //Sirve para separar X de Yd
		utils::splitVector(entrenamiento,X,Yd,1); //Separo X de Y / Ultimo parametro size_y
		
		//Entreno las epocas solicitadas y guardo el error en un vector
		//Haremos un string para poder plotear al final		
		std::vector<float> temp;
		for (unsigned int j = 0; j < criterio_max_epocas; j++) {
			double error = perceptron.train(X,Yd);
			std::cout<<"Epoca "<<j<<". Error: "<<error<<std::endl;
			plot2 += utils::intToStr((int)j) + " " + utils::doubleToStr(error) + " \n";
			temp.push_back( (float) error); //Esto puede ser peligroso :D
			std::vector<Neurona> ntemp;
			perceptron.getNeuronas(ntemp);
			neurona_history.push_back(ntemp);
			// if (abs(error) < criterio_error)
			// 	break; //Se alcanzó el nivel de error deseado
		}
		error_history_entrenamiento.push_back(temp);
		
		//Verificamos contra el conjunto de validación
		
		temp.clear();
		utils::splitVector(validacion,X,Yd,1); //Separo X de Y / Ultimo parametro size_y
		for (unsigned int j = 0; j < criterio_max_epocas; j++) {
			double error = perceptron.train(X,Yd,false);
			plot1 += utils::intToStr((int) j) + " " + utils::doubleToStr(error) + " \n";
			temp.push_back( (float) error); //Esto puede ser peligroso :D
		}
		error_history_validacion.push_back(temp);
		
		//Busco el indice donde se dio el menor error de validacion
		float menor = error_history_validacion[i][0];
		unsigned int size_validacion = error_history_validacion.size();
		unsigned int indice_validacion = 0;
		for (unsigned int j = 1; j < size_validacion; j++) {
				if (error_history_validacion[i][j] < menor) {
					menor = error_history_validacion[i][j];
					indice_validacion = j;
				}
		}
		plot1 += "e\n";
		plot2 += "e\n";
		plotter(plot1);
		plotter(plot2);
		
		std::getchar();
	}
	
	

	
	
    
    
    //utils::saveCSV("Cache/or_disperso500.csv", salida);
    //Dibuja

	
    //Inicializacion de un perceptron simple:
    //Se crean la matriz de adyacencias para las neuronas y las entradas
    //Matriz Neuronas = 1x1 con false, porque no se conecta a sí misma
    //Matriz Entradas = 2x1 con true, hay 2 entradas que se conectan a una sola neurona (true)

	Red perceptron("red_perceptron.txt","Red Perceptron", 0.1, Neurona::FUNCION_SIGMOIDEA);

	return 0;
//	//Definición de una Matriz de adyacencias para las neuronas
//	std::vector<bool> fila;
//	fila.push_back(false); //una sola neurona
//	std::vector<std::vector<bool> > adyacencias;
//	adyacencias.push_back(fila);
//	
//	//Definición de una Matriz de adyacencias para las entradas
//	std::vector<bool> fila_entradas;
//	fila_entradas.push_back(true); 
//	std::vector<std::vector<bool> > adyacencias_entradas;
//	//Se crean 2 entradas para la unica neurona
//    adyacencias_entradas.push_back(fila_entradas); 
//	adyacencias_entradas.push_back(fila_entradas);
//	
//	//Instancio la red
//	Red perceptron(adyacencias,adyacencias_entradas,"Red Perceptron", 0.1, Neurona::FUNCION_SIGMOIDEA);
//	
//	//Divido en X y Yd los casos
//	std::vector<std::vector<double> > X, Y;
//	utils::splitVector(salida, X, Y);
//	
//	//Entreno y grafico
//	
//	unsigned int npatrones = X.size();
//	std::<<"asdasdasdasdasdasdas"<<npatrones;
//	unsigned int epocas = 50;
//	
//
//	
//	for (unsigned int i = 0 ; i < epocas; i++) {
//		std::vector<Neurona> V;
//		perceptron.getNeuronas(V);
//		
//		std::vector<double> W = V[0].getW();
//		
//		//Dibujar la frontera de decision
//		double da  = W[0]/W[2];
//		double da2  = W[1]/W[2];
//		double da3 = rand()%30+1;
//		
//		
//		plotter("plot " + utils::doubleToStr(da) + "-" + utils::doubleToStr(da2) + "*x lt "+ utils::doubleToStr(da3) +" notitle");
//		
//		for (unsigned int j = 0; j < npatrones; j++) {
//			
//			//Entreno en base a los patrones
//			
//			
//			perceptron.train(X[j], Y[j]);
//			
//		}
//		getchar();
//	}


	// 	plotter("plot " + utils::doubleToStr(da) + "-" + utils::doubleToStr(da2) + "*x lt "+ utils::doubleToStr(da3) +" notitle");
		
	// 	//Entreno en base a los patrones
		
		
	// 	perceptron.train(X[i%n_casos], Y[i%n_casos]);
	// 	std::getchar();
	// }
	
	
	return 0;
}

