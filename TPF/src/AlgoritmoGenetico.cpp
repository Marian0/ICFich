#include <algorithm>
#include "utils.h"
#include "AlgoritmoGenetico.h"
#include "Individuo.h"
#include <cstdio>
#include <map>

AlgoritmoGenetico::AlgoritmoGenetico(unsigned int tam_pob, unsigned int cant_genes, float escala, unsigned int variables_fenotipo, unsigned int max_gen, float pcruza, float pmutacion_movimiento, float pmutacion_permutacion, unsigned int elitismo, unsigned int brecha_generacional, unsigned int id_funcion_fitness, std::vector<Clase>  Clases,
                                     unsigned int aulas_disponibles, unsigned int metodo_seleccion, unsigned int k_competencia, unsigned int bits_por_materia,
                                     unsigned int modo_fitness,
                                     float pot_repeticiones, float pot_aulas, float pot_solapamiento, float pot_superposicion,
                                     float mult_repeticiones, float mult_aulas, float mult_solapamiento, float mult_superposicion) {

    //Copia las propiedades del algoritmo
    this->tamanio_poblacion = tam_pob;
    this->cantidad_genes = cant_genes;
    this->generaciones_maximo = max_gen;
    this->generacion_actual = 0;
    this->probabilidad_cruza = pcruza;
    this->probabilidad_mutacion_movimiento = pmutacion_movimiento;
    this->probabilidad_mutacion_permutacion = pmutacion_permutacion;
    this->metodo_seleccion = metodo_seleccion;
    this->k_competencia = k_competencia;
    this->n_elitismo = elitismo;
    this->n_brecha_generacional = brecha_generacional;
    this->id_funcion_fitness = id_funcion_fitness;
    this->cantidad_mutaciones_movimiento = 0;
    this->cantidad_mutaciones_permutacion = 0;
    this->cantidad_cruzas = 0;
    this->escala = escala;
    this->variables_fenotipo = variables_fenotipo;
    this->aulas_disponibles = aulas_disponibles;

    this->Clases = Clases;

    this->bits_por_materia = bits_por_materia;

    this->modo_fitness = modo_fitness;
    this->pot_cantidad_repeticiones = pot_repeticiones;
    this->pot_sobrepaso_aulas = pot_aulas;
    this->pot_solapamientos_adyacentes = pot_solapamiento;
    this->pot_superposicion = pot_superposicion;

    this->mult_cantidad_repeticiones = mult_repeticiones;
    this->mult_sobrepaso_aulas = mult_aulas;
    this->mult_solapamientos_adyacentes = mult_solapamiento;
    this->mult_superposicion = mult_superposicion;

    // A MODIFICAR
    // A MODIFICAR
    // A MODIFICAR
    // A MODIFICAR
    //Vectores que representan la distribucion de los horarios. Osea
    // QUe bloques son de 2 horas y que bloques son de 31
    this->Bloques2.push_back(0);
    this->Bloques2.push_back(5);
    this->Bloques2.push_back(10);
    this->Bloques2.push_back(15);
    this->Bloques2.push_back(20);
    this->Bloques2.push_back(2);
    this->Bloques2.push_back(7);
    this->Bloques2.push_back(12);
    this->Bloques2.push_back(17);
    this->Bloques2.push_back(22);
    this->Bloques2.push_back(3);
    this->Bloques2.push_back(8);
    this->Bloques2.push_back(13);
    this->Bloques2.push_back(18);
    this->Bloques2.push_back(23);
    this->Bloques3.push_back(1);
    this->Bloques3.push_back(6);
    this->Bloques3.push_back(11);
    this->Bloques3.push_back(16);
    this->Bloques3.push_back(21);
    this->Bloques3.push_back(4);
    this->Bloques3.push_back(9);
    this->Bloques3.push_back(14);
    this->Bloques3.push_back(19);
    this->Bloques3.push_back(24);



    //Crea todos los Individuos
    for (unsigned int i = 0; i < this->tamanio_poblacion; i++) {
        Individuo new_ind(this->cantidad_genes, this->id_funcion_fitness, this->Clases,
                          this->aulas_disponibles, this->escala, this->variables_fenotipo,
                          this->modo_fitness,
                          this->pot_cantidad_repeticiones, this->pot_sobrepaso_aulas,
                          this->pot_solapamientos_adyacentes, this->pot_superposicion,
                          this->mult_cantidad_repeticiones, this->mult_sobrepaso_aulas,
                          this->mult_solapamientos_adyacentes, this->mult_superposicion);
        this->poblacion.push_back(new_ind);
    }

    //Evaluo la poblacion
    this->evaluar();

    //Ordeno la población de mayor a menor fitness
    std::sort(this->poblacion.begin(), this->poblacion.end(), AlgoritmoGenetico::ordenarIndividuos);
}

