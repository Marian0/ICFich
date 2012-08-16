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


        this->plot("plot " + doubletostr(W[0]/W[2]) + "-" + doubletostr(W[1]/W[2]) + "*x lt "+ doubletostr((int)rand()%30+1) +" notitle");
*/
class GNUPlot {
public:
        GNUplot() throw(string);
        ~GNUplot();
        void operator ()(const string& command);
        // send any command to gnuplot
protected:
        FILE *gnuplotpipe;
};

GNUplot::GNUplot() throw(string) {
        gnuplotpipe=popen("gnuplot","w");
        if (!gnuplotpipe) {
                throw("Gnuplot not found !");
        }
}

GNUplot::~GNUplot() {
        fprintf(gnuplotpipe,"exit\n");
        pclose(gnuplotpipe);
}

void GNUplot::operator() (const string& command) {
        fprintf(gnuplotpipe,"%s\n",command.c_str());
        fflush(gnuplotpipe);
        // flush is necessary, nothing gets plotted else
};