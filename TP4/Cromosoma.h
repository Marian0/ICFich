#ifndef __CROMOSOMA_H__
#define __CROMOSOMA_H__
#include <vector>
#include "AlgoritmoGenetico.h"
class Cromosoma {
    //Vector con los valores de la cadena de bits
    std::vector<bool> genotipo;
    
    //Ultimo valor obtenido de la función de fitness
    float fitness;
    
    
    public:
    friend class AlgoritmoGenetico;
    Cromosoma(unsigned int cantidad_genes);
    //Calcula el fitness actual y lo guarda en la propiedad fitness
    float calcularFitness();

    //Devuelve el fitness actual
    float getFitness();

    //
};
#endif
