#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <algorithm>

#include "utils.h"
#include "RedRBF.h"

RedRBF::RedRBF (std::string nombre_archivo, std::string nombre_red, float tasa_aprendizaje, float sigma, unsigned int funcion_activacion, float par_sigmoidea ) {
    //Leo los parametros de la red desde un archivo
    std::ifstream file (nombre_archivo.c_str());
    assert(file.is_open());
    unsigned int c_e, c_rbf, c_n;
    file>>c_e>>c_rbf>>c_n;
    file.close();
    
    std::cout<<"Parametros de la red: "<<c_e<<' '<<c_rbf<<' '<<c_n<<'\n';

    //Asigno la cantidad de cada uno de los parametros de la red
    this->cantidad_entradas = c_e;
    this->cantidad_rbf = c_rbf;
    this->cantidad_n = c_n;
    this->cantidad_salidas = c_n;
    this->parametro_sigmoidea = par_sigmoidea;
    
    //Creo las neuronas tipo rbf
    for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
        NeuronaRBF neuRBF(this->cantidad_entradas, sigma, -1, 1);
        this->neuronasRBF.push_back(neuRBF);
    }

    //Creo las neuronas tipo perceptron
    for (unsigned int i = 0; i < this->cantidad_n; i++) {
        Neurona neu (this->cantidad_rbf, -0.5, 0.5, funcion_activacion, tasa_aprendizaje) ;
        this->neuronasP.push_back(neu);
    }

    /*
    //Inicializo todos los valores del objeto para dibujar 
    this->plotter("set xzeroaxis lt -1");
    this->plotter("set yzeroaxis lt -1"); 
    //this->plotter2("set xrange [-4:4]"); plotter2("set yrange [-4:4]");
    this->plotter("set xrange [-1.5:1.5]"); plotter("set yrange [-1.5:1.5]");
    this->plotter("set multiplot");
    this->plotter("set grid back");   
    this->plotter("set pointsize 1");
    */
}

float RedRBF::train(std::vector<std::vector<float> > X, std::vector<std::vector<float> > YD, bool entrena) {
    //Asegura que sean del mismo tamaño
    assert(X.size() == YD.size());

    //Llama a kmeans para aproximar los centroides
    if(entrena == true)
        this->kmeans(X);
    
    unsigned int cantidad_casos = X.size();

    // unsigned int aciertos = 0; //contador de aciertos

    //Aqui se guardan los errores instantaneos
    std::vector<float> error_energia;
    
    //recorro todas las entradas
    for (unsigned int w = 0; w < cantidad_casos; w++) {
        //Entrenamos con el patron actual
        //bool correcto = 
        singleTrain(X[w], YD[w], entrena);
        
        //Si fue correcto, aumentamos la cuenta
        // if (correcto) aciertos++;

        float error_inst = 0.5*utils::vectorPunto(this->error_instantaneo, this->error_instantaneo);
        error_energia.push_back(error_inst);
    }
    
    float promedio_error = utils::promedio(error_energia);
    
    //Calculamos el porcentaje de efectividad
    //float efectividad = ((float) aciertos) / ((float) cantidad_casos);

    return 1-promedio_error;
}

void RedRBF::singleTrain(std::vector<float> X, std::vector<float> YD, bool entrena) {
    // bool salida_sin_error = true;

    std::vector<float> respuestasRBF; //guarda las respuestas de cada una de las neuronasRBF

    //inserto el patron actual en cada una de las RBF y capturo sus salidas
    for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
        //Capturo su respuesta
        float respuesta = this->neuronasRBF[i].getResponse(X);
        //La guardo en el vector de respuestas
        respuestasRBF.push_back(respuesta);
    }
   
    //Limpio el vector de errores
    this->error_instantaneo.clear();

    //inserto la salida de las RBF a las neuronas tipo perceptron,  capturo sus salidas y entreno
    for (unsigned int i = 0; i < this->cantidad_n; i++) {
       
        //Capturo su salida
        float respuesta = this->neuronasP[i].getResponse(respuestasRBF);
   
        //std::cout<<YD[i]<<' '<<respuesta<<'\n';

        //Calculo el error
        float error = respuesta - YD[i];
        
        //Guardo el error en un vector
        this->error_instantaneo.push_back(error); 
       
        // Verifico si hay error en alguna salida
        // if ((salida_sin_error == true) && (fabs(error) > EPS_ERROR)) { //no hubo error aun y son != (hay un error)
        //     salida_sin_error = false;
        // }
 
        if (entrena) {
            //Completo la parte escalar
            float parte_escalar = -1.0*error*this->neuronasP[i].getConstanteAprendizaje();
                    
            //Realizamos una copia de las respuestas
            std::vector<float> copiarespuestas = respuestasRBF;
            
            //Agregamos la entrada correspondiente al Bias \phi_0 = 1
            copiarespuestas.insert(copiarespuestas.begin(), 1.0);

            //Calculo el nuevo W
            std::vector<float> vesc;
            utils::vectorEscalar(copiarespuestas, parte_escalar, vesc);

            //Obtengo su vector de pesos
            std::vector<float> Wi = this->neuronasP[i].getW();
            
            std::vector<float> Wnuevo;
            utils::vectorSuma(Wi, vesc, Wnuevo);
            
            this->neuronasP[i].setW( Wnuevo );
        }
                
    }
    // return salida_sin_error;
}

