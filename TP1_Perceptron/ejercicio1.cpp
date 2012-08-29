#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "Neurona.h"
#include "utils.h"
#include "Red.h"
#include "Config.h"

#include "GNUPlot.h"

//Variable global
Config config("configuracion1.cfg"); //lectura de la configuracion

int main (int argc, char *argv[]) {
	//inicializacion de semilla
	srand( (unsigned) std::time(NULL)); 
	
	//Leemos los valores de configuracion
	std::string archivo_problema = config.getValue("archivo_problema"); //Archivo a leer patrones ej xor.csv
	unsigned int cantidad_casos = utils::strToInt(config.getValue("cantidad_casos"));
	unsigned int cantidad_conjuntos = utils::strToInt(config.getValue("cantidad_conjuntos"));
	unsigned int tamanio_conjunto = utils::strToInt(config.getValue("tamanio_conjunto"));
	float desvio = utils::strToFloat(config.getValue("desvio"));
	unsigned int porcentaje_entrenamiento = utils::strToInt(config.getValue("porcentaje_entrenamiento"));
	unsigned int porcentaje_prueba = utils::strToInt(config.getValue("porcentaje_prueba"));
	float tasa_aprendizaje = utils::strToFloat(config.getValue("tasa_aprendizaje"));
	unsigned int criterio_max_epocas = utils::strToInt(config.getValue("criterio_max_epocas"));
	unsigned int invasores = utils::strToInt(config.getValue("invasores"));
	float criterio_error = utils::strToFloat(config.getValue("criterio_error"));

    //Impresion de los datos de ejecucion
    std::cout<<"Cantidad de epocas = "<<criterio_max_epocas<<'\n';
    std::cout<<"Cantidad de patrones = "<<cantidad_casos<<'\n';
    std::cout<<"Desvio de los datos = "<<desvio*100<<"\%\n";
    std::cout<<"Tasa de aprendizaje = "<<tasa_aprendizaje<<'\n';

	//Inicializamos y configuramos el Graficador
	GNUPlot plotter2;	
	plotter2("set xzeroaxis lt -1");
	plotter2("set yzeroaxis lt -1");	
	plotter2("set xrange [-2:2]");
	plotter2("set yrange [-2:2]");
    plotter2("set multiplot");
	plotter2("set grid back");	
	plotter2("set pointsize 1");
    
	GNUPlot plotter;	
	plotter("set pointsize 1");
	plotter("set grid back");	
	plotter("set xzeroaxis lt -1");
	plotter("set yzeroaxis lt -1");	
	plotter("set xrange [0:"+utils::intToStr(criterio_max_epocas+1)+"]");
	plotter("set yrange [-0.1:100]");
    plotter("set xlabel \"Epocas\"");
    plotter("set ylabel \"Error\"");
    plotter("set format y \"\%g \%\%\""); //formato porcentaje en ylabel
    plotter("set title \"Error durante N Epocas\"");
    plotter("set multiplot");

	
	//Vectores temporales para trabajar
	std::vector<std::vector<float > > patron, entrenamiento, prueba, validacion;
	//Vectores temporales para guardar historial errores
	std::vector<float> error_history_entrenamiento;

	//Leo los patrones en patron
	utils::parseCSV(archivo_problema.c_str(), patron);
	
    //Genero los casos de pruebas en numero y desvío definidos
    patron = utils::genPatrones( patron , cantidad_casos, desvio);
	
	//metemos algunos errores
    random_shuffle(patron.begin() , patron.end());
	for (unsigned int i = 0; i < invasores; i++) {
		if (patron[i][2] == 1)
			patron[i][2] = -1;
		else
			patron[i][2] = 1;
    }
	random_shuffle(patron.begin() , patron.end());

	//Dibuja los puntos de cada clase en una ventana para ver la dispersion
	std::string plot_dot1 = "plot \"-\" notitle pt 1 lt 3\n";
	std::string plot_dot2 = "plot \"-\" notitle pt 1 lt 1\n";
	for (unsigned int w=0; w < patron.size(); w++ ) {
		if (patron[w][2] == 1) 
			plot_dot1 += utils::floatToStr(patron[w][0])+ " " + utils::floatToStr(patron[w][1]) + " \n";
		else
			plot_dot2 += utils::floatToStr(patron[w][0])+ " " + utils::floatToStr(patron[w][1]) + " \n";
	}
	plot_dot1 += "e\n";
	plot_dot2 += "e\n";
	plotter2(plot_dot1);
	plotter2(plot_dot2);
					
	//Inicializo el ploteo
	//Haremos un string para poder plotear al final		
	std::string plot2 = "plot \"-\" notitle pt 1 lt 3\n";
	
 
    //Instancio la red
    Red perceptron("red_perceptron.txt","Red Perceptron", tasa_aprendizaje, Neurona::FUNCION_SIGMOIDEA);
    


	utils::genParticiones(patron, entrenamiento, validacion, prueba, porcentaje_entrenamiento, 
			porcentaje_prueba, 0);

	    //Divido en X y Yd los casos
    std::vector<std::vector<float> > X, Yd;
    utils::splitVector(entrenamiento, X, Yd);
    
	for (unsigned int i = 0 ; i < criterio_max_epocas; i++) {
		std::vector<Neurona> V;
        perceptron.getNeurons(V);
		
		std::vector<float> W = V[0].getW();
		
		//Dibujar la frontera de decision
		float da  = W[0]/W[2];
		float da2  = W[1]/W[2];
		float da3 = rand()%30+1;
		plotter2("plot " + utils::floatToStr(da) + "-" + utils::floatToStr(da2) + "*x lt "+ utils::floatToStr(da3) +" notitle");
		
		//Entrena y calcula error
		float error = 1-perceptron.train(X, Yd, true);
		error_history_entrenamiento.push_back(error); 
				
		//Dibuja
		plot2 += utils::intToStr((int) i) + " " + utils::floatToStr(error*100.0) + " \n";
        //std::getchar();

    }
    
    //Actualizacion del dibujo
    plot2 += "e\n";
    plotter(plot2);

	
	//Prueba con los patrones nunca vistos

	//Cargo el conjunto de prueba
	utils::splitVector(prueba, X, Yd,1); 
	
	float efectividad_esperada = perceptron.train(X, Yd, false);
	std::cout<<"Efectividad Esperada = "<<efectividad_esperada*100.0<<"\%\n";
    
	
    return 0;
}
