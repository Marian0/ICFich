#include <iostream>
#include <cassert>
#include <vector>
//#include "Neurona.h"
#include "utils.cpp"


int main (int argc, char *argv[]) {
	std::vector<std::vector<double> > entrada;
	parseCSV("input.txt", entrada);
	printVectorVector<double>(entrada);

	return 0;
}

