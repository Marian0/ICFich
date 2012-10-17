#ifndef __GNUPLOT_H__
#define __GNUPLOT_H__

#include <stdio.h>
#include <string>

/*
Clase para Plotear..... Gracias a GNU PLOT

Para instalar el gnu plot es bajarlo, ./configure , sudo make install
Necesita incluirse #include <stdio.h>
*/

class GNUPlot {
public:
        GNUPlot() throw(std::string);
        ~GNUPlot();
        void operator ()(const std::string& command);
protected:
        FILE *gnuplotpipe;
};

#endif
