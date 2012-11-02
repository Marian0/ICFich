    #include <vector>
#include "Individuo.h"
#include "utils.h"

//Constructor vacio
Individuo::Individuo() { }

//Constructor
Individuo::Individuo(unsigned int cantidad_genes, unsigned int funcion_fitness_id, std::vector<Clase> clases, float escala, unsigned int variables_fenotipo) {
    this->funcion_fitness_id = funcion_fitness_id;
    this->escala = escala;
    this->variables_fenotipo = variables_fenotipo;
    this->clases = clases;

    //Relleno el individuo con contenido al azar
    for (unsigned int i = 0; i < cantidad_genes; i++) {
        //Genero un numero aleatorio
        bool gen = rand() % 2;

        //Asigno
        this->genotipo.push_back(gen);
    }
    //Calcula las matrices
    this->calcularMatrices();
    //Calcula el fitness
    this->calcularFitness();
}

//Devuelve el fitness actual
float Individuo::getFitness(){
    return this->fitness;
}

//Calcula el fitness de este individuo
float Individuo::calcularFitness() {
    float nuevo_fitness = 0.0;


    this->fitness = nuevo_fitness;
    return nuevo_fitness;
}

float Individuo::getFenotipo() {
    int valor = utils::binary2int(this->genotipo, true);
    return valor/escala;
}

//Calcula las matrices de horarios
void Individuo::calcularMatrices() {
    this->calcularMatrizBool();
    this->calcularMatrizInt();
}

//Calcula la matriz bool
void Individuo::calcularMatrizBool() {
    this->matriz_bool.clear();
    unsigned int cantidad_anios = 5;
    unsigned int cantidad_dias = 5;
    unsigned int cantidad_bloques_por_dia = 5;
    //rezizeamos la matriz a 5 años, pero como se va a utilizar años en base 1, se agrega un año de más
    this->matriz_bool.resize(cantidad_anios+1);
    for (unsigned int i = 0; i < cantidad_anios+1; i++) {
        //Resizeamos cada año a a 5 dias
        this->matriz_bool[i].resize(cantidad_dias);
        //Resizeamos cada dia a 5 bloques
        for (unsigned int j = 0; j < cantidad_dias; j++) {
            this->matriz_bool[i][j].resize(cantidad_bloques_por_dia, false);
        }
    }

    unsigned int cantidad_clases = this->variables_fenotipo;
    //calcula cuantos bits tiene cada clase
    unsigned int paso = this->genotipo.size() / cantidad_clases;

    for (unsigned int i = 0; i < cantidad_clases; i++) {
        std::vector<bool> value;

        unsigned int anio_clase = this->clases[i].anio;

        //asigna al vector los bits de una clase
        value.assign(this->genotipo.begin()+paso*i, this->genotipo.begin()+paso*(i+1));

        //Convierte el vector a int
        unsigned int bloque = utils::binary2int(value, false);

        //Convierte el bloque a coordenadas dentro de la matriz
        std::pair<unsigned int, unsigned int> pos = utils::posicionMatriz(cantidad_dias, cantidad_bloques_por_dia, bloque);

        //Ocupa el bloque
        this->matriz_bool[anio_clase][pos.first][pos.second] = true;
    }
}

//Calcula la matriz int
void Individuo::calcularMatrizInt() {
    this->matriz_int.clear();
    unsigned int cantidad_anios = 5;
    unsigned int cantidad_dias = 5;
    unsigned int cantidad_bloques_por_dia = 5;
    //rezizeamos la matriz a 5 años, pero como se va a utilizar años en base 1, se agrega un año de más
    this->matriz_int.resize(cantidad_anios+1);
    for (unsigned int i = 0; i < cantidad_anios+1; i++) {
        //Resizeamos cada año a a 5 dias
        this->matriz_int[i].resize(cantidad_dias);
        //Resizeamos cada dia a 5 bloques
        for (unsigned int j = 0; j < cantidad_dias; j++) {
            this->matriz_int[i][j].resize(cantidad_bloques_por_dia, false);
        }
    }

    unsigned int cantidad_clases = this->variables_fenotipo;
    //calcula cuantos bits tiene cada clase
    unsigned int paso = this->genotipo.size() / cantidad_clases;

    for (unsigned int i = 0; i < cantidad_clases; i++) {
        std::vector<bool> value;

        unsigned int anio_clase = this->clases[i].anio;

        //asigna al vector los bits de una clase
        value.assign(this->genotipo.begin()+paso*i, this->genotipo.begin()+paso*(i+1));

        //Convierte el vector a int
        unsigned int bloque = utils::binary2int(value, false);

        //Convierte el bloque a coordenadas dentro de la matriz
        std::pair<unsigned int, unsigned int> pos = utils::posicionMatriz(cantidad_dias, cantidad_bloques_por_dia, bloque);

        //Ocupa el bloque
        this->matriz_int[anio_clase][pos.first][pos.second] = i;
    }
}
