#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "Neurona.h"
#include "utils.h"

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
	
	
	std::vector<std::vector<double > > casos, salida;
	
	utils::parseCSV("or.csv", casos);	
	
	//Haremos un string para poder plotear al final
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
	
	utils::saveCSV("Cache/or_disperso500.csv", salida);
	
	
	
	return 0;
}

