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
Config config("configuracion4.cfg"); //lectura de la configuracion

int main (int argc, char *argv[]) {
	//inicializacion de semilla
	srand( (unsigned) std::time(NULL)); 
	
	//Leemos los valores de configuracion
	std::string     archivo_problema               = config.getValue("archivo_problema"); //Archivo a leer patrones ej xor.csv
	float           tasa_aprendizaje               = utils::strToFloat(config.getValue("tasa_aprendizaje"));
	unsigned int    criterio_max_epocas            = utils::strToInt(config.getValue("criterio_max_epocas"));
	float           criterio_error                 = utils::strToFloat(config.getValue("criterio_error"));
    float           parametro_sigmoidea            = utils::strToFloat(config.getValue("parametro_sigmoidea"));
    float           parametro_momento              = utils::strToFloat(config.getValue("parametro_momento"));
    float           criterio_error_consecutivo     = utils::strToFloat(config.getValue("criterio_error_consecutivo"));
    std::string     criterio_finalizacion          = config.getValue("criterio_finalizacion");
    unsigned int    minima_cantidad_consecutivos   = utils::strToInt(config.getValue("minima_cantidad_consecutivos"));
    unsigned int    leave_k_out                    = utils::strToInt(config.getValue("leave_k_out"));

    //Impresion de los datos de ejecucion
    std::cout<<"Bienvenidos al Ejercicio 4 \n";
    std::cout<<"Problema: "<<archivo_problema<<'\n';
    std::cout<<"Cantidad de epocas = "<<criterio_max_epocas<<'\n';
    std::cout<<"Criterio de finalizacion: "<<criterio_finalizacion<<'\n';
    std::cout<<"Tasa de aprendizaje = "<<tasa_aprendizaje<<'\n';
    std::cout<<"Parametro Momento = "<<parametro_momento<<'\n';
    std::cout<<"Leave K Out = "<<leave_k_out<<'\n';

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
    plotter("set format y \"\%g \%\%\""); //formato porcentaje en ylabel
    plotter("set title \"Error durante N Epocas\"");
    plotter("set multiplot");

    
	//Vectores temporales para trabajar
	std::vector<std::vector<float > > patron;
	
    //Vectores temporales para guardar historial errores
	std::vector<float> error_history_entrenamiento;

	//Leo los patrones en patron
	utils::parseCSV(archivo_problema.c_str(), patron);

    unsigned int cantidad_casos = patron.size();
    std::cout<<"Cantidad de casos = "<<cantidad_casos<<'\n';
	random_shuffle(patron.begin() , patron.end());
   
    std::vector<std::vector<float> > X, Yd;
    std::vector<std::vector<float> > Ycodificados;
    
    //Aqui se guardaran los errores de cada una de las k pruebas del leave-k-out method
    std::vector<float> errores_lko;
    
    unsigned int cantidad_subconjuntos = cantidad_casos/leave_k_out; //division entera, REVEER
    for (unsigned int j = 0; j < cantidad_subconjuntos; j++) {
   
        //Instancio la red
        Red perceptron("estructura4.txt","Red Perceptron", tasa_aprendizaje, Neurona::FUNCION_SIGMOIDEA, parametro_sigmoidea, parametro_momento);
      
	    std::vector<std::vector<float> > entrenamiento, validacion;

        //Genero el conjunto de entrenamiento
        entrenamiento = patron;

        //REVEER
        std::vector<std::vector<float> >::iterator p = entrenamiento.begin()+j*leave_k_out; //inicio del tramo
        std::vector<std::vector<float> >::iterator q = entrenamiento.begin()+(j+1)*leave_k_out; //fin del tramo
    
        if (q > entrenamiento.end()) q = entrenamiento.end(); //fix si se fue de rango

        //Inserto k valores para validacion
        validacion.insert   (validacion.begin(), p, q); 
        //Borro los valores usados para validacion
        entrenamiento.erase (p, q); 

        //Divido en X y Yd los casos de entrenamiento
        utils::splitVector(entrenamiento, X, Yd, 1);

        //Decodifica las salidas
        utils::convertirSalida(Yd, Ycodificados);
        Yd = Ycodificados;

        unsigned int i = 0; //contador de epocas
        std::vector<float> errores_consecutivos; //usado para calcular los errores consecutivos

        plotter("clear\n");
        //Inicializo el ploteo
    	//Haremos un string para poder plotear al final		
	    std::string plot = "plot \"-\" notitle pt 2 lt 3\n";
        for (; i < criterio_max_epocas; i++) {

            //Entrena y calcula error
            float error = 1-perceptron.train(X, Yd, true);

            error_history_entrenamiento.push_back(error); 
                    
            //Dibuja el error
            plot += utils::intToStr((int) i) + " " + utils::floatToStr(error*100.0) + " \n";
           
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
            }
        }
        //std::cout<<"Entrenamiento finalizado a las "<<i<<" epocas.\n"; 
        
        //Actualizacion del dibujo
        plot += "e\n";
    	plotter("set xrange [-1:" + utils::intToStr(i + 2) +"]");
        //plotter(plot);

    	//Cargo el conjunto de validacion
	    utils::splitVector(validacion, X, Yd, 1); 
        
        //Decodifica las salidas
        utils::convertirSalida(Yd, Ycodificados);
        Yd = Ycodificados;

        //Prueba con los patrones nunca vistos
    	float error_esperado = 1-perceptron.train(X, Yd, false);
    	std::cout<<"Error Esperado (conjunto de validacion "<<j<<") = "<<error_esperado*100.0<<"\%\n";

        //Guardo el error para sacar promedio al final
        errores_lko.push_back(error_esperado);
        //std::getchar();
    }
    
	float media = utils::promedio(errores_lko);
    float devest = utils::devest(errores_lko, media);

    std::cout<<"Errores obtenidos en las k pruebas: ";
    utils::printVector(errores_lko);
    std::cout<<"Valores medidos luego de las k pruebas (errores):\n";
    std::cout<<"Media = "<<media<<"\nDesviacion estandar = "<<devest<<"\n";


    return 0;
}

