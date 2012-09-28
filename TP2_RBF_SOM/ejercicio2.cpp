#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <utility>
#include "utils.h"
#include "RedSOM.h"
#include "Config.h"

#include "GNUPlot.h"

//Variable global
Config config("configuracion2.cfg"); //lectura de la configuracion

int main (int argc, char *argv[]) {
	//inicializacion de semilla
	srand( (unsigned) std::time(NULL)); 
	
	//Leemos los valores de configuracion
    //Parametros de los patrones de entrenamiento y prueba
	std::string     archivo_problema               = config.getValue("archivo_problema"); //Archivo a leer patrones ej xor.csv
	unsigned int    porcentaje_entrenamiento       = utils::strToInt(config.getValue("porcentaje_entrenamiento"));
	unsigned int    porcentaje_prueba              = utils::strToInt(config.getValue("porcentaje_prueba"));
	unsigned int    cantidad_casos                 = utils::strToInt(config.getValue("cantidad_casos"));
    //Parametros de la red SOM
    unsigned int    alto                           = utils::strToInt(config.getValue("alto"));
    unsigned int    ancho                          = utils::strToInt(config.getValue("ancho"));
    unsigned int    cantidad_entradas              = utils::strToInt(config.getValue("cantidad_entradas"));
    unsigned int    cantidad_salidas               = utils::strToInt(config.getValue("cantidad_salidas"));
	float           eta0                           = utils::strToFloat(config.getValue("eta0"));
	float           sigma0                         = utils::strToFloat(config.getValue("sigma0"));
	float           tau2                           = utils::strToFloat(config.getValue("tau2"));
	//Parametros para el entrenamiento
    unsigned int    epocas_fase_ordenamiento       = utils::strToInt(config.getValue("epocas_fase_ordenamiento"));
    unsigned int    epocas_fase_convergencia       = utils::strToInt(config.getValue("epocas_fase_convergencia"));
    unsigned int    intervalo_dibujo               = utils::strToInt(config.getValue("intervalo_dibujo"));
    

    //Impresion de los datos de ejecucion
    std::cout<<"Bienvenidos al Ejercicio 2 \n";
    std::cout<<"Problema: "<<archivo_problema<<'\n';
    std::cout<<"Porcentaje para entrenamiento = "<<porcentaje_entrenamiento<<"\%\n";
    std::cout<<"Porcentaje para validacion = "<<100-porcentaje_entrenamiento-porcentaje_prueba<<"\%\n";
    std::cout<<"Porcentaje para prueba = "<<porcentaje_prueba<<"\%\n";

	//Inicializamos y configuramos el Graficador
    //Graficador para el error
    /*
    GNUPlot plotter;	
	plotter("set pointsize 1");
	plotter("set grid back");	
	plotter("set xzeroaxis lt -1");
	plotter("set yzeroaxis lt -1");	
//	plotter("set xrange [0:"+utils::intToStr(criterio_max_epocas+1)+"]");
	plotter("set yrange [-0.1:100]");
    plotter("set xlabel \"Epocas\"");
    plotter("set ylabel \"Error\"");
    //plotter("set format y \"\%g \%\%\""); //formato porcentaje en ylabel
    plotter("set title \"Error durante N Epocas\"");
    plotter("set multiplot");
*/
    GNUPlot plotter2;	
	plotter2("set pointsize 1");
	plotter2("set grid back");	
	plotter2("set xzeroaxis lt -1");
	plotter2("set yzeroaxis lt -1");	
	plotter2("set xrange [-1.2:1.2]");
	plotter2("set yrange [-1.2:2]");
    plotter2("set xlabel \"X\"");
    plotter2("set ylabel \"Y\"");
    //plotter2("set format y \"\%g \%\%\""); //formato porcentaje en ylabel
    plotter2("set title \"Puntos del entrenamiento\"");
    plotter2("set multiplot");

    //Vectores temporales para trabajar
	std::vector<std::vector<float > > patron, entrenamiento, prueba, validacion;
	
    //Leo el problema en patron
    if ( archivo_problema.compare("cuadrado") == 0)
        utils::generarCuadrado(cantidad_casos, std::make_pair<float,float>(-1.0,-1.0), std::make_pair<float,float>(1.0,1.0), patron);
    if ( archivo_problema.compare("circulo") == 0)
        utils::generarCirculo(cantidad_casos, 1.0, patron);
    if ( archivo_problema.compare("formaT") == 0)
        utils::generarT(cantidad_casos, patron);

    random_shuffle(patron.begin() , patron.end());

    //Instancio la red
    RedSOM redSOM(cantidad_entradas, alto, ancho, sigma0, eta0, tau2 );

    //Genera las particiones de entrenamiento y prueba
    utils::genParticiones(patron, entrenamiento, validacion, prueba, porcentaje_entrenamiento, 
            porcentaje_prueba, 0);

    //Divido en X y Yd los casos de entrenamiento
    std::vector<std::vector<float> > X, Yd;
    
    utils::splitVector(entrenamiento, X, Yd, cantidad_salidas);

    //utils::drawPoints(X, plotter2);

    //Vector temporales para guardar historial errores
    std::vector<float> error_history_entrenamiento;

    std::vector<std::vector<float> > puntosSOM;
    
    redSOM.getPuntos(puntosSOM);
    plotter2("clear\n");
    utils::drawPoints(puntosSOM, plotter2);

    std::cout<<"Fase de ordenamiento\n";
    for (unsigned int i = 0; i < epocas_fase_ordenamiento; i++) {
        //Entrena
        redSOM.train(X, Yd, true, true);
        /*
        if (i % intervalo_dibujo == 0) {
            std::vector<std::vector<float> > puntosSOM;
            redSOM.getPuntos(puntosSOM);
            //utils::printVectorVector(puntosSOM);
            plotter2("clear\n");
            utils::drawPoints(puntosSOM, plotter2);
        }
          */      
    }

    std::cout<<"Fin Fase de ordenamiento \n";
    std::cout<<"Fase de convergencia\n";
    //Para convergencia, uso la cantidad de neuronas por el valor en epocas_fase_convergencia
    // epocas_fase_convergencia *= redSOM.getCantidadNeuronas();
    redSOM.setCteAprendizaje(0.01);
    
    redSOM.getPuntos(puntosSOM);
    plotter2("clear\n");
    utils::drawPoints(puntosSOM, plotter2);

    for (unsigned int i = 0; i < epocas_fase_convergencia; i++) {
        //Entrena
        redSOM.train(X, Yd, true, false);

        /*
        //Dibuja si corresponde
        if (i % intervalo_dibujo == 0) {
            std::vector<std::vector<float> > puntosSOM;
            redSOM.getPuntos(puntosSOM);
            //utils::printVectorVector(puntosSOM);
            plotter2("clear\n");
            utils::drawPoints(puntosSOM, plotter2);
        }
        */
    }

    std::cout<<"Fin Fase de convergencia \n";
    
    redSOM.getPuntos(puntosSOM);
    plotter2("clear\n");
    utils::drawPoints(puntosSOM, plotter2);
  /* 
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
		plot2 += utils::intToStr((int) k) + " " + utils::floatToStr(errores_normalizados[k]) + " \n";
    }

    plot2 += "e\n";
    unsigned int max_value = (unsigned int) 100*max_val;
	plotter("set xrange [0:" + utils::intToStr(i + 2) +"]");
	plotter("set yrange [0:1]");
	//plotter("set yrange [0:"+ utils::floatToStr(max_val*100) +"]");
    
    plotter(plot2);
	
	//Prueba con los patrones nunca vistos

	//Cargo el conjunto de prueba
	utils::splitVector(prueba, X, Yd, cantidad_salidas); 
    
	
	float error_esperado = 1-redSOM.train(X, Yd, false);
	std::cout<<"Error Esperado (conjunto de prueba) = "<<error_esperado*100.0<<"\%\n";
	*/
    return 0;
}

