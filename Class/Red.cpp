#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include "Red.h"
#include "utils.h"

//Constructor que lee la estructura desde un archivo
Red::Red(std::string nombre_archivo,
        std::string identificador,
		 double tasa_aprendizaje,
		 unsigned int int_funcion_activacion
        ) {
    //Lee la estructura desde el archivo
    readStructure(nombre_archivo); 
    
    //Se guarda el id
	this->identificador = identificador;

    //Genera la estructura, creando las neuronas
    structureGenerator(tasa_aprendizaje, int_funcion_activacion); 
}

Red::Red(std::vector<std::vector<bool> > adyacencias,
		 std::vector<std::vector<bool> > adyacencias_entradas, 
		 std::string identificador,
		 double tasa_aprendizaje,
		 unsigned int int_funcion_activacion
		) {
    
    //Guarda la informacion de la estructura
    this->identificador = identificador;
	this->adyacencias = adyacencias;
	this->adyacencias_entradas = adyacencias_entradas;
    //Genera la estructura, creando las neuronas
    structureGenerator(tasa_aprendizaje, int_funcion_activacion);	

    /* Codigo viejo, candidato a ser desterrado de la faz de la tierra
    unsigned int n,m,ne,me;

    n = adyacencias.size();             //Filas
	assert(n > 0);                      //Control de que sea una matriz
	m = adyacencias[0].size();          //Columnas
	assert(m > 0 && m == n);            //Control de que sea una matriz y cuadrada
	
	ne = adyacencias_entradas.size();   //Filas de adyacencia
	assert(ne > 0);                     //Control de que sea una matriz
	me = adyacencias_entradas[0].size();
	assert(me > 0 && me == n);          //Control de que sea una matriz y que esten definidas 
                                        // la misma cantidad de neuronas en adyacencias_entradas 
	

    //Guardamos el atributo de identificador de la red.
	this->identificador = identificador;
	this->multicapa = false;
	this->adyacencias = adyacencias;
	this->adyacencias_entradas = adyacencias_entradas;

    //Instanciamos las neuronas, para ello debemos contar su dimensión.
	for (unsigned int i = 0; i < n; i++) {
		unsigned int dimension = 0;
		//Cuento la dimension debida a Neuronas
		for (unsigned int j = 0; j < n; j++) {
			if (adyacencias[j][i]) {
                //comprobar i != j ?
				dimension++;
				this->multicapa = true; //Detectamos que es multicapa ( ver despues, el tema de la identidad, osea que tiene una sola capa pero es recursivo)
			}
		}
		
		//Cuento la dimension debida a entradas
		for (unsigned int j = 0; j < ne; j++) {
			if (adyacencias_entradas[j][i]) {
				dimension++;
			}
		}
		Neurona neu(dimension, -0.5, 0.5, int_funcion_activacion , tasa_aprendizaje);
		this->neuronas.push_back(neu);
	}
    */
}

//Genera la estructura, llamado por los constructores. Crea las neuronas y se las guarda
void Red::structureGenerator( double tasa_aprendizaje, unsigned int int_funcion_activacion) {
    unsigned int n,m,ne,me;

    n = adyacencias.size();             //Filas
	assert(n > 0);                      //Control de que sea una matriz
	m = adyacencias[0].size();          //Columnas
	assert(m > 0 && m == n);            //Control de que sea una matriz y cuadrada
	
	ne = adyacencias_entradas.size();   //Filas de adyacencia
	assert(ne > 0);                     //Control de que sea una matriz
	me = adyacencias_entradas[0].size();
	assert(me > 0 && me == n);          //Control de que sea una matriz y que esten definidas 
                                        // la misma cantidad de neuronas en adyacencias_entradas 
	
	this->multicapa = false;

    //Instanciamos las neuronas, para ello debemos contar su dimensión.
	for (unsigned int i = 0; i < n; i++) {
		unsigned int dimension = 0;
		//Cuento la dimension debida a Neuronas
		for (unsigned int j = 0; j < n; j++) {
			if (this->adyacencias[j][i]) {
                //comprobar i != j ?
				dimension++;
				this->multicapa = true; //Detectamos que es multicapa ( ver despues, el tema de la identidad, osea que tiene una sola capa pero es recursivo)
			}
		}
		
		//Cuento la dimension debida a entradas
		for (unsigned int j = 0; j < ne; j++) {
			if (this->adyacencias_entradas[j][i]) {
				dimension++;
			}
		}
		Neurona neu(dimension, -0.5, 0.5, int_funcion_activacion , tasa_aprendizaje);
		this->neuronas.push_back(neu);
	}
}


//Devuelve el error en el entrenamiento
//Comprueba la estructura y forma de la red para utilizar uno u otro algoritmo de entrenamiento
bool Red::train(std::vector<double> X, std::vector<double> YD, bool update ) {	
	if (this->multicapa) {
		assert(false); //No esta programado asi q mato.
	} else {
		return singleTrain(X,YD, update);
	}
}

//Ejecuta un conjunto de pruebas y devuelve el porcentaje de aciertos (efectividad)
double Red::train(std::vector<std::vector<double> > X,
				  std::vector<std::vector<double> > YD, bool update){ 
    unsigned int total_aciertos = 0;
    assert(X.size() == YD.size());
    for (unsigned int i = 0; i < X.size(); i++){
        bool acierto = train(X[i], YD[i], update);
        if (acierto) 
            total_aciertos++;
        //guardar historial pesos
    }
    double porcentaje = ((double) total_aciertos) / ((double) X.size());
    return porcentaje;
}

