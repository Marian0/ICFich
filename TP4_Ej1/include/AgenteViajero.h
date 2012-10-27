#ifndef AGENTEVIAJERO_H
#define AGENTEVIAJERO_H
#include <vector>
#include <string>
#include <utility>

class AgenteViajero{
    //Cantidad de ciudades en el mapa
    unsigned int cantidad_ciudades;

    //Matriz de distancias entre las ciudades
    std::vector<std::vector<float> > distancias;

    //Vector de coordenadas de las ciudades
    std::vector<std::pair<int, int> > coordenadas;

public:
    //Constructor vacio
    AgenteViajero();

    //Lee el mapa del agente viajero y calcula las distancias entre todas las ciudades
    void leer(std::string filename);

    //Comprueba que una solucion es valida
    float solucionValida(std::vector<int> &recorrido);

    //Calcula la distancia entre dos pares
    float dist(std::pair<int, int> p1, std::pair<int, int> p2);

    //Calcula el recorrido
    float calcularRecorrido(std::vector<int> &recorrido);

};

#endif // AGENTEVIAJERO_H
