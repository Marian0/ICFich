#ifndef __REDRBF_H__
#define __REDRBF_H__
#include <vector>
#include "NeuronaRBF.h"
#include "NeuronaPerceptron.h"
class RedRBF {
    private:
        std::vector<NeuronaRBF> neuronasRBF;
        std::vector<NeuronaPerceptron> neuronasPerceptron;
        unsigned int dimension;
        unsigned int cantidad_rbf;
        unsigned int cantidad_perceptron;
    public:
        RedRBF(unsigned int dimension, unsigned int cantidad_rbf, unsigned int cantidad_perceptron, float eta);

        float test(std::vector<std::vector<float> > X, std::vector<std::vector<float> > Yd);
        float train (std::vector<std::vector<float> > X, std::vector<std::vector<float> > Yd);
        void kmeans (std::vector<std::vector<float> > X, int maxit);
        
        std::vector<float> getVarianzas();
        std::vector<std::vector<float> > getMedias();
};
#endif
