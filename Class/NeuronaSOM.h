#ifndef __NEURONASOM_H__
#define __NEURONASOM_H__
#include <vector>
class NeuronaSOM {
    private:
    //Dimension de la neurona
    unsigned int dim;

    //Vector de pesos
    std::vector<float> W;
    
    public:
    
    //Constructor
    NeuronaSOM(unsigned int dim, float min, float max);

    //Calcula la distancia entre esta neurona y el patron X
    float getDistancia(std::vector<float> X);

};

#endif
