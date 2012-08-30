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
Config config("configuracion2.cfg"); //lectura de la configuracion

int main (int argc, char *argv[]) {

	//inicializacion de semilla
	srand( (unsigned) std::time(NULL)); 
	
	//Leemos los valores de configuracion
	std::string archivo_problema = config.getValue("archivo_problema"); //Archivo a leer patrones ej xor.csv
	unsigned int cantidad_casos = utils::strToInt(config.getValue("cantidad_casos"));
	unsigned int cantidad_conjuntos = utils::strToInt(config.getValue("cantidad_conjuntos"));
	float desvio = utils::strToFloat(config.getValue("desvio"));
	float tasa_aprendizaje = utils::strToFloat(config.getValue("tasa_aprendizaje"));
	unsigned int porcentaje_entrenamiento = utils::strToInt(config.getValue("porcentaje_entrenamiento"));
	unsigned int porcentaje_prueba = utils::strToInt(config.getValue("porcentaje_prueba"));
	unsigned int criterio_max_epocas = utils::strToInt(config.getValue("criterio_max_epocas"));
	unsigned int invasores = utils::strToInt(config.getValue("invasores"));
	float criterio_error_entrenamiento = utils::strToFloat(config.getValue("criterio_error_entrenamiento"));

    //Impresion de los datos de ejecucion
    std::cout<<"Bienvenidos al Ejercicio 2 \n ";
    std::cout<<"Cantidad de epocas = "<<criterio_max_epocas<<'\n';
    std::cout<<"Cantidad de patrones = "<<cantidad_casos<<'\n';
    std::cout<<"Desvio de los datos = "<<desvio*100<<"\%\n";
    std::cout<<"Tasa de aprendizaje = "<<tasa_aprendizaje<<'\n';
    std::cout<<"Porcentaje para entrenamiento = "<<porcentaje_entrenamiento<<"\%\n";
    std::cout<<"Porcentaje para validacion = "<<100-porcentaje_entrenamiento-porcentaje_prueba<<"\%\n";
    std::cout<<"Porcentaje para prueba = "<<porcentaje_prueba<<"\%\n";

    //Inicializo el graficador    
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
	std::vector<std::vector<float> > error_history_entrenamiento, error_history_validacion;
	//Vector temporal para guardar el historico de los pesos sinápticos W
	std::vector<std::vector<Neurona> > neurona_history;
	
	//Leo los patrones en patron
	utils::parseCSV(archivo_problema.c_str(), patron);
	
    //Genero los casos de pruebas en numero y desvío definidos
    patron = utils::genPatrones( patron , cantidad_casos, desvio);
	
	//metemos algunos errores
    random_shuffle(patron.begin() , patron.end());
	for (unsigned int i = 0; i < invasores; i++) {
		return 0;
		if (patron[i][3] == 1)
			patron[i][3] = -1;
		else
			patron[i][3] = 1;
    }
	random_shuffle(patron.begin() , patron.end());


	//Para cada conjunto de particion (Entrenamiento, prueba, validación)
	for (unsigned int i = 0; i < cantidad_conjuntos; i++) {
        std::cout<<"Conjunto "<<i<<"\t";
                        
        //Genero una particion de entrenamiento, prueba y validacion
		utils::genParticiones(patron, entrenamiento, validacion, prueba, 60, 
			30, i*std::floor(porcentaje_prueba/100.0*patron.size()));
		
		std::vector<std::vector<float> > X, Yd; //Sirve para separar X de Yd
		utils::splitVector(entrenamiento,X,Yd,1); //Separo X de Y / Ultimo parametro size_y
						
        //Uso del archivo de estructura de la red para cargarla
        Red perceptron("red_perceptron2.txt","Red Perceptron", tasa_aprendizaje, Neurona::FUNCION_SIGNO);
        

        // perceptron.printStructure(); getchar();
        //Entreno las epocas solicitadas y guardo el error en un vector
		std::vector<float> error_epoca;
		std::string plot2 = "plot \"-\" notitle pt 2 lt 3\n";
        
        for (unsigned int j = 0; j < criterio_max_epocas; j++) {
			//Entrena y calcula error
            float error = 1-perceptron.train(X,Yd);
            error_epoca.push_back(error); 
					
            //Guarda historial de neuronas
			std::vector<Neurona> ntemp;
			perceptron.getNeurons(ntemp);
			neurona_history.push_back(ntemp);
            //std::cout<<"Epoca "<<j<<". Error: "<<error<<std::endl;

            //Dibuja
            plot2 += utils::intToStr((int)j) + " " + utils::floatToStr(error*100.0) + " \n";

    //         if (abs(error) < criterio_error_entrenamiento)
				// break; //Se alcanzó el nivel de error deseado
		}
		plot2 += "e\n";
		plotter(plot2);

		//Guarda error de este conjunto
        error_history_entrenamiento.push_back(error_epoca);
	
        std::getchar();
		plotter("clear"); //limpia dibujo
	}
	
	

	return 0;
}