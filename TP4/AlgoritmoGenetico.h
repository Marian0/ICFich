#ifndef __ALGORITMOGENETICO_H__
#define __ALGORITMOGENETICO_H__

#include <vector>
#include "Gen.h"

class AlgoritmoGenetico {
    std::vector<Gen> genes;
    public:
    AlgoritmoGenetico();
    void seleccion(std::vector<Gen> &nuevos_padres);
    void ruleta(std::vector<Gen> &nuevos_padres);
    void ventanas(std::vector<Gen> &nuevos_padres);
    void competencia(std::vector<Gen> &nuevos_padres);
    void cruza(Gen padre, Gen madre, std::vector<Gen> &hijos);
    void mutacion(Gen padre, &Gen hijo);

};
#endif
