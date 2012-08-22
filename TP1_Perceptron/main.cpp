#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
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
	
//	
	std::vector<std::vector<double > > casos, salida;
//	
	utils::parseCSV("Cache/or_disperso500.csv", casos);	

//	
//	//Haremos un string para poder plotear al final
	std::string plot1 = "plot \"-\" notitle pt 1 lt 1\n";
	std::string plot2 = "plot \"-\" notitle pt 8 lt 3\n";
	for (unsigned i = 0 ; i < 500; i++) {
		std::vector<double> a = casos[i%4];
		
		a[0] += utils::randomDecimal(-0.049, 0.049);
		a[1] += utils::randomDecimal(-0.049, 0.049);	
		
		if (a[2] == 1) //es una clase
			plot1 += utils::doubleToStr(a[0]) + " " + utils::doubleToStr(a[1]) + " \n";
		else
			plot2 += utils::doubleToStr(a[0]) + " " + utils::doubleToStr(a[1]) + " \n";

//		plotter(utils::doubleToStr(a[0]) + " " + utils::doubleToStr(a[1]) );
		salida.push_back(a);
	}
	plot1 += "e\n";
	plot2 += "e\n";
	plotter(plot1);
	plotter(plot2);
//	
//	utils::saveCSV("Cache/or_disperso500.csv", salida);
	
	//Definición de una Matriz de adyacencias para las neuronas
	std::vector<bool> a;
	a.push_back(0);
	std::vector<std::vector<bool> > b;
	b.push_back(a);
	
	//Definición de una Matriz de adyacencias para las entradas
	std::vector<bool> ea;
	ea.push_back(1);
	std::vector<std::vector<bool> > eb;
	eb.push_back(ea);
	eb.push_back(ea);
	
	//Instancio la red
	Red perceptron(b,eb,"Red Perceptron");
	
	
	//Divido en X y Yd los casos
	std::vector<std::vector<double> > X, Y;
	utils::splitVector(casos, X, Y);
	
	//Entreno y grafico
	
	for (unsigned int i = 0; i < 1500 ; i++) {
		//Grafico una frontera de desición de color aleatorio
		std::vector<Neurona> V;
		perceptron.getNeuronas(V);
		
		std::vector<double> W;
		V[0].getW(W);
		
		double da  = W[0]/W[2];
		double da2  = W[1]/W[2];
		double da3 = rand()%30+1;
		
		if (i%100 == 0)
		plotter("plot " + utils::doubleToStr(da) + "-" + utils::doubleToStr(da2) + "*x lt "+ utils::doubleToStr(da3) +" notitle");
		
		//Entreno en base a los patrones
		
		
		perceptron.train(X[i%500], Y[i%500]);
		//std::getchar();
	}
	
	
	return 0;
}

