#include "gradiente.h"
#include "utils.h"
#include <vector>
#include <cmath>
Gradiente::Gradiente(float tasa, std::vector<float> x_ini, unsigned int id_ejercicio, float criterio_corte, unsigned int maxit) {
    this->tasa_aprendizaje = tasa;
    this->x = x_ini;
    this->id_ejercicio = id_ejercicio;
    this->criterio_corte = criterio_corte;
    this->iteraciones_maximas = maxit;
}

unsigned int Gradiente::descender() {
    unsigned int iteraciones = 0;
    while(true) {
        std::vector<float> x_nuevo;
        std::vector<float> gradiente = this->evaluarGradiente(this->x);
        std::vector<float> cambio;
        utils::vectorEscalar(gradiente, this->tasa_aprendizaje, cambio);
        utils::vectorResta(this->x, cambio, x_nuevo);

        std::cout<<"Solucion del gradiente a iteracion "<<iteraciones<<" = ";
        utils::printVector(x_nuevo);

        float dist = utils::vectorDistancia(this->x, x_nuevo);
        if(dist < this->criterio_corte) {
            std::cout<<"Se sale del gradiente descediente porque el cambio fue menor a "<<this->criterio_corte<<'\n';
            break;
        }

        this->x = x_nuevo;

        iteraciones++;
        if (iteraciones > this->iteraciones_maximas) {
            std::cout<<"Se sale del gradiente descendiente porque se alcanzo el limite de iteraciones máximas = "<<this->iteraciones_maximas<<'\n';
            break;
        }
    }
    return iteraciones;
}

std::vector<float> Gradiente::getSolucion(){
    return this->x;
}

std::vector<float> Gradiente::evaluarGradiente(std::vector<float> &X) {
    std::vector<float> ret_val;
    switch(this->id_ejercicio){
    case 1: { //ejercicio 1a
        //una sola variable
        float x = X[0];

        float derivada = -sin(sqrt(abs(x)))-(pow(x,2)*cos(sqrt(abs(x))))/(2*pow(abs(x),(3/2)));
        ret_val.push_back(derivada);
        break;
    }
    case 2: { //ejercicio 1b
        //una sola variable
        float x = X[0];
        float derivada = -40*sin(5*x)+15*cos(3*x)+1;
        ret_val.push_back(derivada);
        break;
    }
    case 3: { //ejercicio 1c
        //dos variables
        float x = X[0];
        float y = X[1];
        //float derivadaX = (0.5*x*(sin(50*(y^2+x^2)^0.1)+1)^2)/(y^2+x^2)^0.75+(20.0*x*cos(50*(y^2+x^2)^0.1)*(sin(50*(y^2+x^2)^0.1)+1))/(y^2+x^2)^0.65;
        //float derivadaY = (0.5*y*(sin(50*(y^2+x^2)^0.1)+1)^2)/(y^2+x^2)^0.75+(20.0*y*cos(50*(y^2+x^2)^0.1)*(sin(50*(y^2+x^2)^0.1)+1))/(y^2+x^2)^0.65;

        float derivadaX = (0.5*x*pow(sin(50*pow(pow(y,2)+pow(x,2),0.1)+1),2)) / pow(pow(y,2)+pow(x,2),0.75) + (20.0*x*cos(50*pow(pow(y,2)+pow(x,2),0.1))) * (sin(50*pow(pow(y,2)+pow(x,2),0.1)+1)) / pow(pow(y,2)+pow(x,2),0.65);
        float derivadaY = (0.5*y*pow(sin(50*pow(pow(y,2)+pow(x,2),0.1)+1),2)) / pow(pow(y,2)+pow(x,2),0.75) + (20.0*y*cos(50*pow(pow(y,2)+pow(x,2),0.1))) * (sin(50*pow(pow(y,2)+pow(x,2),0.1)+1)) / pow(pow(y,2)+pow(x,2),0.65);
        ret_val.push_back(derivadaX);
        ret_val.push_back(derivadaY);
        break;
    }
    }
    return ret_val;
}
