#ifndef __NEURONARBF_H__
#define __NEURONARBF_H__

#include <vector>
class NeuronaRBF {
    private:
        std::vector<float> media;
        float varianza;
        unsigned int dimension;
    public:
        NeuronaRBF(unsigned int dim);
        float getResponse(std::vector<float> X);
        void setMedia(std::vector<float> new_media);
        void setVarianza(float new_var);
        float distancia(std::vector<float> X);
        float getVarianza();
        std::vector<float> getMedia();
};

#endif
