#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <algorithm>

#include "utils.h"
#include "RedRBF.h"

RedRBF::RedRBF (std::string nombre_archivo, std::string nombre_red, float tasa_aprendizaje, unsigned int funcion_activacion, float par_sigmoidea ) {
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
        NeuronaRBF neuRBF(this->cantidad_entradas, -1, 1);
        this->neuronasRBF.push_back(neuRBF);
    }

    //Creo las neuronas tipo perceptron
    for (unsigned int i = 0; i < this->cantidad_n; i++) {
        Neurona neu (this->cantidad_rbf, -0.5, 0.5, funcion_activacion, tasa_aprendizaje) ;
        this->neuronasP.push_back(neu);
    }
}

float RedRBF::train(std::vector<std::vector<float> > X, std::vector<std::vector<float> > YD, bool entrena) {
    //Asegura que sean del mismo tamaño
    assert(X.size() == YD.size());

    //Llama a kmeans para aproximar los centroides
    if(entrena == true)
        this->kmeans(X);
    
    unsigned int cantidad_casos = X.size();

    unsigned int aciertos = 0; //contador de aciertos

    //recorro todas las entradas
    for (unsigned int w = 0; w < cantidad_casos; w++) {
        //Entrenamos con el patron actual
        bool correcto = singleTrain(X[w], YD[w], entrena);
        //Si fue correcto, aumentamos la cuenta
        if (correcto)
            aciertos++;
    }
    //Calculamos el porcentaje de efectividad
    float efectividad = ((float) aciertos) / ((float) cantidad_casos);

    return efectividad;
}

bool RedRBF::singleTrain(std::vector<float> X, std::vector<float> YD, bool entrena) {
    bool salida_sin_error = true;

    std::vector<float> respuestasRBF; //guarda las respuestas de cada una de las neuronasRBF

    //inserto el patron actual en cada una de las RBF y capturo sus salidas
    for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
        //Capturo su respuesta
        float respuesta = this->neuronasRBF[i].getResponse(X);
        //La guardo en el vector de respuestas
        respuestasRBF.push_back(respuesta);
    }
    
    //inserto la salida de las RBF a las neuronas tipo perceptron,  capturo sus salidas y entreno
    for (unsigned int i = 0; i < this->cantidad_n; i++) {
        //Capturo su salida
        float respuesta = this->neuronasP[i].getResponse(respuestasRBF, this->parametro_sigmoidea);
            
        //Obtengo los pesos sinápticos actuales
        std::vector<float> Wi = this->neuronasP[i].getW();

        // Verifico si hay error en alguna salida
        float error = fabs(YD[i] - respuesta);
       
        if ((salida_sin_error == true) && (error > EPS)) { //no hubo error aun y son != (hay un error)
            salida_sin_error = false;
        }


        //Calculo de los nuevos pesos
        //Parte Escalar
        float parte_escalar = (YD[i] - respuesta) * ( this->neuronasP[i].getConstanteAprendizaje() );

        //Realizamos una copia de las respuestas
        std::vector<float> copiarespuestas = respuestasRBF;
        //Agregamos la entrada correspondiente al Bias
        copiarespuestas.insert(copiarespuestas.begin(), -1);
        
        //Temporal para el producto
        std::vector<float> vesc; 
        utils::vectorEscalar(copiarespuestas, parte_escalar, vesc);

        //Temporal para la suma
        std::vector<float> Wnuevo; 
        utils::vectorSuma(Wi, vesc, Wnuevo);

        //Actualizar pesos
        if (entrena) {
            this->neuronasP[i].setW( Wnuevo );
        }
    }
    return salida_sin_error;
}
/*
void RedRBF::kmeans(std::vector<std::vector<float> > entradas) {

    unsigned int cantidad_casos = entradas.size();
    float J = 0.0;
    for (unsigned int j = 0; j < this->cantidad_rbf; j++) {
        std::vector<float> mu_j = this->neuronasRBF[j].getMu();
        for (unsigned int w = 0; w < cantidad_casos; w++) {
            std::vector<float> resta;
            utils::vectorResta(entradas[w], mu_j, resta);
            float norma = utils::vectorNorma(resta);
            J += (norma*norma);
        }
    }
}
*/

void RedRBF::kmeans(std::vector<std::vector<float> > entradas) {
    
    //guardo los centroides viejos para comparar y salir del while true
    std::vector<std::vector<float> > centroides_viejos;
    for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
        //obtengo el mu actual
        std::vector<float> mu_i = this->neuronasRBF[i].getMu();
        //lo pushbackeo
        centroides_viejos.push_back(mu_i);
    }

    unsigned int iteraciones = 0;

    while (true) { //hasta que el delta_mu sea menor que un EPS
  
        //vector de conjuntos de puntos
        //En cada conjuntos[i] se almacena la posicion dentro de 
        //  entradas de cada uno de los puntos que pertenecen al conjunto i
        std::vector<std::vector<std::vector<float> > > conjuntos;
        conjuntos.resize(this->cantidad_rbf);
        
        std::vector<std::vector<float> > inputs = entradas;
        unsigned int cantidad_entradas = inputs.size();
        for (unsigned int w = 0; w < cantidad_entradas; w++) { //para cada patron
            std::vector<float> distancias; //aqui se guardaran las distancias de este caso a todas las neuronas
            
            //mido distancias a cada neuronaRBF
            for (unsigned int k = 0; k < this->cantidad_rbf; k++) { //recorro las neuronas RBF
                
                //Le pido el centro actual a la neurona k
                std::vector<float> V = neuronasRBF[k].getMu();
                
                //calculo la distancia entre la neurona y el patron w
                float dist = utils::vectorDistancia(V, inputs[w]);
                
                distancias.push_back(dist);
            }
            
            //obtengo donde ocurrio la menor de las distancias 
            unsigned int indice_menor = utils::getMinIdx(distancias); //este patron tiene esta clase
            std::cout<<indice_menor<<' ';
            //Agrego el patron a la clase indice_menor
            conjuntos[indice_menor].push_back(inputs[w]);
        }
        
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
            float dist_i = utils::vectorDistancia(mu_i, centroides_viejos[i]);
            distancias_ceros.push_back(dist_i);
        }

        //Sumo todas las distancias obtenidas
        float suma_distancias = 0.0;
        for (unsigned int i = 0; i < distancias_ceros.size(); i++) {
            suma_distancias += distancias_ceros[i];
        }
        suma_distancias /= (float) distancias_ceros.size();

        //Si la suma es muy chica, quiere decir que se movieron poco, salgo del while true
        if (suma_distancias < EPS) 
            break;
        
        //actualizo los centroides viejos
        for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
            std::vector<float> mu_i = this->neuronasRBF[i].getMu();
            centroides_viejos[i] = mu_i;
        }
        iteraciones++;
    }
    std::cout<<"Termino el K-Means luego de "<<iteraciones<<" iteraciones.\n";
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
