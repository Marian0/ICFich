#include "AgenteViajero.h"
#include "utils.h"
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <utility>
#include <cassert>
#include <iostream>

//constructor vacio
AgenteViajero::AgenteViajero(){ }

//Lee las ciudades del problema del agente viajero
void AgenteViajero::leer(std::string filename) {
    std::ifstream file (filename.c_str());
    assert(file.is_open());
    //Leo la cantidad de ciudades
    file>>this->cantidad_ciudades;
    for (unsigned int i = 0; i < this->cantidad_ciudades; i++) {
        std::pair<int, int> par;
        //leo las coord y las pongo en el par
        file>>par.first>>par.second;
        //Las agrego al vector
        this->coordenadas.push_back(par);
    }
    file.close();

    //Calculo la matriz de distancias
    //Redimensiono
    this->distancias.resize(this->cantidad_ciudades);
    for (unsigned int i = 0; i < this->cantidad_ciudades; i++)
        this->distancias[i].resize(this->cantidad_ciudades);

    //Calculo la distancia de todos contra todos y la guardo en distancias
    for (unsigned int i = 0; i < this->cantidad_ciudades; i++) {
        for (unsigned int j = 0; j < this->cantidad_ciudades; j++) {
            float distancia = this->dist(this->coordenadas[i], this->coordenadas[j]);
            this->distancias[i][j] = distancia;
        }
    }
}


//Devuelve 1 si el recorrido es valido
//De lo contrario, devuelve sin_repetir/total, que sera 1 si no hay repetidos y casi 0 si esta la misma ciudad en todas
float AgenteViajero::solucionValida(std::vector<int> &recorrido) {
    //Comprobaremos que es una solucion valida
    //Una solucion valida es aquella que:
    // * Todas las ciudades son validas (ninguna es mayor o igual a cantidad_ciudades
    // * Recorre todas las ciudades
    // * No tiene ciudades repetidas


//Comentado porque se han sobrecargado los operadores de variacion poblacional para que no generen individuos invalidos
//    unsigned int ciudades_invalidas = 0;
//    for (unsigned int i = 0; i < recorrido.size(); i++) {
//        if (recorrido[i] >= this->cantidad_ciudades)
//            ciudades_invalidas++;
//    }

//    if (ciudades_invalidas > 0) return 0.0;



    //Comprobacion basica: si empezamos y terminamos en el mismo lugar
    //Nota: no se tiene en cuenta que el nodo final sea igual al inicial en el cromosoma, pero al calcular la longitud
    //del recorrido, esa distancia se suma al total.


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


    unsigned int cantidad_ciudades_repetidas = this->cantidad_ciudades - sin_repetir.size();

    return 1 / pow(cantidad_ciudades_repetidas + 1,2);


//    float ret_val;
//    //Compruebo que se recorran todas las ciudades
//    if (sin_repetir.size() == this->cantidad_ciudades) {
//        //Aqui podemos concluir que el recorrido es valido
//        ret_val = 1.0;
//    }
//    else { //Si hay repetidos o ciudades que no existen en el mapa, devolvemos un porcentaje de error
//        ret_val = float(sin_repetir.size()) / float(this->cantidad_ciudades);
//    }

//    return ret_val;


//    return 1.0/float(this->cantidad_ciudades);
}

//Calcula la distancia entre pares
float AgenteViajero::dist(std::pair<int, int> p1, std::pair<int, int> p2) {
    return sqrt(pow(p2.first - p1.first,2) + pow(p2.second - p1.second,2));
}

//Calcula la suma recorrida por el algoritmo para el agente viajero
float AgenteViajero::calcularRecorrido(std::vector<int> &recorrido) {
    float camino = 0.0;
    for (unsigned int i = 1; i < recorrido.size(); i++) {
        unsigned int desde = recorrido[i-1];
        unsigned int hasta = recorrido[i];
        //no se calculan distancias para ciudades invalidas
        if (desde < this->cantidad_ciudades and hasta < this->cantidad_ciudades) {
            camino += this->distancias[desde][hasta];
        }
    }
    if(recorrido.front() < this->cantidad_ciudades and recorrido.back() < this->cantidad_ciudades)
        camino += this->distancias[recorrido.front() ][recorrido.back()];
    return camino;
}
