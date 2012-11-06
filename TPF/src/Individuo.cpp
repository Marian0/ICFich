#include <vector>
#include <map>
#include "Individuo.h"
#include "utils.h"


//Constructor vacio
Individuo::Individuo() { }

//Constructor
Individuo::Individuo(unsigned int cantidad_genes, unsigned int funcion_fitness_id, std::vector<Clase> clases,
                     unsigned int aulas_disponibles, float escala, unsigned int variables_fenotipo) {
    this->funcion_fitness_id = funcion_fitness_id;
    this->escala = escala;
    this->variables_fenotipo = variables_fenotipo;
    this->clases = clases;
    this->aulas_disponibles = aulas_disponibles;

    for (unsigned int i = 0; i < variables_fenotipo; i++) {
        unsigned int bloque = rand() % 25;
        std::vector<bool> bloque_binary = utils::int2binary(bloque, false);
        while (bloque_binary.size() < 5 ) {
            bloque_binary.insert(bloque_binary.begin(), 0);
        }

        this->genotipo.insert(this->genotipo.end(), bloque_binary.begin(), bloque_binary.end() );
    }

    //Calcula las matrices
    this->calcularMatrices();

    //Calcula el fitness
    this->calcularFitness();

}

//Devuelve el fitness actual
float Individuo::getFitness(){
    return this->fitness;
}

//Calcula el fitness de este individuo
float Individuo::calcularFitness() {
    float nuevo_fitness = 0.0;

    //Validamos que sea un individuo valido.
    //Contamos en basura la cantidad de "2x1"=dos materias del mismo año en el mismo bloque
    //Si basura=0, quiere decir que el individuo es valido y esta todo bien
    unsigned int basura = 0;

    //en el vector guardamos las materias que se dan en cada bloque
    std::vector<std::vector<unsigned int> > materias_por_bloque;
    unsigned int cantidad_bloques = (this->matriz_bool.size()-1) * this->matriz_bool[0].size();
    materias_por_bloque.resize(cantidad_bloques);
    unsigned int paso = floor(this->genotipo.size() / this->variables_fenotipo);

    for (unsigned int i = 0; i < this->variables_fenotipo; i++) {
        std::vector<bool> bloque;
        //Le asignamos el bloque que corresponde a esta variable del fenotipo
        bloque.insert(bloque.end(), this->genotipo.begin() + paso*i, this->genotipo.begin() + paso*(i+1));

        //Obtenemos el id del bloque al que apunta esta materia
        unsigned int bloque_id = (unsigned int) utils::binary2int(bloque, false);

        //i me dice el id de clase. Almaceno el año de la materia pidiendole al vector de clases
        unsigned int anio_materia = this->clases[i].anio;
        materias_por_bloque[bloque_id].push_back(anio_materia);
    }

    //Contamos la cantidad de veces que se repite una clase del mismo año en el mismo bloque de horario
    for (unsigned int i = 0; i < materias_por_bloque.size(); i++) {
        std::map<unsigned int, unsigned int> repetidos;
        for (unsigned int j = 0; j < materias_por_bloque[i].size(); j++) {
            if (repetidos.find(materias_por_bloque[i][j]) == repetidos.end()) {
                repetidos[materias_por_bloque[i][j]] = 1;
            } else {
                repetidos[materias_por_bloque[i][j]]++;
            }
        }

        std::map<unsigned int, unsigned int>::iterator p = repetidos.begin();
        std::map<unsigned int, unsigned int>::iterator q = repetidos.end();
        while (p != q) {          
            if (p->second > 1)
                basura++;
            p++;
        }
    }


    //------------
    //Contamos la cantidad de solapamientos entre años adyacentes
    unsigned int solapamientos_adyacentes = 0;

    for (unsigned int i = 0; i < this->matriz_bool.size() - 1; i++) {
        std::vector<std::vector<bool> > matriz_producto =
                utils::multiplicarMatrices(this->matriz_bool[i], this->matriz_bool[i+1]);
        for (unsigned int j = 0; j < matriz_producto.size(); j++) {
            for (unsigned int k = 0; k < matriz_producto[j].size(); k++)
                if(matriz_producto[j][k] == true)
                    solapamientos_adyacentes++;
        }
    }

    //-------------
    //Contamos la cantidad de veces que se sobrepasa el limite de aulas
    //Copiamos el vector del primer año
    std::vector<std::vector<unsigned int> > suma_aulas;
    suma_aulas.resize(this->matriz_bool[0].size());
    for (unsigned int i = 0; i < this->matriz_bool[0].size(); i++) {
        suma_aulas[i].resize(this->matriz_bool[0][i].size());
        for (unsigned int j = 0; j < this->matriz_bool[0][i].size(); j++) {
            suma_aulas[i][j] = this->matriz_bool[0][i][j];
        }
    }

    for (unsigned int i = 1; i < this->matriz_bool.size(); i++){
        suma_aulas = utils::sumarMatrices(suma_aulas, this->matriz_bool[i]);
    }

    unsigned int sobrepaso_aulas = 0;

    for (unsigned int i = 0; i < suma_aulas.size(); i++) {
        for (unsigned int j = 0; j < suma_aulas[i].size(); j++) {
            if (suma_aulas[i][j] > this->aulas_disponibles)
                sobrepaso_aulas++;
        }
    }

    //-------------
    //Contamos la cantidad de veces que se repite una materia en el mismo día
    unsigned int cantidad_repeticiones = 0;
    //Para cada año
    for (unsigned int i = 0; i < this->matriz_int.size(); i++) {
        //Para cada día
        for (unsigned int j = 0; j < this->matriz_int[i].size(); j++) {
            //Para cada materia
            //Map de id de materia a cantidad de ocurrencias
            std::map<unsigned int, unsigned int> contador;
            //Para cada slot de tiempo
            for (unsigned int k = 0; k < this->matriz_int[i][j].size(); k++) {
                int valor = this->matriz_int[i][j][k];
                if (valor == -1)
                    continue; //no hay materia en este bloque
                if(contador.find(valor) == contador.end()){//si no lo encontre
                    contador[valor] = 1; //inicializo el contador en 1
                } else {
                    contador[valor]++; //aumento el contador
                }
            }
            std::map<unsigned int, unsigned int>::iterator p = contador.begin();
            std::map<unsigned int, unsigned int>::iterator q = contador.end();

            while (p != q) {
                if (p->second > 1) //aparece la materia mas de una vez en el dia, no deseable
                    cantidad_repeticiones++;
                p++;
            }
        }
    }

    //std::cout<<cantidad_repeticiones<<' '<<sobrepaso_aulas<<' '<<solapamientos_adyacentes<<' '<<basura<<'\n';


    //Calculamos el denominador de la funcion de fitness
    float denominador = (cantidad_repeticiones+1) * (sobrepaso_aulas+1) *
            pow(solapamientos_adyacentes+1,2) * pow(basura+1, 5);


    //confiamos en que nunca el denominador va a ser cero
    nuevo_fitness = 1/denominador;

    this->fitness = nuevo_fitness;
    return nuevo_fitness;
}

