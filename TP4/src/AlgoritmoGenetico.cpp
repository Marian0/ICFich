#include <algorithm>
#include "utils.h"
#include "AlgoritmoGenetico.h"
#include "Individuo.h"
#include <cstdio>

AlgoritmoGenetico::AlgoritmoGenetico(   unsigned int tam_pob, unsigned int cant_genes,
                                        float escala, unsigned int variables_fenotipo,
                                        unsigned int max_gen, float pcruza, float pmutacion,
                                        unsigned int elitismo, unsigned int brecha_generacional,
                                        unsigned int id_funcion_fitness,
                                        unsigned int met_sel, unsigned int k_competencia) {

    //Copia las propiedades del algoritmo
    this->tamanio_poblacion = tam_pob;
    this->cantidad_genes = cant_genes;
    this->generaciones_maximo = max_gen;
    this->generacion_actual = 0;
    this->probabilidad_cruza = pcruza;
    this->probabilidad_mutacion = pmutacion;
    this->metodo_seleccion = met_sel;
    this->k_competencia = k_competencia;
    this->n_elitismo = elitismo;
    this->n_brecha_generacional = brecha_generacional;
    this->id_funcion_fitness = id_funcion_fitness;
    this->cantidad_mutaciones = 0;
    this->cantidad_cruzas = 0;
    this->escala = escala;
    this->variables_fenotipo = variables_fenotipo;

    //Crea todos los Individuos
    for (unsigned int i = 0; i < this->tamanio_poblacion; i++) {
        Individuo new_ind(this->cantidad_genes, this->id_funcion_fitness, this->escala, this->variables_fenotipo);
        this->poblacion.push_back(new_ind);
    }

    /*
    //Evaluo la poblacion
    this->evaluar();

    //Ordeno la población de mayor a menor fitness
    std::sort(this->poblacion.begin(), this->poblacion.end(), AlgoritmoGenetico::ordenarIndividuos);
    */

}

//Crea la nueva generacion
void AlgoritmoGenetico::reproduccion() {
    //Actualizacion de generacion
    this->generacion_actual++;

    if(this->generacion_actual > this->generaciones_maximo) {
        std::cout<<"Maximo de generaciones alcanzadas\n";
        return;
    }

    std::vector<Individuo> nueva_poblacion;

    //Ordeno la población de mayor a menor fitness
    std::sort(this->poblacion.begin(), this->poblacion.end(), AlgoritmoGenetico::ordenarIndividuos);

    //Seleccionamos Padres
    std::vector<Individuo> padres;
    this->seleccion(padres, this->tamanio_poblacion - this->n_elitismo - this->n_brecha_generacional);

    //Genero un arreglo de índices para luego mezlcar y ser eficiete
    std::vector<int> vector_id_poblacion;

    unsigned int npadres = padres.size();
    assert(npadres>1);

    for (unsigned int i = 0; i < npadres; i++ ) {
        vector_id_poblacion.push_back(i);
    }
    std::random_shuffle(vector_id_poblacion.begin(), vector_id_poblacion.end() );

    unsigned int n_iteraciones = floor( (npadres / 2 )) ;

    for (unsigned int i = 0; i < n_iteraciones; i++) {
        //Tomamos aleatorio los id de un padre y una madre de la poblacion
        unsigned int id_padre = rand() % npadres;
        unsigned int id_madre = rand() % npadres;

        std::vector<Individuo> hijos;
        //Realizamos la cruza (Se obtendrán 2 hijos)
        this->cruza(padres[vector_id_poblacion[id_padre]], padres[vector_id_poblacion[id_madre]], hijos );

        //Realizamos la mutación de los hijos
        this->mutacion(hijos[0]);
        this->mutacion(hijos[1]);

        //Agregamos los hijos a la nueva población
        nueva_poblacion.push_back(hijos[0]);
        nueva_poblacion.push_back(hijos[1]);
    }

    //Tomo los individuos elite de la población vieja
    for (unsigned int i = 0; i < this->n_elitismo && i < this->tamanio_poblacion; i++) {
        nueva_poblacion.push_back(this->poblacion[i]);
    }

    //Paso los padres sin modificar a la nueva generacion mediante brecha generacional
    for (unsigned int i = 0; i < this->n_brecha_generacional; i++) {
        unsigned int pos = rand() % this->tamanio_poblacion;
        nueva_poblacion.push_back(this->poblacion[pos]);
    }

    //Actualizo los valores de la nueva población
    this->poblacion = nueva_poblacion;
    this->tamanio_poblacion = nueva_poblacion.size();

}

