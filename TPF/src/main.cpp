#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include "utils.h"
#include "Config.h"
#include "GNUPlot.h"
#include "AlgoritmoGenetico.h"
#include "Clase.h"


//Variable global
Config config("configuracion.cfg"); //lectura de la configuracion

//Lee todas las clases y las guarda en un vector
std::vector<Clase> leerClases(std::string nombre_archivo) {
    //Leemos el archivo
    std::ifstream file (nombre_archivo.c_str());
    //Comprobamos que este abierto
    assert(file.is_open());
    //Valor a retornar, vector con todas las clases
    std::vector<Clase> ret_val;
    std::string line;
    //Hasta llegar al final
    //Lee una linea
    while (getline(file, line)) {
        std::stringstream iss;
        //Pasa la string a streamstring
        iss<<line;

        //Cometario, no se procesa
        if(!iss.str().empty() && iss.str()[0] == '#'){
            //std::cout<<iss.str()<<std::endl;
            continue;
        }

        //Separa la linea en 3
        std::string nom_mat;
        unsigned int id_mat;
        unsigned int cant_horas;
        unsigned int anio;


        //std::stringstream ss;
        for (unsigned int i = 0; i < 4; i++) {
            std::string s;
            //Lee un parametro
            getline(iss,s,',');

            //Segun que tipo de parametro sea...
            if (i == 0)
                nom_mat = s;
            if (i == 1)
                id_mat = utils::strToInt(s);
            if (i == 2)
                cant_horas = utils::strToInt(s);
            if (i == 3)
                anio = utils::strToInt(s);
        }
        //Construye una clase
        Clase clase (nom_mat, id_mat, cant_horas, anio);
        //La agrega al vector
        ret_val.push_back(clase);

    }

    file.close();
    return ret_val;
}

void escribirSolucion(std::vector<std::vector<std::vector<int> > >matriz_int, std::vector<Clase> clases, std::string archivo_salida) {
    std::ofstream file(archivo_salida.c_str());
    assert(file.is_open());
    //por cada aÃ±o
    for (unsigned int i = 1; i < matriz_int.size(); i++) {
        file<<"\nAnio "<<i<<"\n";
        //por cada dia
        for (unsigned int j = 0; j < matriz_int[i].size(); j++) {
            //por cada bloque
            for (unsigned int k = 0; k < matriz_int[i][j].size(); k++) {
                unsigned id_clase = matriz_int[i][j][k];
                std::string nombre_clase = clases[id_clase].nombre;
                file<<nombre_clase<<'\t';
            }
            file<<'\n';
        }
        file<<'\n';
    }
    file<<'\n';
    file.close();
}


int main() {
    //inicializacion de semilla
    srand( (unsigned) std::time(NULL));

    std::vector<Clase> clases = leerClases("clases.txt");
    getwchar();

    //Leemos los valores de configuracion
      float           probabilidad_cruza      = utils::strToFloat(config.getValue("cruza"));
      float           probabilidad_mutacion   = utils::strToFloat(config.getValue("mutacion"));
      unsigned int    tamanio_poblacion       = utils::strToInt(config.getValue("tamanio_poblacion"));
      unsigned int    variables_fenotipo      = utils::strToInt(config.getValue("variables_fenotipo"));
      unsigned int    cantidad_generaciones   = utils::strToInt(config.getValue("cantidad_generaciones"));
      unsigned int    cantidad_genes          = utils::strToInt(config.getValue("cantidad_genes"));
      float           escala                  = utils::strToFloat(config.getValue("escala"));
      unsigned int    elitismo                = utils::strToInt(config.getValue("elitismo"));
      unsigned int    id_funcion_fitness      = utils::strToInt(config.getValue("id_funcion_fitness"));
      std::string     forma_seleccion         = config.getValue("forma_seleccion");
      float           error                   = utils::strToFloat(config.getValue("error"));
      unsigned int    brecha_generacional     = utils::strToInt(config.getValue("brecha_generacional"));
      unsigned int    k_competencia           = utils::strToInt(config.getValue("k_competencia"));
      float           fitness_deseado         = utils::strToFloat(config.getValue("fitness_deseado"));
      unsigned int    bits_por_clase           = utils::strToInt(config.getValue("bits_por_clase"));


    std::cout<<"Bienvenidos al Trabajo Final - Inteligencia Computacional - 2012\n";
    std::cout<<"Problema de Organización de Materias en una Facultad\n";
    std::cout<<"Nellmeldin Fernando - Peyregne Mariano\n";





    getwchar();
    return 0;
}
