//Includes de C
#include <vector>
#include <cassert>

//Includes de otras clases
#include "NeuronaSOM.h"
#include "utils.h"

//Constructor
NeuronaSOM::NeuronaSOM(unsigned int dim, unsigned int cantidad_clases, float min, float max) {
    this->dim = dim;

    //Iniciamos aleatoriamente los pesos
    for (unsigned int i = 0; i < this->dim; i++) {
        this->W.push_back(utils::randomDecimal(min, max));
    }

    //Agrego ceros para contar las clases
    for (unsigned int i = 0; i < cantidad_clases; i++)
        this->contador_clases.push_back(0);

    this->clase = NeuronaSOM::NO_CLASS;
}

//Calcula la distancia entre esta neurona y el patron X
float NeuronaSOM::getDistancia(std::vector<float> & X) {
    //Compruebo que tengan la misma dimension
    assert(X.size() == this->W.size());

    //Calcula la distancia
    return utils::vectorDistancia(this->W, X);
}

std::vector<float> NeuronaSOM::getW() {
    return this->W;
}

void NeuronaSOM::setW(std::vector<float> newW) {
    this->W = newW;
}


//suma los contadores
void NeuronaSOM::sumarContadorClases(std::vector<float> Yd) {
    //compruebo que tenga un valor al menos
    if (Yd.size() == 0) return;

    unsigned int new_class;
    
    if (Yd.size() > 1) //si es un vector, lo convierto a uint
        new_class = utils::binary2int(Yd);
    else //si no es vector, es un solo valor
        new_class = (unsigned int) Yd[0];

    //compruebo que sea una clase conocida
    assert(new_class < this->contador_clases.size());

    this->contador_clases[new_class]++;
    
}

//Define su clase en base a los contadores
void NeuronaSOM::definirClase() {
    unsigned int pos_max = 0;
    unsigned int maximo = this->contador_clases[pos_max];
    //Obtengo la posicion donde aparece el mayor contador
    for (unsigned int i = 1; i < this->contador_clases.size(); i++){
        if (this->contador_clases[i] > maximo) {
            pos_max = i;
            maximo = this->contador_clases[i];
        }
    }

    //pruebo si realmente hubo un maximo y no es 0
    if (maximo > 0) 
        this->clase = pos_max;
}

//suma los contadores
bool NeuronaSOM::compararClase(std::vector<float> Yd) {
    //compruebo que tenga un valor al menos
    assert(Yd.size() > 0);

    unsigned int clase_prueba;
    
    if (Yd.size() > 1) //si es un vector, lo convierto a uint
        clase_prueba = utils::binary2int(Yd);
    else //si no es vector, es un solo valor
        clase_prueba = (unsigned int) Yd[0];
    
    //true si la clase esta bien
    return clase_prueba == this->clase;
}

int NeuronaSOM::getClase() {
    return this->clase;
}
