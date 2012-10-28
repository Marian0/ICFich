#include "include/particula.h"
#include "utils.h"

Particula::Particula(std::vector<float> limites_inferior, std::vector<float> limites_superior) {
    //Hacemos una verificacion de las dimensiones de los vectores
    this->dimension = limites_inferior.size();
    assert(this->dimension != limite_superior.size());

    //Instanciamos la partícula como un aleatorio entre las dimensiones especificadas
    for (unsigned int i = 0 ; i < this->dimension; i++) {
        float x = utils::randomDecimal(limites_inferior[i] , limites_superior[i] );
        this->posicion.push_back(x);
    }
    std::vector<float> velocidad_inicial(dimension, 0 );

    this->mejor_posicion_personal = this->posicion;
    this->velocidad = velocidad_inicial;
    this->limite_inferior = limites_inferior;
    this->limite_superior = limites_superior;

}


void Particula::actualizarPosicion(
                          std::vector<float> mejor_posicion_entorno,
                          float c1, float c2 ) {

    std::vector<float> error_personal;
    utils::vectorResta(this->mejor_posicion_personal, this->posicion, error_personal);

    std::vector<float> error_entorno;
    utils::vectorResta(mejor_posicion_entorno, this->posicion, error_entorno);


    for (unsigned int i = 0 ; i < this->dimension; i++ ) {
        float r1 = utils::randomDecimal(0,1);
        float r2 = utils::randomDecimal(0,1);
        //Calculos auxiliares para llegar a la nueva expresion de velocidad
        error_personal[i] *= c1*r1;
        error_entorno [i]  *= c2*r2;
    }

    //Hacemos v(t+1) = v(t) + c1 r1 * ...
    std::vector<float> nueva_velocidad;

    utils::vectorSuma(this->velocidad, error_personal, nueva_velocidad);
    utils::vectorSuma(nueva_velocidad, error_entorno, nueva_velocidad);
    utils::vectorSuma(nueva_velocidad, this->posicion, this->posicion);

    //Truncamos en las dimensiones que se pasen de los límites definidos.
    for (unsigned int i = 0; i < dimension; i++) {
        if (this->posicion[i] > this->limite_superior[i])
            this->posicion[i] = this->limite_superior[i];
        else if (this->posicion[i] < this->limite_inferior[i])
            this->posicion[i] = this->limite_inferior[i];
    }

}

void Particula::setMejorPosicionPersonal(std::vector<float> mejor_posicion_personal) {
    this->mejor_posicion_personal = mejor_posicion_personal;
}

std::vector<float> Particula::getMejorPosicionPersonal() {
    return this->mejor_posicion_personal;
}

std::vector<float> Particula::getPosicion() {
    return this->posicion;
}

float Particula::getMejorFitness() {
    return this->mejor_fitness;
}

void Particula::setMejorFitness(float mejor_fitness){
    this->mejor_fitness = mejor_fitness;
}
