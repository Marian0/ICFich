#include <iostream>
#include <cassert>
#include <vector>
#include "Neurona.h"
#include "utils.h"

#include "GNUPlot.h"

int main (int argc, char *argv[]) {
	//GNUPlot plotter;
	
	//plotter("plot sin(x); pause -1");
	Neurona *hola = new Neurona(3,-0.5,0.5,Neurona::FUNCION_SIGMOIDEA,0.5);
	
//	
//	std::vector<std::vector<double> > entrada;
//	parseCSV("input.txt", entrada);
//	printVectorVector<double>(entrada);

	return 0;
}

