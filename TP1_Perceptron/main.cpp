#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include "Neurona.h"
#include "utils.h"
#include "Red.h"


#include "GNUPlot.h"

int main (int argc, char *argv[]) {
	GNUPlot plotter;	
	plotter("set pointsize 3");
	plotter("set grid back");
	
	plotter("set xzeroaxis lt -1");
	plotter("set yzeroaxis lt -1");
	
	plotter("set xrange [-2:2]");
	plotter("set yrange [-2:2]");
	plotter("set multiplot");
   
    srand( (unsigned) std::time(NULL)); //inicializacion de semilla
    	
	//Lectura de casos de prueba
    std::vector<std::vector<double > > casos, salida;
	utils::parseCSV("Cache/or_disperso500.csv", casos);	
    
    //Haremos un string para poder plotear al final
	std::string plot1 = "plot \"-\" notitle pt 1 lt 1\n";
	std::string plot2 = "plot \"-\" notitle pt 8 lt 3\n";

	// Esto bate fruta porque lee cosas re locas
     
    //Genera casos aleatorios con <5% de dispersion
    for (unsigned i = 0 ; i < 500; i++) {
		std::vector<double> a = casos[i];
		
		a[0] += utils::randomDecimal(-0.449, 0.449);
		a[1] += utils::randomDecimal(-0.449, 0.449);	
		
		if (a[2] == 1) //Clase "true"
			plot1 += utils::doubleToStr(a[0]) + " " + utils::doubleToStr(a[1]) + " \n";
		else  //Clase "false"
			plot2 += utils::doubleToStr(a[0]) + " " + utils::doubleToStr(a[1]) + " \n";

        //plotter(utils::doubleToStr(a[0]) + " " + utils::doubleToStr(a[1]) );
		salida.push_back(a);
	}
    

    
    //utils::saveCSV("Cache/or_disperso500.csv", salida);
    
    //Dibuja
	plot1 += "e\n";
	plot2 += "e\n";
	plotter(plot1);
	plotter(plot2);
	
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
	Red perceptron(adyacencias,adyacencias_entradas,"Red Perceptron");
	
	//Divido en X y Yd los casos
	std::vector<std::vector<double> > X, Y;
	utils::splitVector(casos, X, Y);
	
	//Entreno y grafico

	for (unsigned int i = 0; i < 1500 ; i++) {
		//Grafico una frontera de desición de color aleatorio
		std::vector<Neurona> V;
		perceptron.getNeuronas(V);
		
		std::vector<double> W = V[0].getW();
		
        //Dibujar la frontera de decision
		double da  = W[0]/W[2];
		double da2  = W[1]/W[2];
		double da3 = rand()%30+1;

		
		plotter("plot " + utils::doubleToStr(da) + "-" + utils::doubleToStr(da2) + "*x lt "+ utils::doubleToStr(da3) +" notitle");
		
		//Entreno en base a los patrones
		
		
		perceptron.train(X[i%500], Y[i%500]);
		std::getchar();
	}
	
	
	return 0;
}

