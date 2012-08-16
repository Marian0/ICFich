#include "GNUPlot.h"


/*
        Class: GNUPlot 

        Abre un pipe para graficación en GNUPlot desde C++
*/


GNUPlot::GNUPlot() throw(std::string) {
	//persist para evitar que se cierre la ventana
    gnuplotpipe = popen("gnuplot -persist","w"); 
    if (!gnuplotpipe) {
        throw("Gnuplot not found !");
    }
}

GNUPlot::~GNUPlot() {
    fprintf(gnuplotpipe,"exit");
    pclose(gnuplotpipe);
}

void GNUPlot::operator() (const std::string& command) {
    fprintf(gnuplotpipe,"%s\n",command.c_str());
    fflush(gnuplotpipe);
    // flush is necessary, nothing gets plotted else
};