//Evalua la poblacion, calculando los fitness, y devuelve el mejor
float AlgoritmoGenetico::evaluar() {
    //Variable que guarda el fitness mayor encontrado
    float fitness_max = this->poblacion[0].calcularFitness();
    unsigned int id_max_fit = 0;

    //Variable que guarda el fitness peor encontrado
    float fitness_min = this->poblacion[0].getFitness();
    unsigned int id_min_fit = 0;

    //Recorro la poblacion y la evaluo
    for (unsigned int i = 1; i < this->tamanio_poblacion; i++) {
        //Calculo el fitness
        float fitness_i = this->poblacion[i].calcularFitness();
        //Reemplazo si es mejor que el que tenia
        if (fitness_i > fitness_max) {
            fitness_max = fitness_i;
            id_max_fit = i;
        }
        //Reemplazo si es peor que el que tenia
        if (fitness_i < fitness_min) {
            fitness_min = fitness_i;
            id_min_fit = i;
        }
    }

    this->id_maximo_fitness = id_max_fit;
    this->id_minimo_fitness = id_min_fit;

    return fitness_max;
}


//Realiza la selección de la poblacion, y guarda en nuevos_padres los Individuos elegidos.
//Segun el metodo de seleccion definido, llama a Ruleta, Ventanas o Competencia
void AlgoritmoGenetico::seleccion(std::vector<Individuo> &nuevos_padres, unsigned int cantidad_a_generar) {
    nuevos_padres.clear();
    std::vector<Individuo> progenitores;
    switch(this->metodo_seleccion) {
    case SELECCION_RULETA: {
        ruleta(progenitores, cantidad_a_generar);
        break;
    }
    case SELECCION_VENTANAS: {
        ventanas(progenitores, cantidad_a_generar);
        break;
    }
    case SELECCION_COMPETENCIA: {
        competencia(progenitores, cantidad_a_generar);
        break;
    }
    }

    nuevos_padres.insert(nuevos_padres.end(), progenitores.begin(), progenitores.end());

}

//Metodo de la ruleta para la seleccion
void AlgoritmoGenetico::ruleta(std::vector<Individuo> &nuevos_padres, unsigned int cantidad_a_generar) {
    //Calculo la suma de fitness total
    float sum_fitness = 0.0;
    for (unsigned int i = 0; i < this->tamanio_poblacion; i++) {
        sum_fitness += this->poblacion[i].getFitness();
    }

    //Calculo de las probabilidades de cada individuo
    std::vector<float> probabilidades_individuos;
    std::vector<float> probabilidades_acumuladas;
    float q_i = 0.0; //utilizado para la probabilidad acumulada

    for (unsigned int i = 0; i < this->tamanio_poblacion; i++) {
        //Calculo la probabilidad de cada individuo
        float p_i = this->poblacion[i].getFitness() / sum_fitness;
        probabilidades_individuos.push_back(p_i);

        //Calculo la probabilidad acumulada de cada individuo
        q_i += p_i; //acumulo
        probabilidades_acumuladas.push_back(q_i);
    }

    //Genero tantos padres como me lo dice el parametro
    for (unsigned int w = 0; w < cantidad_a_generar; w++) {
        //Genero la posicion en la ruleta
        float posicion = utils::randomDecimal(0.0, 1.0);
        unsigned int buscador = 0;
        //Busco donde se da esa probabilidad
        while ((probabilidades_acumuladas[buscador+1] < posicion) and (buscador < probabilidades_acumuladas.size()-1))
            buscador++;
        //Una vez encontrado, pusheo
        nuevos_padres.push_back(this->poblacion[buscador]);
    }
}

