#ifndef CLASE_H
#define CLASE_H
#include <string>

//Modela una clase de una materia, por ejemplo Matematica I (Teoría)
class Clase {
    public:
    std::string nombre;
    unsigned int id_materia;
    unsigned int cantidad_horas;
    unsigned int anio;

    Clase(std::string nom, unsigned int id_mat, unsigned int cant_horas, unsigned int anio);
    bool operator==(Clase & c);
    bool operator!=(Clase & c);

};

#endif // CLASE_H
