#ifndef __GNUPLOT_H__
#define __GNUPLOT_H__

#include <stdio.h>
#include <string>

/*

Clase para Plotear..... Gracias a GNU PLOT

Para instalar el gnu plot es bajarlo, ./configure , sudo make install
Necesita incluirse #include <stdio.h>


algunos ejemplos
        plotter("set pointsize 3");
        plotter("set grid back");
        
        plotter("set xzeroaxis lt -1");
        plotter("set yzeroaxis lt -1");
        
        plotter("set xrange [-2:2]");
        plotter("set yrange [-2:2]");
        //plotter("set key box"); es un recuadro al titulo derecho
        plotter("set multiplot");
        plotter("set title \"Grafica TP 1 - Inteligencia Computacional 2011\"");
        plotter("plot \"-\" notitle pt 6");


        this->plot("plot " + floattostr(W[0]/W[2]) + "-" + floattostr(W[1]/W[2]) + "*x lt "+ floattostr((int)rand()%30+1) +" notitle");
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
