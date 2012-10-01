#include <vector>
#include <ctime>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "utils.h"
#include "RedRBF.h"
#include "GNUPlot.h"
  
int main() {
    //Inicializo la semilla
    srand(time(NULL));
    
    
    //defino parametros
    unsigned int cantidad_epocas = 100;
    unsigned int maxit_kmeans = 30;
    unsigned int cantidad_entradas = 5;
    unsigned int cantidad_rbf = 8;
    unsigned int cantidad_perceptron = 2;
    float tasa_aprendizaje = 0.3001;
    std::string archivo_problema = "phoneme.csv";
    unsigned int cantidad_salidas = 1;

    //leo los patrones
    std::vector<std::vector<float> > patrones, entrenamiento, prueba, validacion, X, Yd;
    utils::parseCSV(archivo_problema, patrones);
    
    utils::genParticiones(patrones, entrenamiento, validacion, prueba, 75, 25, 0);

    //los separo
    utils::splitVector(entrenamiento, X, Yd, cantidad_salidas);

    //codifico las salidas
    std::vector<std::vector<float> > Yconvertida;
    utils::convertirSalida(Yd, Yconvertida);
    Yd = Yconvertida;
    
    //construyo la red
    //(entradas, rbf, perceptron, eta)
    RedRBF redRBF(cantidad_entradas, cantidad_rbf, cantidad_perceptron, tasa_aprendizaje);
  
    //Ejecuto KMEANS para encontrar los centroides
    redRBF.kmeans(X, maxit_kmeans);

    std::vector<std::vector<float> > medias = redRBF.getMedias();
    std::cout<<"Centroides:\n";
    utils::printVectorVector(medias);

    std::vector<float> varianzas = redRBF.getVarianzas();
    std::cout<<"Varianzas:\n";
    utils::printVector(varianzas);
    std::cout<<'\n';
    
    std::vector<float> errores;
    for (unsigned int i = 0; i < cantidad_epocas; i++) {
        float error = redRBF.train(X, Yd);
        errores.push_back(error);
    }

    //Inicializamos y configuramos el Graficador
    //Graficador para el error
    GNUPlot plotter;	
    plotter("set pointsize 1");
    plotter("set grid back");	
    plotter("set xzeroaxis lt -1");
    plotter("set yzeroaxis lt -1");	
    plotter("set xrange [0:100]");
    plotter("set yrange [-0.1:1.1]");
    plotter("set xlabel \"Epocas\"");
    plotter("set ylabel \"Error\"");
    plotter("set title \"Error durante N Epocas\"");
    plotter("set multiplot");
    
    //Haremos un string para poder plotear al final		
    std::string plot2 = "plot \"-\" notitle pt 2 lt 3\n";

    for (unsigned int k = 0; k < errores.size(); k++) {
        plot2 += utils::intToStr((int) k) + " " + utils::floatToStr(errores[k]) + " \n";
    }
    plot2 += "e\n";
    plotter(plot2);
    
    /*
    std::vector<std::vector<float> > errores_guardar;
    errores_guardar.push_back(errores);
    utils::saveCSV("errores.csv", errores_guardar);
    std::cout<<"Errores obtenidos = ";
    utils::printVector(errores);
    */
    
    //los separo
    utils::splitVector(prueba, X, Yd, cantidad_salidas);

    //codifico las salidas
    utils::convertirSalida(Yd, Yconvertida);
    Yd = Yconvertida;

    float efectividad = redRBF.test(X, Yd);
    std::cout<<"Efectividad esperada = "<<efectividad*100<<"\%\n";
   
    return 0;
}
