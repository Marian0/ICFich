#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "Neurona.h"
#include "utils.h"

#include "GNUPlot.h"

int main (int argc, char *argv[]) {
	GNUPlot plotter;	
	
	
	
	std::vector<std::vector<double > > casos, salida;
	
	utils::parseCSV("or.csv", casos);	
	
	plotter("plot \"-\" ");
	for (unsigned i = 0 ; i < 500; i++) {
		std::vector<double> a = casos[i%4];
		
		a[0] += utils::randomDecimal(-0.049, 0.049);
		a[1] += utils::randomDecimal(-0.049, 0.049);	
		
		plotter(utils::doubleToStr(a[0]) + " " + utils::doubleToStr(a[1]) );
		salida.push_back(a);
	}
	plotter("e");
	
	utils::saveCSV("or_disperso500.csv", salida);
	
	
	
	return 0;
}