void RedRBF::kmeans(std::vector<std::vector<float> > entradas) {
   
    
    //Inicializo el kmeans con patrones aleatorios del conjunto a clusterear
    std::vector<std::vector<std::vector<float> > > conjuntos;
    conjuntos.resize(this->cantidad_rbf);
    for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
        std::random_shuffle(entradas.begin(), entradas.end());
        std::vector<float> V = entradas[0];
        neuronasRBF[i].setMu(V);
    }

    //guardo los centroides viejos para comparar y salir del while true
    std::vector<std::vector<float> > centroides_viejos = getMus();

    unsigned int iteraciones = 0;

    //Este vector se va a utilizar para estimar los sigmas, una vez terminado el proceso
    std::vector<std::vector<std::vector<float> > > conjuntos_sigma;
    while (true) { //hasta que el delta_mu sea menor que un EPS
  
        //vector de conjuntos de puntos
        //En cada conjuntos[i] se almacena la posicion dentro de 
        //  entradas de cada uno de los puntos que pertenecen al conjunto i
        conjuntos.clear();
        conjuntos.resize(this->cantidad_rbf);
       
        unsigned int cantidad_entradas = entradas.size();
        for (unsigned int w = 0; w < cantidad_entradas; w++) { //para cada patron
            std::vector<float> distancias; //aqui se guardaran las distancias de este caso a todas las neuronas
           
            //mido distancias a cada neuronaRBF
            for (unsigned int k = 0; k < this->cantidad_rbf; k++) { //recorro las neuronas RBF
                
                //Le pido el centro actual a la neurona k
                std::vector<float> V = neuronasRBF[k].getMu();
                
                //calculo la distancia entre la neurona y el patron w
                float dist = utils::vectorDistancia(entradas[w], V);
                //uso distancia cuadrada
                dist *= dist;
                
                distancias.push_back(dist);
            }
            
            //obtengo donde ocurrio la menor de las distancias 
            unsigned int indice_menor = utils::getMinIdx(distancias); //este patron tiene esta clase
            //Agrego el patron a la clase indice_menor
            conjuntos[indice_menor].push_back(entradas[w]);
        }

        /*
        //Dibuja centroides y conjuntos por color
        for (unsigned int m=0; m < conjuntos.size(); m++) {
            std::vector<std::vector<float> > temp;
            temp.push_back(centroides_viejos[m]);
            if (conjuntos[m].size() > 0) {
                utils::drawPoints(conjuntos[m], this->plotter, m);
            }
            utils::drawPoints(temp, this->plotter, conjuntos.size() + 1, 3);
        }
        getchar();
        this->plotter("clear\n");
        */

        //recalcular centroide
        for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
            unsigned int c_size = conjuntos[i].size(); //cantidad de patrones en esta clase
            
            if (c_size == 0) continue; //si no tengo ningun patron en esta clase, continuo

            std::vector<float> sumas = conjuntos[i][0]; //asigno el primero
           
            //sumo todos los patrones
            for (unsigned int j = 1; j < c_size; j++) {
                std::vector<float> temp;
                utils::vectorSuma(sumas, conjuntos[i][j], temp);
                sumas = temp;
            }
            //divido por la cantidad de patrones
            std::vector<float> centroide_nuevo;
            utils::vectorEscalar(sumas, 1.0/((float)c_size), centroide_nuevo);
                
            this->neuronasRBF[i].setMu(centroide_nuevo);
        }

        //Calculo las distancias entre los mu nuevos y los viejos
        std::vector<float> distancias_ceros;
        for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
            std::vector<float> mu_i = this->neuronasRBF[i].getMu();
            //std::cout<<"Distancias: \n"; utils::printVector(mu_i); utils::printVector(centroides_viejos[i]);
            float dist_i = utils::vectorDistancia(mu_i, centroides_viejos[i]);
            //dist_i *= dist_i;
            distancias_ceros.push_back(dist_i);
        }

        //Sumo todas las distancias obtenidas
        float suma_distancias = 0.0;
        for (unsigned int i = 0; i < distancias_ceros.size(); i++) {
            suma_distancias += distancias_ceros[i];
        }
        suma_distancias /= (float) distancias_ceros.size();

        //Si la suma es muy chica, quiere decir que se movieron poco, salgo del while true
        if (fabs(suma_distancias) < EPS_KMEANS) {
            conjuntos_sigma = conjuntos ; //guardo la ultima disposicion de conjuntos para estimar los sigmas
            break;
        }
       
        //actualizo los centroides viejos
        centroides_viejos = getMus();
        iteraciones++;
    }

    //std::cout<<"Termino el K-Means luego de "<<iteraciones<<" iteraciones.\n";
    
    //Actualizaremos las varianzas de las RBF de acuerdo a la formula:
    //sigma_j^2 = \frac{1}{M_j} \sum (\norm x - mu_j \norm )^2
    for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
        
        unsigned int M_i = conjuntos_sigma[i].size();
        if(M_i == 0) continue; //no hay patrones en este conjunto, no hago nada

        //Capturo el centroide i
        std::vector<float> centroide = this->neuronasRBF[i].getMu();
        //Sumo
        float suma = 0.0;
        for (unsigned int j = 0; j < M_i; j++) {
            //Mido distancia entre el patron j y el centroide i
            float distancia = utils::vectorDistancia(conjuntos[i][j], centroide);
            //guardo el valor
            suma += distancia*distancia;
        }
        //divido por la cantidad de patrones asociados a este centroide
        float nueva_varianza = suma/M_i;
        //asigno la nueva varianza
        this->neuronasRBF[i].setVarianza(nueva_varianza);
    }
}


//Devuelve un vector con todos los mu de esta red
std::vector<std::vector<float> > RedRBF::getMus(){
    std::vector<std::vector<float> > ret_val;
    for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
        std::vector<float> mu_i = this->neuronasRBF[i].getMu();
        ret_val.push_back(mu_i);
    }
    return ret_val;

}


