#include "Clase.h"
#include <string>
#include <iostream>
Clase::Clase(std::string nom, unsigned int id_mat, unsigned int cant_horas, unsigned int anio) {
    this->nombre = nom;
    this->id_materia = id_mat;
    this->cantidad_horas = cant_horas;
    this->anio = anio;
}

//Compara que esta clase y la del parametro pertenecen a la misma materia
// (mismo id_materia pero distintas horas y nombre)
bool Clase::operator==(Clase & c) {
    return this->id_materia == c.id_materia;
}

//Compara que esta clase y la del parametro pertenecen a distintas materias
// (mismo id_materia pero distintas horas y nombre)
bool Clase::operator!=(Clase & c) {
    return this->id_materia != c.id_materia;
}