//Crea la nueva generacion
void AlgoritmoGenetico::reproduccion() {
    //Actualizacion de generacion
    this->generacion_actual++;

    if(this->generacion_actual > this->generaciones_maximo) {
        std::cout<<"AAAAAAAAAAAAAAAAAAMaximo de generaciones alcanzadas\n";
//        return;
        getwchar();
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
        //Realizamos la cruza (Se obtendrán 2 hijos
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

    //Recalculamos las matrices de horarios
    for (unsigned int i = 0 ; i < this->tamanio_poblacion; i++ ) {
        this->poblacion[i].calcularMatrices();
    }

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

    //Tiro la moneda para ver si tengo que cruzar
    float prob = utils::randomDecimal(0.0,1.0);
    if (prob >= this->probabilidad_cruza) {
        //No se cruzan, "se clonan los padres"
        hijos.push_back(padre);
        hijos.push_back(madre);
        return;
    }

    // Se producirá la cruza => Aumento contador
    this->cantidad_cruzas++;


    unsigned int posicion_cruza1;
    unsigned int posicion_cruza2, temp;
    posicion_cruza1 = (rand() % this->variables_fenotipo )* this->bits_por_materia;
    posicion_cruza2 = (rand() % this->variables_fenotipo )* this->bits_por_materia;

    if (posicion_cruza1 > posicion_cruza2) {
        //swapping
        temp = posicion_cruza1;
        posicion_cruza1 = posicion_cruza2;
        posicion_cruza2 = temp;
    }

    //Algoritmo de cruza
    Individuo hijo1(  this->cantidad_genes, this->id_funcion_fitness, this->Clases,
                      this->aulas_disponibles, this->escala, this->variables_fenotipo,
                      this->modo_fitness,
                      this->pot_cantidad_repeticiones, this->pot_sobrepaso_aulas,
                      this->pot_solapamientos_adyacentes, this->pot_superposicion,
                      this->mult_cantidad_repeticiones, this->mult_sobrepaso_aulas,
                      this->mult_solapamientos_adyacentes, this->mult_superposicion);

    Individuo hijo2(  this->cantidad_genes, this->id_funcion_fitness, this->Clases,
                      this->aulas_disponibles, this->escala, this->variables_fenotipo,
                      this->modo_fitness,
                      this->pot_cantidad_repeticiones, this->pot_sobrepaso_aulas,
                      this->pot_solapamientos_adyacentes, this->pot_superposicion,
                      this->mult_cantidad_repeticiones, this->mult_sobrepaso_aulas,
                      this->mult_solapamientos_adyacentes, this->mult_superposicion);

    hijo1.genotipo.clear();
    hijo2.genotipo.clear();

    //Cruza hijo 1
    hijo1.genotipo.insert(hijo1.genotipo.end(), padre.genotipo.begin(), padre.genotipo.begin() + posicion_cruza1);
    hijo1.genotipo.insert(hijo1.genotipo.end(), madre.genotipo.begin() + posicion_cruza1 , madre.genotipo.begin() + posicion_cruza2 );
    hijo1.genotipo.insert(hijo1.genotipo.end(), padre.genotipo.begin() + posicion_cruza2 , padre.genotipo.end() );
    //Cruza hijo 2
    hijo2.genotipo.insert(hijo2.genotipo.end(), madre.genotipo.begin(), madre.genotipo.begin() + posicion_cruza1);
    hijo2.genotipo.insert(hijo2.genotipo.end(), padre.genotipo.begin() + posicion_cruza1 , padre.genotipo.begin() + posicion_cruza2 );
    hijo2.genotipo.insert(hijo2.genotipo.end(), madre.genotipo.begin() + posicion_cruza2 , madre.genotipo.end() );

    hijos.push_back(hijo1);
    hijos.push_back(hijo2);
}

//Realiza la mutación de un padre en un hijo
void AlgoritmoGenetico::mutacion(Individuo &individuo_a_mutar) {

    //Control de probabilidad para el metodo de mutacion 1
    float prob;
    prob = utils::randomDecimal(0.0,1.0);
    if (prob < this->probabilidad_mutacion_permutacion) {
        mutacionPermutacion(individuo_a_mutar);
        //aumentamos contador
        this->cantidad_mutaciones_permutacion++;
    }

    //Control de probabilidad para el metodo de mutacion 1
    prob = utils::randomDecimal(0.0,1.0);
    if (prob < this->probabilidad_mutacion_movimiento) {
        mutacionMovimiento(individuo_a_mutar);
        //aumentamos contador
        this->cantidad_mutaciones_movimiento++;
    }
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
    std::cout<<"\nCantidad de mutaciones por Movimiento = "<<this->cantidad_mutaciones_movimiento;
    std::cout<<"\nCantidad de mutaciones por Permutación = "<<this->cantidad_mutaciones_permutacion;
    std::cout<<"\nMejor Fitness = "<<this->getMejorFitness();
    std::cout<<"\nPeor Fitness = "<<this->getPeorFitness();

    std::vector<unsigned int> valores_mejor_fitness = this->poblacion[id_maximo_fitness].getValoresFitness();
    std::cout<<"\nParametros del mejor fitness:";
    std::cout<<"\n\tCantidad de repeticiones = "<<valores_mejor_fitness[0];
    std::cout<<"\n\tSobrepaso de aulas = "<<valores_mejor_fitness[1];
    std::cout<<"\n\tSolapamiento de Anios Adyacentes = "<<valores_mejor_fitness[2];
    std::cout<<"\n\tDos materias del mismo anio en un mismo bloque = "<<valores_mejor_fitness[3];
    std::cout<<"\nParametros del calculo de fitness:\n";
    if(this->modo_fitness == Individuo::FITNESS_PRODUCTO) {
        std::cout<<"\nForma de calculo del fitness = Producto";
        std::cout<<"\n\tPotencia Cantidad de Repeticiones = "<<this->pot_cantidad_repeticiones;
        std::cout<<"\n\tPotencia Sobrepaso de Aulas = "<<this->pot_sobrepaso_aulas;
        std::cout<<"\n\tPotencia Solapamientos adyacentes = "<<this->pot_solapamientos_adyacentes;
        std::cout<<"\n\tPotencia Superposicion = "<<this->pot_superposicion;
    } else if (this->modo_fitness == Individuo::FITNESS_SUMA) {
        std::cout<<"\nForma de calculo del fitness = Suma";
        std::cout<<"\n\tPeso Cantidad de Repeticiones = "<<this->mult_cantidad_repeticiones;
        std::cout<<"\n\tPeso Sobrepaso de Aulas = "<<this->mult_sobrepaso_aulas;
        std::cout<<"\n\tPeso Solapamientos Adyacentes = "<<this->mult_solapamientos_adyacentes;
        std::cout<<"\n\tPeso Superposicion = "<<this->mult_superposicion;

    }
    std::cout<<"\n";

}

void AlgoritmoGenetico::imprimirResumen(std::string nombre_archivo) {
    std::ofstream file(nombre_archivo.c_str(), std::ofstream::out|std::ofstream::trunc);
    assert(file.is_open());
    file<<"Generaciones = "<<this->generacion_actual;
    file<<"\nTamanio Poblacion = "<<this->tamanio_poblacion;
    file<<"\nElitismo = "<<this->n_elitismo;
    file<<"\nBrecha Generacional = "<<this->n_brecha_generacional;
    file<<"\nMetodo seleccion = "<<this->metodo_seleccion;
    file<<"\nProbabilidades:";
    file<<"\n\tCruza = "<<this->probabilidad_cruza;
    file<<"\n\tMutacion por movimiento = "<<this->probabilidad_mutacion_movimiento;
    file<<"\n\tMutacion por permutacion = "<<this->probabilidad_mutacion_permutacion;
    file<<"\nCantidad de operaciones:";
    file<<"\n\tCruzas = "<<this->cantidad_cruzas;
    file<<"\n\tMutaciones por Movimiento = "<<this->cantidad_mutaciones_movimiento;
    file<<"\n\tMutaciones por Permutación = "<<this->cantidad_mutaciones_permutacion;
    file<<"\nMejor Fitness = "<<this->getMejorFitness();
    file<<"\nPeor Fitness = "<<this->getPeorFitness();

    std::vector<unsigned int> valores_mejor_fitness = this->poblacion[id_maximo_fitness].getValoresFitness();
    file<<"\nParametros del mejor fitness:";
    file<<"\n\tCantidad de repeticiones = "<<valores_mejor_fitness[0];
    file<<"\n\tSobrepaso de aulas = "<<valores_mejor_fitness[1];
    file<<"\n\tSolapamiento de Anios Adyacentes = "<<valores_mejor_fitness[2];
    file<<"\n\tDos materias del mismo anio en un mismo bloque = "<<valores_mejor_fitness[3];
    file<<"\nParametros del calculo de fitness:\n";
    if(this->modo_fitness == Individuo::FITNESS_PRODUCTO) {
        file<<"\nForma de calculo del fitness = Producto";
        file<<"\n\tPotencia Cantidad de Repeticiones = "<<this->pot_cantidad_repeticiones;
        file<<"\n\tPotencia Sobrepaso de Aulas = "<<this->pot_sobrepaso_aulas;
        file<<"\n\tPotencia Solapamientos adyacentes = "<<this->pot_solapamientos_adyacentes;
        file<<"\n\tPotencia Superposicion = "<<this->pot_superposicion;
    } else if (this->modo_fitness == Individuo::FITNESS_SUMA) {
        file<<"\nForma de calculo del fitness = Suma";
        file<<"\n\tPeso Cantidad de Repeticiones = "<<this->mult_cantidad_repeticiones;
        file<<"\n\tPeso Sobrepaso de Aulas = "<<this->mult_sobrepaso_aulas;
        file<<"\n\tPeso Solapamientos Adyacentes = "<<this->mult_solapamientos_adyacentes;
        file<<"\n\tPeso Superposicion = "<<this->mult_superposicion;

    }

    file<<"\n";

    file.close();
}


void AlgoritmoGenetico::mutacionMovimiento(Individuo &individuo_a_mutar) {
    //Obtengo una posición al azar del fenotipo
    unsigned int posicion_random = rand() % (individuo_a_mutar.variables_fenotipo);

    //Obtengo el Genotipo del individuo
    std::vector<bool> genotipo = individuo_a_mutar.genotipo;
    //Variable para recortar el genotipo del individuo y obtener UN fenotipo
    std::vector<bool> genotipo_bloque;

    genotipo_bloque.insert( genotipo_bloque.begin(),
                           genotipo.begin() + posicion_random * this->bits_por_materia ,
                           genotipo.begin() + (posicion_random + 1) * this->bits_por_materia );
    //Obtengo el año
    unsigned int anio = this->Clases[posicion_random].anio;

    //Definimos la matriz a buscar de acuerdo al año
    std::vector<std::vector<bool> > matriz_horarios_bool = individuo_a_mutar.matriz_bool[anio];

    //Definimos en que rango buscar, si de 3 o de 2
    unsigned int horas = this->Clases[posicion_random].cantidad_horas;

    std::vector<unsigned int> bloques_a_buscar;
    if ( horas == 2 ) {
        bloques_a_buscar = this->Bloques2;
    } else {
        bloques_a_buscar = this->Bloques3;
    }

    unsigned int control_iteraciones = floor(bloques_a_buscar.size() * 2.5);
    bool buscar = true;
    while   (buscar) {

        unsigned int buscando = rand() % bloques_a_buscar.size();
        std::pair<unsigned int, unsigned int> posicion =  utils::posicionMatriz(5,5,bloques_a_buscar[buscando]);
        unsigned int i = posicion.first;
        unsigned int j = posicion.second;

        if (!matriz_horarios_bool[i][j]) {

            //Esta vacío!
            std::vector<bool> nuevo_genotipo;
            std::vector<bool> buscando_binary = utils::int2binary(buscando, false);


            while(buscando_binary.size() < this->bits_por_materia)
                buscando_binary.insert(buscando_binary.begin(), 0);


            nuevo_genotipo.insert( nuevo_genotipo.begin(),
                                   individuo_a_mutar.genotipo.begin(),
                                   individuo_a_mutar.genotipo.begin() + posicion_random * this->bits_por_materia
                                   );


            nuevo_genotipo.insert( nuevo_genotipo.end(),
                                   buscando_binary.begin(),
                                   buscando_binary.end()
                                   );

            nuevo_genotipo.insert( nuevo_genotipo.end(),
                                   individuo_a_mutar.genotipo.begin() + (posicion_random + 1) * this->bits_por_materia,
                                   individuo_a_mutar.genotipo.end()
                                   );
            individuo_a_mutar.genotipo = nuevo_genotipo;

            individuo_a_mutar.calcularMatrices();
            break;
        }

        control_iteraciones--;
        if (control_iteraciones == 0)
            return; //Control anti while true infinito
    }
    this->cantidad_mutaciones_movimiento++;
}

void AlgoritmoGenetico::mutacionPermutacion(Individuo &individuo_a_mutar) {

    //Obtengo una posición al azar del fenotipo
    unsigned int posicion_random = rand() % (individuo_a_mutar.variables_fenotipo);

    //Obtengo el Genotipo del individuo
    std::vector<bool> genotipo = individuo_a_mutar.genotipo;
    //Variable para recortar el genotipo del individuo y obtener UN fenotipo
    std::vector<bool> genotipo_bloque;

    genotipo_bloque.insert( genotipo_bloque.end(),
                           genotipo.begin() + posicion_random * this->bits_por_materia ,
                           genotipo.begin() + (posicion_random + 1) * this->bits_por_materia );

    //Obtengo el año de esa clase
    unsigned int anio = this->Clases[posicion_random].anio;
    //Obtengo la cantidad de horas que tiene la clase
    unsigned int horas = this->Clases[posicion_random].cantidad_horas;

    //Definimos en que rango buscar, si de 3 o de 2
    std::vector<unsigned int> bloques_a_buscar;
    if ( horas == 2 ) {
        bloques_a_buscar = this->Bloques2;
    } else {
        bloques_a_buscar = this->Bloques3;
    }

    //Busco una posicion para permutación válida
    int control_iteraciones = floor(bloques_a_buscar.size() * 2.5);
    unsigned int posicion_permutacion;
    while(true) {
        posicion_permutacion = rand() % individuo_a_mutar.variables_fenotipo;
        if (posicion_permutacion != posicion_random &&
                this->Clases[posicion_permutacion].cantidad_horas == horas &&
                this->Clases[posicion_permutacion].anio == anio ) {
            //Encontramos una posicion de la longitud deseada y que no es la misma que la random del inicio
            break;
        }

        control_iteraciones--;
        if (control_iteraciones == 0)
            return; //No se encuentra con quien permutar

    }
    //Swappeo si la posicion de permutacion es menor que la inicial
    unsigned int temp;
    if (posicion_random > posicion_permutacion) {
        temp = posicion_random;
        posicion_random = posicion_permutacion;
        posicion_permutacion = temp;
    }
    std::vector<bool> nuevo_genotipo, permutacion1_binary, permutacion2_binary;

    //Obtengo los valores del genotipo en la posicion
    permutacion1_binary.insert(permutacion1_binary.end(), individuo_a_mutar.genotipo.begin() + posicion_random * this->bits_por_materia, individuo_a_mutar.genotipo.begin() + (posicion_random + 1) * this->bits_por_materia  );
    permutacion2_binary.insert(permutacion2_binary.end(), individuo_a_mutar.genotipo.begin() + posicion_permutacion * this->bits_por_materia, individuo_a_mutar.genotipo.begin() + (posicion_permutacion + 1) * this->bits_por_materia  );

    nuevo_genotipo.insert( nuevo_genotipo.end(),
                            individuo_a_mutar.genotipo.begin(),
                            individuo_a_mutar.genotipo.begin() + posicion_random * this->bits_por_materia
                          );

    nuevo_genotipo.insert( nuevo_genotipo.end(),
                            permutacion2_binary.begin(),
                            permutacion2_binary.end()
                          );

    nuevo_genotipo.insert( nuevo_genotipo.end(),
                            individuo_a_mutar.genotipo.begin() + (posicion_random + 1) * this->bits_por_materia ,
                            individuo_a_mutar.genotipo.begin() + (posicion_permutacion) * this->bits_por_materia
                          );

    nuevo_genotipo.insert( nuevo_genotipo.end(),
                           permutacion1_binary.begin(),
                           permutacion1_binary.end()
                         );

    nuevo_genotipo.insert( nuevo_genotipo.end(),
                            individuo_a_mutar.genotipo.begin() + (posicion_permutacion + 1) * this->bits_por_materia,
                            individuo_a_mutar.genotipo.end()
                          );

    individuo_a_mutar.genotipo = nuevo_genotipo;

    individuo_a_mutar.calcularMatrices();

    this->cantidad_mutaciones_permutacion++;


}

std::vector<std::vector<std::vector<int> > > AlgoritmoGenetico::getSolucion() {
    unsigned int id_mejor = 0;
    float fit = this->poblacion[id_mejor].getFitness();
    for (unsigned int i = 1; i < this->poblacion.size(); i++) {
        float f = this->poblacion[i].getFitness();
        if (f > fit) {
            fit = f;
            id_mejor = i;
        }
    }
    this->poblacion[id_mejor].calcularMatrices();
    return this->poblacion[id_mejor].matriz_int;
    //return this->poblacion[this->id_maximo_fitness].matriz_int;
}

unsigned int AlgoritmoGenetico::getDiversidad() {
    unsigned int ret_val = 0;
    std::map<float, unsigned int> contador;
    for (unsigned int i = 0; i < this->poblacion.size(); i++) {
        float fitness = this->poblacion[i].getFitness();
        if (contador.find(fitness) == contador.end()) {
            contador[fitness] = 1;
        } else {
            contador[fitness]++;
        }
    }
    ret_val = contador.size();
    return ret_val;
}
