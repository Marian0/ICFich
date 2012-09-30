/*
 * =====================================================================================
 *
 *       Filename:  RedRBF.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  30/09/12 00:14:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "RedRBF.h"
#include <vector>
#include <cstdlib>
#include "utils.h"
#include <cstdio>
RedRBF::RedRBF(unsigned int dimension, unsigned int cantidad_rbf, unsigned int cantidad_perceptron, float eta) {
    this->dimension = dimension;
    this->cantidad_rbf = cantidad_rbf;
    this->cantidad_perceptron = cantidad_perceptron;
    for (unsigned int i = 0; i < cantidad_rbf; i++) {
        NeuronaRBF neu(this->dimension);
        this->neuronasRBF.push_back(neu);
    }

    for (unsigned int i = 0; i < cantidad_perceptron; i++) {
        NeuronaPerceptron neu(this->cantidad_rbf+1, eta);
        this->neuronasPerceptron.push_back(neu);
    }
}

float RedRBF::train (std::vector<std::vector<float> > X, std::vector<std::vector<float> > Yd) {
    //contador del error
    float errores = 0.0;

    for (unsigned int w = 0; w < X.size(); w++) {//para cada patron
        //Aqui se guardaran las respuestas de las RBF
        std::vector<float> respuestasRBF;
        for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
            //Capturo la respuesta de cada RBF al patron w
            respuestasRBF.push_back(this->neuronasRBF[i].getResponse(X[w]));
        }
        
        //Inserto una entrada mas, que corresponde al bias \phi_0
        respuestasRBF.insert(respuestasRBF.begin(), 1);
       
        //utils::printVector(respuestasRBF); std::getchar();

        //para cada neurona de la capa de salida
        for (unsigned int i = 0; i < this->cantidad_perceptron; i++) {
            
            //actualizo la neurona actual y me devuelve el error que obtuvo
            //Su entrada es la salida de las RBF (respuestasRBF)
            float error_i = this->neuronasPerceptron[i].update(respuestasRBF, Yd[w][i]);
            
            //sumo el error
            errores += error_i*error_i;
        }
        errores *= 0.5;
    }
    return errores/((float) X.size());
}
float RedRBF::test(std::vector<std::vector<float> > X, std::vector<std::vector<float> > Yd) {
    //contador del error
    unsigned int cantidad_aciertos = 0;

    for (unsigned int w = 0; w < X.size(); w++) {//para cada patron
        
        //Aqui se guardaran las respuestas de las RBF
        std::vector<float> respuestasRBF;
        for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
            //Capturo la respuesta de cada RBF al patron w
            respuestasRBF.push_back(this->neuronasRBF[i].getResponse(X[w]));
        }
        
        //Inserto una entrada mas, que corresponde al bias \phi_0
        respuestasRBF.insert(respuestasRBF.begin(), 1);
       
        //Aciertos guardara los aciertos de CADA neurona
        unsigned int aciertos = 0;
        //para cada neurona de la capa de salida
        for (unsigned int i = 0; i < this->cantidad_perceptron; i++) {
            
            //actualizo la neurona actual y me devuelve el error que obtuvo
            //Su entrada es la salida de las RBF (respuestasRBF)
            float resp = this->neuronasPerceptron[i].getResponse(respuestasRBF);
            
            //Si es acierto, sumo aciertos
            if (fabs(resp - Yd[w][i]) < 0.1) aciertos++;
        }
        //Comparo si tuve la misma cantidad de aciertos que neuronas => realmente dio bien
        if (aciertos == this->cantidad_perceptron) cantidad_aciertos++;
    }
    return cantidad_aciertos/((float) X.size());
}


void RedRBF::kmeans (std::vector<std::vector<float> > X, int maxit) {
    //seteo las medias con patrones aleatorios
    for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
        unsigned int ran_num = rand() % X.size();
        this->neuronasRBF[i].setMedia(X[ran_num]);
    }

    std::vector<std::vector<std::vector<float> > > subconjuntos;
    while (maxit--) {
        subconjuntos.clear();
        subconjuntos.resize(this->cantidad_rbf);
        
        //Para cada patron
        for (unsigned int w = 0; w < X.size(); w++) {
            //encuentro el centroide que tiene mas cerca
            float dist_min = neuronasRBF[0].distancia(X[w]);
            unsigned int mas_cercano = 0;
            
            for (unsigned int j = 1; j < this->cantidad_rbf; j++) {
                float dist = neuronasRBF[j].distancia(X[w]);
                if (dist < dist_min) {
                    dist_min = dist;
                    mas_cercano = j;
                }
            }
            //asigno ese patron al subconjunto mas cercano
            subconjuntos[mas_cercano].push_back(X[w]);
        }
        //recalculo de los mu
        
        //para cada centroide
        for (unsigned int j = 0; j < this->cantidad_rbf; j++) {
            unsigned int cantidad = subconjuntos[j].size();
            
            if (cantidad == 0) continue;
            
            std::vector<float> nuevaMedia;
            nuevaMedia.resize(this->dimension, 0.0);

            //Para cada patron asociado a este subconjunto
            for (unsigned int w = 0; w < cantidad; w++) {
                for (unsigned int k = 0; k < this->dimension; k++) {
                    nuevaMedia[k] += subconjuntos[j][w][k];
                }
            }
            
            for (unsigned int k = 0; k < this->dimension; k++) {
                nuevaMedia[k] /= (float) cantidad;
            }
            this->neuronasRBF[j].setMedia(nuevaMedia);
        }
    }

    //estimacion de la varianza
    //sigma_j^2 = \frac{1}{M_j} \sum (\norm x - mu_j \norm )^2

    for (unsigned int j = 0; j < this->cantidad_rbf; j++) {
        unsigned int cantidad = subconjuntos[j].size();
        float new_var = 0.0;
       //Para cada patron asociado a este subconjunto
        for (unsigned int w = 0; w < cantidad; w++) {
            float dist = this->neuronasRBF[j].distancia(subconjuntos[j][w]);
            new_var += dist;
        }
        new_var = new_var / ((float) cantidad);
        this->neuronasRBF[j].setVarianza(new_var);
    }
}


std::vector<std::vector<float> > RedRBF::getMedias() {
    std::vector<std::vector<float> > salida;
    for (unsigned int i = 0; i < this->cantidad_rbf; i++) {
        salida.push_back(this->neuronasRBF[i].getMedia()); 
    }
    return salida;
}

std::vector<float> RedRBF::getVarianzas() {
    std::vector<float> salida;
    for (unsigned int i = 0; i < this->cantidad_rbf; i++)
        salida.push_back(this->neuronasRBF[i].getVarianza());
    return salida;
}