float Individuo::getFenotipo() {
    int valor = utils::binary2int(this->genotipo, true);
    return valor/escala;
}

//Calcula las matrices de horarios
void Individuo::calcularMatrices() {
    this->calcularMatrizBool();
    this->calcularMatrizInt();
}

//Calcula la matriz bool
void Individuo::calcularMatrizBool() {
    this->matriz_bool.clear();
    unsigned int cantidad_anios = 5;
    unsigned int cantidad_dias = 5;
    unsigned int cantidad_bloques_por_dia = 5;
    //rezizeamos la matriz a 5 años, pero como se va a utilizar años en base 1, se agrega un año de más
    this->matriz_bool.resize(cantidad_anios+1);
    for (unsigned int i = 0; i < cantidad_anios+1; i++) {
        //Resizeamos cada año a a 5 dias
        this->matriz_bool[i].resize(cantidad_dias);
        //Resizeamos cada dia a 5 bloques
        for (unsigned int j = 0; j < cantidad_dias; j++) {
            this->matriz_bool[i][j].resize(cantidad_bloques_por_dia, false);
        }
    }

    unsigned int cantidad_clases = this->variables_fenotipo;
    //calcula cuantos bits tiene cada clase  
    unsigned int paso = this->genotipo.size() / cantidad_clases;

    for (unsigned int i = 0; i < cantidad_clases; i++) {
        std::vector<bool> value;

        unsigned int anio_clase = this->clases[i].anio;

        //asigna al vector los bits de una clase
        value.assign(this->genotipo.begin()+paso*i, this->genotipo.begin()+paso*(i+1));

        //Convierte el vector a int
        unsigned int bloque = utils::binary2int(value, false);

        //Convierte el bloque a coordenadas dentro de la matriz
        std::pair<unsigned int, unsigned int> pos = utils::posicionMatriz(cantidad_dias, cantidad_bloques_por_dia, bloque);

        //Ocupa el bloque
        this->matriz_bool[anio_clase][pos.first][pos.second] = true;
    }
}

//Calcula la matriz int
void Individuo::calcularMatrizInt() {
    this->matriz_int.clear();
    unsigned int cantidad_anios = 5;
    unsigned int cantidad_dias = 5;
    unsigned int cantidad_bloques_por_dia = 5;
    //rezizeamos la matriz a 5 años, pero como se va a utilizar años en base 1, se agrega un año de más
    this->matriz_int.resize(cantidad_anios+1);
    for (unsigned int i = 0; i < cantidad_anios+1; i++) {
        //Resizeamos cada año a a 5 dias
        this->matriz_int[i].resize(cantidad_dias);
        //Resizeamos cada dia a 5 bloques
        for (unsigned int j = 0; j < cantidad_dias; j++) {
            this->matriz_int[i][j].resize(cantidad_bloques_por_dia, -1);
        }
    }

    unsigned int cantidad_clases = this->variables_fenotipo;
    //calcula cuantos bits tiene cada clase
    unsigned int paso = this->genotipo.size() / cantidad_clases;

    for (unsigned int i = 0; i < cantidad_clases; i++) {
        std::vector<bool> value;

        unsigned int anio_clase = this->clases[i].anio;

        //asigna al vector los bits de una clase
        value.assign(this->genotipo.begin()+paso*i, this->genotipo.begin()+paso*(i+1));

        //Convierte el vector a int
        unsigned int bloque = utils::binary2int(value, false);

        //Convierte el bloque a coordenadas dentro de la matriz
        std::pair<unsigned int, unsigned int> pos = utils::posicionMatriz(cantidad_dias, cantidad_bloques_por_dia, bloque);

        //Ocupa el bloque
        this->matriz_int[anio_clase][pos.first][pos.second] = i;
    }
}
