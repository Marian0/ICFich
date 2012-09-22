#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "utils.h"
#include "RedRBF.h"
#include "Config.h"

#include "GNUPlot.h"

//Variable global
Config config("configuracion1.cfg"); //lectura de la configuracion

int main (int argc, char *argv[]) {
	//inicializacion de semilla
	srand( (unsigned) std::time(NULL)); 
	
	//Leemos los valores de configuracion
	std::string     archivo_problema               = config.getValue("archivo_problema"); //Archivo a leer patrones ej xor.csv
	unsigned int    porcentaje_entrenamiento       = utils::strToInt(config.getValue("porcentaje_entrenamiento"));
	unsigned int    porcentaje_prueba              = utils::strToInt(config.getValue("porcentaje_prueba"));
	unsigned int    cantidad_casos                 = utils::strToInt(config.getValue("cantidad_casos"));
	float           desvio                         = utils::strToFloat(config.getValue("desvio"));
	float           tasa_aprendizaje               = utils::strToFloat(config.getValue("tasa_aprendizaje"));
	unsigned int    criterio_max_epocas            = utils::strToInt(config.getValue("criterio_max_epocas"));
	float           criterio_error                 = utils::strToFloat(config.getValue("criterio_error"));
    float           parametro_sigmoidea            = utils::strToFloat(config.getValue("parametro_sigmoidea"));
    float           criterio_error_consecutivo     = utils::strToFloat(config.getValue("criterio_error_consecutivo"));
    std::string     criterio_finalizacion          = config.getValue("criterio_finalizacion");
    unsigned int    minima_cantidad_consecutivos   = utils::strToInt(config.getValue("minima_cantidad_consecutivos"));
    unsigned int    intervalo_dibujo               = utils::strToInt(config.getValue("intervalo_dibujo"));

    //Impresion de los datos de ejecucion
    std::cout<<"Bienvenidos al Ejercicio 1 \n";
    std::cout<<"Problema: "<<archivo_problema<<'\n';
    std::cout<<"Cantidad de epocas = "<<criterio_max_epocas<<'\n';
    std::cout<<"Tasa de aprendizaje = "<<tasa_aprendizaje<<'\n';
    std::cout<<"Criterio de finalizacion: "<<criterio_finalizacion<<'\n';
    
    std::cout<<"Porcentaje para entrenamiento = "<<porcentaje_entrenamiento<<"\%\n";
    std::cout<<"Porcentaje para validacion = "<<100-porcentaje_entrenamiento-porcentaje_prueba<<"\%\n";
    std::cout<<"Porcentaje para prueba = "<<porcentaje_prueba<<"\%\n";

	//Inicializamos y configuramos el Graficador
    //Graficador para el error
    GNUPlot plotter;	
	plotter("set pointsize 1");
	plotter("set grid back");	
	plotter("set xzeroaxis lt -1");
	plotter("set yzeroaxis lt -1");	
	plotter("set xrange [0:"+utils::intToStr(criterio_max_epocas+1)+"]");
	plotter("set yrange [-0.1:100]");
    plotter("set xlabel \"Epocas\"");
    plotter("set ylabel \"Error\"");
    //plotter("set format y \"\%g \%\%\""); //formato porcentaje en ylabel
    plotter("set title \"Error durante N Epocas\"");
    plotter("set multiplot");

    //Graficador para la dispersion de puntos
	GNUPlot plotter2;	
	plotter2("set xzeroaxis lt -1");
	plotter2("set yzeroaxis lt -1");	
	//plotter2("set xrange [-4:4]"); plotter2("set yrange [-4:4]");
	plotter2("set xrange [-0.1:1.1]"); plotter2("set yrange [-0.1:1.1]");
    plotter2("set multiplot");
	plotter2("set grid back");	
	plotter2("set pointsize 1");
    
	//Vectores temporales para trabajar
	std::vector<std::vector<float > > patron, entrenamiento, prueba, validacion;
	
    //Vectores temporales para guardar historial errores
	std::vector<float> error_history_entrenamiento;

	//Leo los patrones en patron
	utils::parseCSV(archivo_problema.c_str(), patron);
    patron = utils::genPatrones(patron, cantidad_casos, desvio);

	random_shuffle(patron.begin() , patron.end());
/*
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
*/					
	    
    //Instancio la red
    RedRBF redRBF("estructura1.txt","Red RBF", tasa_aprendizaje, Neurona::FUNCION_SIGMOIDEA, parametro_sigmoidea);

    //Genera las particiones de entrenamiento y prueba
	utils::genParticiones(patron, entrenamiento, validacion, prueba, porcentaje_entrenamiento, 
			porcentaje_prueba, 0);


    //Divido en X y Yd los casos de entrenamiento
    std::vector<std::vector<float> > X, Yd;
    
    utils::splitVector(entrenamiento, X, Yd);

    unsigned int i = 0; //contador de epocas
    std::vector<float> errores_consecutivos; //usado para calcular los errores consecutivos
	for (; i < criterio_max_epocas; i++) {

        std::vector<std::vector<float> > ultimas_salidas;
        //Entrena y calcula error
        redRBF.train(X, Yd, true);

        //obtener los centroides de cada neurona
        std::vector<std::vector<float> > centroides = redRBF.getMus();
        for (unsigned int k = 0; k < centroides.size(); k++) {
            std::cout<<"Centroide "<<k<<" = ";
            utils::printVector(centroides[k]);
        }
        
        float error = 1-redRBF.train(X, Yd, false);
        std::cout<<"Error = "<<error*100<<"\%\n";


        std::getchar();
        
        /*
        if(i < intervalo_dibujo || i % intervalo_dibujo == 0) {
                    //Graficador para la dispersion de puntos
            plotter2("clear\n");
            utils::drawPlot(X, Yd, ultimas_salidas, plotter2);
        }

		error_history_entrenamiento.push_back(error); 
				
        //Comprueba si se llego a un error muy chico
        if (((criterio_finalizacion.compare("error") == 0) || criterio_finalizacion.compare("todos") == 0) && 
                fabs(error) < criterio_error) {
            std::cout<<"Se termino el entrenamiento temprano a las "<<i<<" epocas porque se llego a un error inferior al "<<criterio_error<<'\n';
            break;
        }
	    
        //Comprueba si el error no varió mucho en iteraciones consecutivas
        if (((criterio_finalizacion.compare("consecutivo") == 0) ||
                criterio_finalizacion.compare("todos") == 0) && 
                (errores_consecutivos.size() > minima_cantidad_consecutivos)) { 

            //Se crea un nuevo vector con el error actualizado
            std::vector<float> errores_nuevo (errores_consecutivos); //crea una copia
            errores_nuevo.erase(errores_nuevo.begin()); //borra el mas viejo
            errores_nuevo.push_back(error); //inserta el nuevo

            //Mide el parecido entre dos vectores
            bool parecido = utils::vectorParecido(errores_nuevo, errores_consecutivos, criterio_error_consecutivo);

            //si cumple el criterio
            if (parecido) {
                std::cout<<"Se termino el entrenamiento temprano a las "<<i<<" epocas porque se llego, luego de "<<
                    minima_cantidad_consecutivos<<" iteraciones consecutivas, a un parecido mayor al "<<criterio_error_consecutivo<<'\n';
                break;
            }
            //se actualiza el vector de errores
            errores_consecutivos = errores_nuevo;
        } else {
            //guarda el error de esta iteracion
            errores_consecutivos.push_back(error);
        }*/
    }
    std::cout<<"Entrenamiento finalizado a las "<<i<<" epocas.\n"; 
    
    //Dibuja los errores
    
    //Obtiene el maximo de los errores
    float max_val = *(std::max_element(error_history_entrenamiento.begin(), error_history_entrenamiento.end()));
    
    std::vector<float> errores_normalizados;
    if (fabs(max_val) > criterio_error) { //si el error es 0, no normalizo nada
    //normaliza los errores
        utils::vectorEscalar(error_history_entrenamiento, 1/max_val, errores_normalizados);
    } else {
        errores_normalizados = error_history_entrenamiento;
    }
    
    //Haremos un string para poder plotear al final		
	std::string plot2 = "plot \"-\" notitle pt 5 lt 3\n";

	for (unsigned int k = 0; k < errores_normalizados.size(); k++) {
		plot2 += utils::intToStr((int) k) + " " + utils::floatToStr(errores_normalizados[k]*100.0) + " \n";
    }

    plot2 += "e\n";
    unsigned int max_value = (unsigned int) 100*max_val;
	plotter("set xrange [0:" + utils::intToStr(i + 2) +"]");
	plotter("set yrange [0:"+ utils::floatToStr(max_val*100) +"]");
    
    //plotter(plot2);
	
	//Prueba con los patrones nunca vistos

	//Cargo el conjunto de prueba
	utils::splitVector(prueba, X, Yd, 1); 
    
	
	float error_esperado = 1-redRBF.train(X, Yd, false);
	std::cout<<"Error Esperado (conjunto de prueba) = "<<error_esperado*100.0<<"\%\n";
	
    return 0;
}

