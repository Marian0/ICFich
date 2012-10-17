#ifndef __INDIVIDUO_H__
#define __INDIVIDUO_H__

#include <vector>


class Individuo {
    public:
    //Vector con los valores de la cadena de bits
    std::vector<bool> genotipo;
    
    //Ultimo valor obtenido de la función de fitness
    float fitness; 

    //Constructor
    Individuo(unsigned int cantidad_genes);

    //Calcula el fitness actual y lo guarda en la propiedad fitness
    virtual float calcularFitness() = 0; //la hace clase abstracta

    //Devuelve el fitness actual
    float getFitness();

};
#endif