//Metodo de ventanas para la seleccion
void AlgoritmoGenetico::ventanas(std::vector<Individuo> &nuevos_padres, unsigned int cantidad_a_generar) {
    nuevos_padres.clear();

    for (unsigned int i = 0; i < cantidad_a_generar; i++) {
        std::vector<Individuo> poblacion_ventaneada;
        //Copio una ventana de la poblacion
        poblacion_ventaneada.assign(this->poblacion.begin(), this->poblacion.end()-i);

        //Genero un indice al azar
        int idx = rand() % poblacion_ventaneada.size();

        //Inserto el nuevo progenitor
        nuevos_padres.push_back(poblacion_ventaneada[idx]);
    }
}


//Metodo de competencias para la seleccion
void AlgoritmoGenetico::competencia(std::vector<Individuo> &nuevos_padres, unsigned int cantidad_a_generar) {
    unsigned int npoblacion = this->poblacion.size();
    assert(npoblacion > 0);
    nuevos_padres.clear();

    //Creo un vector de índices para referenciar a la poblacion
    std::vector<int> vector_id_poblacion;

    for (unsigned int i = 0; i < npoblacion; i++ ) {
        vector_id_poblacion.push_back(i);
    }

    for (unsigned int w = 0; w < cantidad_a_generar; w++) {
        //Mezclo el array de indices
        std::random_shuffle(vector_id_poblacion.begin(), vector_id_poblacion.end() );

        unsigned int id_max_fitness = 0;
        float max_fitness = this->poblacion[ vector_id_poblacion[0] ].getFitness();

        for (unsigned int i = 1; i < this->k_competencia && i < npoblacion ; i++ ) {

            float fitness_i = this->poblacion[ vector_id_poblacion[i] ].getFitness();

            if (fitness_i > max_fitness) {
                max_fitness = fitness_i;
                id_max_fitness = i;
            }

        }

        nuevos_padres.push_back(this->poblacion[id_max_fitness]);
    }
}

//Realiza la cruza entre un padre y una madre, y guarda en hijos el resultado
void AlgoritmoGenetico::cruza(Individuo & padre, Individuo & madre, std::vector<Individuo> &hijos) {
    hijos.clear();
    unsigned int posicion_cruza;
    float prob = utils::randomDecimal(0.0,1.0);

    if (prob < this->probabilidad_cruza) {
        //No tocar esta linea o te arrancaré la piel! (Solo agente de viajero tiene id_funcion_activacion = 5)
        if (this->id_funcion_fitness == 5) {
            unsigned int paso = this->cantidad_genes / this->variables_fenotipo;
            posicion_cruza = (rand() % this->variables_fenotipo) * paso;
        } else {
            posicion_cruza = rand() % this->cantidad_genes;
        }
    } else {
        //No se cruzan, "se clonan los padres"
        hijos.push_back(padre);
        hijos.push_back(madre);
        return;
    }
    //Aumento contador
    this->cantidad_cruzas++;

    //Algoritmo de cruza
    Individuo hijo1(this->cantidad_genes, this->id_funcion_fitness, escala, variables_fenotipo);
    hijo1.agente_viajero = this->agente_viajero;
    Individuo hijo2(this->cantidad_genes, this->id_funcion_fitness, escala, variables_fenotipo);
    hijo2.agente_viajero = this->agente_viajero;

    hijo1.genotipo.clear();
    hijo2.genotipo.clear();

    hijo1.genotipo.insert(hijo1.genotipo.begin(), padre.genotipo.begin(), padre.genotipo.begin() + posicion_cruza);
    hijo2.genotipo.insert(hijo2.genotipo.begin(), madre.genotipo.begin(), madre.genotipo.begin() + posicion_cruza);

    hijo1.genotipo.insert(hijo1.genotipo.end(), madre.genotipo.begin() + posicion_cruza, madre.genotipo.end() );
    hijo2.genotipo.insert(hijo2.genotipo.end(), padre.genotipo.begin() + posicion_cruza, padre.genotipo.end() );

    hijos.push_back(hijo1);
    hijos.push_back(hijo2);
}



