/*
 * =====================================================================================
 *
 *       Filename:  agenteViajero.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/10/12 11:11:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <utility>
#include <cmath>
#include <algorithm>

unsigned int cantidad_ciudades;
//Guardo el recorrido realizado
std::vector<int> recorrido;
//Matriz de distancias entre las ciudades
std::vector<std::vector<float> > distancias;
//Vector de coordenadas de las ciudades
std::vector<std::pair<int, int> > coordenadas;

//Calcula la distancia entre pares
float dist(std::pair<int, int> p1, std::pair<int, int> p2) {
    return sqrt(pow(p2.first - p1.first,2) + pow(p2.second - p1.second,2));
}

//Lee las ciudades
void leer(std::string filename) {
    std::ifstream file (filename.c_str());
    assert(file.is_open());
    //Leo la cantidad de ciudades
    file>>cantidad_ciudades;
    for (unsigned int i = 0; i < cantidad_ciudades; i++) {
        std::pair<int, int> par;
        int x, y;
        //leo las coord y las pongo en el par
        file>>par.first>>par.second;
        //Las agrego al vector
        coordenadas.push_back(par);
    }
    

    //Calculo la matriz de distancias
    //Redimensiono
    distancias.resize(cantidad_ciudades);
    for (unsigned int i = 0; i < cantidad_ciudades; i++)
        distancias[i].resize(cantidad_ciudades);

    //Calculo la distancia de todos contra todos y la guardo en distancias
    for (unsigned int i = 0; i < cantidad_ciudades; i++) {
        for (unsigned int j = 0; j < cantidad_ciudades; j++) {
            float distancia = dist(coordenadas[i], coordenadas[j]);
            distancias[i][j] = distancia;
        }
    }
}

//Devuelve true si el recorrido es valido
bool solucionValida() {
    //Comprobaremos que es una solucion valida
    //Una solucion valida es aquella que:
    // * Empieza y termina en el mismo lugar
    // * Recorre todas las ciudades
    // * No tiene ciudades repetidas
    bool valido = false;
   
    //Comprobacion basica: si empezamos y terminamos en el mismo lugar
    if (recorrido.front() == recorrido.back()) {

        //Comprobamos que se recorra solo una vez cada ciudad (excepto la inicial y final, por lo que restamos 1 al recorrido.size)
        if (recorrido.size()-1 == cantidad_ciudades) {
            //Aqui ya sabemos que hay 10 ciudades recorridas, ahora averiguaremos si son todas distintas
            //Para ello, haremos una copia del recorrido y le quitaremos los duplicados, para ver si su tamaño coincide con la cantidad de ciudades

            //Copio el contenido
            std::vector<int> sin_repetir;
            sin_repetir = recorrido;
            
            //Quitamos los repetidos
            //Primero Ordenamos
            std::sort(sin_repetir.begin(), sin_repetir.end());
            //y luego removemos los repetidos
            std::vector<int>::iterator nuevo_fin = std::unique(sin_repetir.begin(), sin_repetir.end());
            //Borramos los elementos que sobran
            sin_repetir.erase(nuevo_fin, sin_repetir.end());
            
            //Compruebo que se recorran todas las ciudades
            if (sin_repetir.size() == cantidad_ciudades) {
                //Aqui podemos concluir que el recorrido es valido                
                valido = true;
            }
        }
    }
    return valido;
}



//Calcula la suma recorrida por el algoritmo
float calcularRecorrido() {
    float camino = 0;
    for (unsigned int i = 1; i < recorrido.size(); i++) {
        unsigned int desde = recorrido[i-1];
        unsigned int hasta = recorrido[i];
        camino += distancias[desde][hasta];
    }
    return camino;
}


int main() {
    //Leo las ciudades
    leer("ciudades.txt");
    recorrido.push_back(0);
    recorrido.push_back(1);
    recorrido.push_back(2);
    recorrido.push_back(6);
    recorrido.push_back(8);
    recorrido.push_back(4);
    recorrido.push_back(5);
    recorrido.push_back(7);
    recorrido.push_back(3);
    recorrido.push_back(9);
    recorrido.push_back(0);
    if(solucionValida())
        std::cout<<calcularRecorrido()<<'\n';
    else
        std::cout<<-1<<'\n';

}
