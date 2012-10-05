#include <vector>
#include <ctime>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "utils.h"
#include "RedRBF.h"
#include "GNUPlot.h"
#include "Red.h"
  
int main() {
    //Inicializo la semilla
    srand(time(NULL));
    
    
    std::string archivo_problema = "iris.csv";
    unsigned int cantidad_salidas = 1;
    
    //defino parametros
    unsigned int cantidad_epocas = 50;
    unsigned int maxit_kmeans = 40;
    unsigned int cantidad_entradas = 4;
    unsigned int cantidad_rbf = 10;
    unsigned int cantidad_perceptron = 2;
    float tasa_rbf = 0.01;
    
    
    unsigned int cantidad_epocas_mlp = 100;
    float parametro_sigmoidea = 5;
    float parametro_momento = 0.9;
    float tasa_mlp = 0.01;
    
    //Variables para contar tiempo
    clock_t t_ini, t_fin;
    double secs;

    std::vector<std::vector<float> > patrones, entrenamiento, prueba, validacion, X, Yd;
    std::vector<std::vector<float> > Yconvertida;
    
    //leo los patrones 
    utils::parseCSV(archivo_problema, patrones);
    std::random_shuffle(patrones.begin(), patrones.end());
    utils::genParticiones(patrones, entrenamiento, validacion, prueba, 100, 0, 0);
    //los separo
    utils::splitVector(entrenamiento, X, Yd, cantidad_salidas);

    //codifico las salidas
    utils::convertirSalida(Yd, Yconvertida);
    Yd = Yconvertida;
    
    
    //construyo la red
    //(entradas, rbf, perceptron, eta)
    RedRBF redRBF(cantidad_entradas, cantidad_rbf, cantidad_perceptron, tasa_rbf);
  
    //Ejecuto KMEANS para encontrar los centroides
    t_ini = clock();
    
    //======= ENTRENAMIENTO RBF
    redRBF.kmeans(X, maxit_kmeans);
  
    /*
    std::vector<std::vector<float> > medias = redRBF.getMedias();
    std::cout<<"Centroides:\n";
    utils::printVectorVector(medias);

    std::vector<float> varianzas = redRBF.getVarianzas();
    std::cout<<"Varianzas:\n";
    utils::printVector(varianzas);
    std::cout<<'\n';
    */
    
    std::vector<float> errores_rbf;
    for (unsigned int i = 0; i < cantidad_epocas; i++) {
        float error = redRBF.train(X, Yd);
        errores_rbf.push_back(error);
    }
    t_fin = clock();
    std::cout<<"Tiempo RBF = "<<(double)(t_fin - t_ini) / CLOCKS_PER_SEC<<'\n';
    
    //======= ENTRENAMIENTO PERCEPTRON MULTICAPA
    
    Red perceptron("estructura.txt","Red Perceptron", tasa_mlp, Neurona::FUNCION_SIGMOIDEA, parametro_sigmoidea, parametro_momento);
    
    std::vector<std::vector<float> > ultimas_salidas;
    std::vector<float> errores_mlp;
    t_ini = clock();
    //Entrenamiento de perceptron
    for (unsigned int i = 0; i < cantidad_epocas_mlp; i++) {
	perceptron.train(X, Yd, ultimas_salidas, true);
	float errorsin = 1-perceptron.train(X, Yd, ultimas_salidas, false);
	errores_mlp.push_back(errorsin);
	std::cout<<i<<' '<<errorsin<<'\n';
    }
    t_fin = clock();
    std::cout<<"Tiempo MLP = "<<(double)(t_fin - t_ini) / CLOCKS_PER_SEC<<'\n';
    
    //Pruebas
    //los separo
    utils::splitVector(entrenamiento, X, Yd, cantidad_salidas);

    //codifico las salidas
    utils::convertirSalida(Yd, Yconvertida);
    Yd = Yconvertida;

    float error_rbf = 1-redRBF.test(X, Yd);
    std::cout<<"Error esperado RBF = "<<error_rbf*100<<"\%\n";  

    float error_mlp = 1-perceptron.train(X, Yd, ultimas_salidas, false);
    std::cout<<"Error esperado MLP = "<<error_mlp*100<<"\%\n";
    
    
        //Inicializamos y configuramos el Graficador
    //Graficador para el error
    /*
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
    */
    /*
    std::vector<std::vector<float> > errores_guardar;
    errores_guardar.push_back(errores);
    utils::saveCSV("errores.csv", errores_guardar);
    std::cout<<"Errores obtenidos = ";
    utils::printVector(errores);
    */
    
    
    return 0;
}