//Realiza la mutación de un padre en un hijo
void AlgoritmoGenetico::mutacion(Individuo &individuo_a_mutar) {
    //Control de probabilidad
    float prob = utils::randomDecimal(0.0,1.0);
    if (prob >= this->probabilidad_mutacion)
        return;

    //aumentamos contador
    this->cantidad_mutaciones++;

    //Sobrecarga para operar con el problema de agente.
    if (this->id_funcion_fitness == 5) {
        this->mutacionAgente(individuo_a_mutar);
        return;
    }

    unsigned int i_random = rand() % this->cantidad_genes;
    individuo_a_mutar.genotipo[i_random] = ! individuo_a_mutar.genotipo[i_random];
}



//Sobrecargamos el operador de mutacion para agente viajero
void AlgoritmoGenetico::mutacionAgente(Individuo &individuo_a_mutar) {

    unsigned int cantidad_ciudades = this->variables_fenotipo;
//    unsigned int cantidad_genes = this->cantidad_genes;
    unsigned int paso = this->cantidad_genes / cantidad_ciudades;

    unsigned int ciudad_a_mutar = rand() % cantidad_ciudades;
    unsigned int id_nueva_ciudad = rand() % cantidad_ciudades;

    std::vector<bool> id_nueva_ciudad_binary = utils::int2binary(id_nueva_ciudad, false);

    while (id_nueva_ciudad_binary.size() < 4 ) {
        id_nueva_ciudad_binary.insert(id_nueva_ciudad_binary.begin(),0);
    }

    std::vector<bool> nuevo_genotipo;
    nuevo_genotipo.insert(nuevo_genotipo.end(), individuo_a_mutar.genotipo.begin(), individuo_a_mutar.genotipo.begin() + paso * ciudad_a_mutar );
    nuevo_genotipo.insert(nuevo_genotipo.end(), id_nueva_ciudad_binary.begin(), id_nueva_ciudad_binary.end() );
    nuevo_genotipo.insert(nuevo_genotipo.end(), individuo_a_mutar.genotipo.begin() + paso * (ciudad_a_mutar + 1), individuo_a_mutar.genotipo.end() );

    individuo_a_mutar.genotipo = nuevo_genotipo;
}


//Devuelve el fitness de toda la poblacion
void AlgoritmoGenetico::getFitness(std::vector<float> &fitness_todos) {
    for (unsigned int i = 0; i < this->tamanio_poblacion; i++)
        fitness_todos.push_back(this->poblacion[i].getFitness());
}

//Devuelve el mejor fitness de la poblacion
float AlgoritmoGenetico::getMejorFitness() {
    return this->poblacion[id_maximo_fitness].getFitness();
}

//Devuelve el peor fitness de la poblacion
float AlgoritmoGenetico::getPeorFitness() {
    return this->poblacion[id_minimo_fitness].getFitness();
}

void AlgoritmoGenetico::getMejorGenotipo(std::vector<bool> &mejor_genotipo) {
    mejor_genotipo = this->poblacion[id_maximo_fitness].genotipo;
}

float AlgoritmoGenetico::getMejorSolucion() {
    return this->poblacion[id_maximo_fitness].getFenotipo();
}

void AlgoritmoGenetico::imprimirResumen() {
    std::cout<<"Cantidad de generaciones = "<<this->generacion_actual;
    std::cout<<"\nCantidad de cruzas = "<<this->cantidad_cruzas;
    std::cout<<"\nCantidad de mutaciones = "<<this->cantidad_mutaciones;
    //std::cout<<"\nMejor Fitness = "<<this->getMejorFitness()<< ", con fenotipo = "<<this->getMejorSolucion();
    std::cout<<"\nMejor Fitness = "<<this->getMejorFitness();
    std::cout<<"\nPeor Fitness = "<<this->getPeorFitness();
}

void AlgoritmoGenetico::cargarAgenteViajero(std::string nombre_archivo) {
    this->agente_viajero.leer(nombre_archivo);
    for (unsigned int i = 0; i < this->tamanio_poblacion; i++)
        this->poblacion[i].agente_viajero = this->agente_viajero;
}