//Devuelve true si no hubo error
// true: la red dio el resultado esperado
// false: dio resultado incorrecto
bool Red::singleTrain(std::vector<double> X, std::vector<double> YD, bool update) {
	//El single train supone red monocapa 
	unsigned int n = this->neuronas.size(); //Cantidad de neuronas en la primera capa
	assert(n>0); //Control de que al menos haya una neurona
	unsigned int ne = this->adyacencias_entradas.size(); //Filas de adyacencia

	assert(ne == X.size()); //Verificamos que se envien la cantidad de entradas necesarias para el entrenamiento de la RED

	bool salida_sin_error = true;
    //Se recorre cada una de las neuronas
	for (unsigned int i = 0; i < n ; i++) {
		std::vector<double> entradan;
	    
        //Recorrer cada entrada y filtrar las entradas que van para cada neurona
		for (unsigned int j = 0; j < ne; j++) {
			if (this->adyacencias_entradas[j][i]) {
				entradan.push_back(X[j]);
			}
		}
		
        //Estimular una neurona y obtener su respuesta // getResponse ya contempla el x0 = -1
		double respuesta = this->neuronas[i].getResponse(entradan);
		
		//Obtengo los pesos sinápticos actuales
		std::vector<double> Wi = this->neuronas[i].getW();
        

        // Verifico si hay error en alguna salida
        double error = fabs(YD[i] - respuesta);
        
		if ((salida_sin_error == true) && (error > EPS)) { //no hubo error aun y son != (hay un error)
			salida_sin_error = false;
		}
		//Calculo de los nuevos pesos
		//Parte Escalar
		respuesta = (YD[i] - respuesta) * ( this->neuronas[i].getConstanteAprendizaje()/2 );

        //Temporal para el producto
		entradan.insert(entradan.begin(), -1); //Agregamos la entrada correspondiente al Bias
		std::vector<double> vesc; 
		utils::vectorEscalar(entradan, respuesta, vesc);

        //Temporal para la suma
		std::vector<double> Wnuevo; 
		utils::vectorSuma(Wi, vesc, Wnuevo);

        //Actualizar pesos
		if (update) {
			this->neuronas[i].setW( Wnuevo );
		}
	}
	return salida_sin_error;
}

void backpropagation() {}; //soon


void Red::getNeurons(std::vector<Neurona> &N) {
	N = this->neuronas;
}

//Grabo las neuronas por cuestiones de Entrenamiento optimo
void Red::setNeurons(std::vector<Neurona> &N) {
	this->neuronas = N;
}
//Imprime la estructura de la red (adyacencias)
void Red::printStructure() {
    std::cout<<"Red:\n";

    for (unsigned int i = 0; i < this->adyacencias.size(); i++) {
        for (unsigned int j = 0; j < this->adyacencias[i].size(); j++) {
            std::cout<<this->adyacencias[i][j]<<' ';
        }
        std::cout<<std::endl;
    }
    
    std::cout<<"Entradas:\n";
    for (unsigned int i = 0; i < this->adyacencias_entradas.size(); i++) {
        for (unsigned int j = 0; j < this->adyacencias_entradas[i].size(); j++) {
            std::cout<<this->adyacencias_entradas[i][j]<<' ';
        }
        std::cout<<std::endl;
    }
}

//Lee la estructura de una red desde un archivo
// Formato:
// 0 0 1 
// 0 1 1
// 1 1 0
// # < indica que se terminaron las ady de red y empiezan las ady de entradas
// 1 0 0
// 1 0 0
// 1 0 0
void Red::readStructure(std::string nombre_archivo) {
    //apertura del archivo
    std::ifstream file(nombre_archivo.c_str());
    assert(file.is_open());
    
    std::string line;

    //lectura de las adyacencias de las neuronas
    std::vector<std::vector<bool> > ady;
    while(getline(file, line)) {
        if(line.length() > 0 && line[0] == '#')
            break; //termina de leer las adyacencias de las neuronas
        
        std::vector<bool> fila; //fila de la matriz
        
        //Le mando la linea al string stream
        std::stringstream stream;
        stream<<line;

        std::string value;
        //asignacion de la fila al vector
        while(stream>>value) {
            if (value[0] == '0')
                fila.push_back(false);
            else
                fila.push_back(true);
        }
        //guardo el vector en la matriz
        ady.push_back(fila);
    	stream.str(""); //limpia la string asociada
		stream.clear(); //limpia el stream
    }
   
    //WARNING: codigo duplicado, no hacer esto en casa

    //lectura de las adyacencias de las entradas
    std::vector<std::vector<bool> > ady_ent;
    while(getline(file, line)) {
        std::vector<bool> fila; //fila de la matriz
        
        //Le mando la linea al string stream
        std::stringstream stream;
        stream<<line;

        std::string value;
        //asignacion de la fila al vector
        while(stream>>value) {
            if (value[0] == '0')
                fila.push_back(false);
            else
                fila.push_back(true);
        }
        //guardo el vector en la matriz
        ady_ent.push_back(fila);
    	stream.str(""); //limpia la string asociada
		stream.clear(); //limpia el stream
    } 
    file.close();

    //limpieza por las dudas
    this->adyacencias.clear();
    this->adyacencias_entradas.clear();

    //Asigna la nueva estructura
    this->adyacencias = ady;
    this->adyacencias_entradas = ady_ent;

}
