#include <iostream>
#include <cassert>
#include <vector>
#include "Neurona.h"
#include "utils.cpp"

#include "GNUPlot.h"

int main (int argc, char *argv[]) {
	GNUPlot plotter;
	
	plotter("plot sin(x); pause -1");
	
	
//	
//	std::vector<std::vector<double> > entrada;
//	parseCSV("input.txt", entrada);
//	printVectorVector<double>(entrada);

	return 0;
}

