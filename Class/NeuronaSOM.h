#ifndef __NEURONASOM_H__
#define __NEURONASOM_H__
#include <vector>
class NeuronaSOM {
    private:
    //Dimension de la neurona
    unsigned int dim;

    //Vector de pesos
    std::vector<float> W;

    std::vector<unsigned int> contador_clases;
    int clase; 
    
    public:
  
    //valor demostrativo que indica que no tiene clase
    static const int NO_CLASS = -100;
    
    //Constructor
    NeuronaSOM(unsigned int dim, unsigned int cantidad_clases, float min, float max);

    //Calcula la distancia entre esta neurona y el patron X
    float getDistancia(std::vector<float> & X);

    //Devuelve los pesos
    std::vector<float> getW();

    //Setea los pesos
    void setW(std::vector<float> newW);

    //suma los contadores
    void sumarContadorClases(std::vector<float> Yd);

    //Define su clase en base a los contadores
    void definirClase();

    bool compararClase(std::vector<float> Yd);

    int getClase();
};

#endif
