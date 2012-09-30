
#ifndef  __NEURONAPERCEPTRON_H__
#define  __NEURONAPERCEPTRON_H__
#include <vector>

/*
 * =====================================================================================
 *        Class:  NeuronaPerceptron
 *  Description:  
 * =====================================================================================
 */
class NeuronaPerceptron {
    private:
        std::vector<float> W;
        unsigned int dimension;
        float eta;
    public:
        NeuronaPerceptron (unsigned int dim, float eta);                             /* constructor */
        float getResponse(std::vector<float> X);
        float update(std::vector<float> X, float Yd);

}; 

#endif     
