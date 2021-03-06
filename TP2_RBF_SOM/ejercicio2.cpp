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
    unsigned int    cantidad_clases                = utils::strToInt(config.getValue("cantidad_clases"));
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

    GNUPlot plotter2;	
	plotter2("set pointsize 1");
	plotter2("set grid back");	
	plotter2("set xzeroaxis lt -1");
	plotter2("set yzeroaxis lt -1");	
	plotter2("set xrange [-1.1:1.1]");
	plotter2("set yrange [-1.1:1.1]");
    plotter2("set xlabel \"X\"");
    plotter2("set ylabel \"Y\"");
    plotter2("set title \"Puntos del entrenamiento\"");
    plotter2("set multiplot");

    //Vectores temporales para trabajar
	std::vector<std::vector<float > > patron, entrenamiento, prueba, validacion;
	
    //Leo el problema en patron
    if ( archivo_problema.compare("cuadrado") == 0)
        utils::generarCuadrado(cantidad_casos, std::make_pair<float,float>(-1.0,-1.0), std::make_pair<float,float>(1.0,1.0), patron);
    else if ( archivo_problema.compare("circulo") == 0)
        utils::generarCirculo(cantidad_casos, 1.0, patron);
    else if ( archivo_problema.compare("formaT") == 0)
        utils::generarT(cantidad_casos, patron);
    else
        utils::parseCSV(archivo_problema, patron);

    random_shuffle(patron.begin() , patron.end());

    
    //Genera las particiones de entrenamiento y prueba
    utils::genParticiones(patron, entrenamiento, validacion, prueba, porcentaje_entrenamiento, 
            porcentaje_prueba, 0);
    
    
    unsigned int maxit = epocas_fase_ordenamiento + epocas_fase_convergencia;

    //Instancio la red
    RedSOM redSOM(cantidad_entradas, cantidad_clases, alto, ancho, sigma0, eta0, tau2, maxit);


    //Divido en X y Yd los casos de entrenamiento
    std::vector<std::vector<float> > X, Yd;
    
    utils::splitVector(entrenamiento, X, Yd, cantidad_salidas);
    

    std::vector<std::vector<float> > puntosSOM;
    
    redSOM.getPuntos(puntosSOM);
    plotter2("clear\n");
    utils::drawPoints(puntosSOM, plotter2);

    std::vector<std::vector<float> > Ycalculados;

    std::cout<<"Fase de ordenamiento\n";
    for (unsigned int i = 0; i < epocas_fase_ordenamiento; i++) {
        //Entrena
        redSOM.train(X, Yd, Ycalculados, true, true);
        
        if (i % intervalo_dibujo == 0) {
            std::cout<<"ordenamiento iteracion "<<i<<'\n';
            std::vector<std::vector<float> > puntosSOM;
            redSOM.getPuntos(puntosSOM);
            plotter2("clear\n");
            utils::drawPoints(puntosSOM, plotter2);
        }
    }

    std::cout<<"Fin Fase de ordenamiento \n";
    std::cout<<"Fase de convergencia\n";
    //Para convergencia, uso la cantidad de neuronas por el valor en epocas_fase_convergencia
    // epocas_fase_convergencia *= redSOM.getCantidadNeuronas();
    redSOM.setCteAprendizaje(0.01);
    redSOM.setSigma(1);

    redSOM.getPuntos(puntosSOM);
    plotter2("clear\n");
    utils::drawPoints(puntosSOM, plotter2);

    for (unsigned int i = 0; i < epocas_fase_convergencia; i++) {
        //Entrena
        redSOM.train(X, Yd, Ycalculados, true, false);

        //Dibuja si corresponde
        if (i % intervalo_dibujo == 0) {
            std::cout<<"convergencia iteracion "<<i<<'\n';
            std::vector<std::vector<float> > puntosSOM;
            redSOM.getPuntos(puntosSOM);
            plotter2("clear\n");
            utils::drawPoints(puntosSOM, plotter2);
        }
    }

    std::cout<<"Fin Fase de convergencia \n";
    
    redSOM.getPuntos(puntosSOM);
    plotter2("clear\n");
    utils::drawPoints(puntosSOM, plotter2);
  
    std::cout<<"El SOM esta entrenado.\n";

   return 0;
}

