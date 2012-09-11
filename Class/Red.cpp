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
		 float tasa_aprendizaje,
		 unsigned int int_funcion_activacion,
         float par_sigmoidea,
         float parametro_momento
        ) {
    //Lee la estructura desde el archivo
    readStructure(nombre_archivo); 
    
    //Se guarda el id
	this->identificador = identificador;
    //Se guarda el parametro para la funcion sigmoidea
    this->parametro_sigmoidea = par_sigmoidea;
    //Genera la estructura, creando las neuronas
    structureGenerator(tasa_aprendizaje, int_funcion_activacion); 
}

Red::Red(std::vector<std::vector<bool> > adyacencias,
		 std::vector<std::vector<bool> > adyacencias_entradas, 
		 std::string identificador,
		 float tasa_aprendizaje,
		 unsigned int int_funcion_activacion,
         float par_sigmoidea
		) {
    
    //Se guarda el parametro para la funcion sigmoidea
    this->parametro_sigmoidea = par_sigmoidea;
 
    //Guarda la informacion de la estructura
    this->identificador = identificador;
	this->adyacencias = adyacencias;
	this->adyacencias_entradas = adyacencias_entradas;
    //Genera la estructura, creando las neuronas
    structureGenerator(tasa_aprendizaje, int_funcion_activacion);	
}

//Genera la estructura, llamado por los constructores. Crea las neuronas y se las guarda
void Red::structureGenerator( float tasa_aprendizaje, unsigned int int_funcion_activacion) {
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

    //Generaremos la estructura de la red en forma matricial
    unsigned int n = adyacencias_entradas.size(); //Cantidad de entradas
    if (n > 0) {
        unsigned int m = adyacencias_entradas[0].size(); //Cantidad de Neuronas

        //Detectamos la primera capa de neuronas (conectadas con entradas)
        std::vector<unsigned int> temp;
        for (unsigned int i = 0; i < m; i++ ) {
            for (unsigned int j=0; j < n ; j++) {
                if (adyacencias_entradas[j][i]) {
                    //La neurona i está conectada a la entrada j
                    temp.push_back(i); 
                    break;
                }
            }
        }
        //pushbackeamos las entradas a la estructura
        this->estructura.push_back(temp);

        while (true) {
            std::vector<unsigned int> temp2;
            for (unsigned int i = 0; i < temp.size(); i++) {
                std::vector<unsigned int> V;
                this->getNext(temp[i], V);

                temp2.push_back(V);
            }  
            if (temp2.size() == 0)
                break; //No hay más capas porque a la derecha no hay nada.

            //Eliminamos duplicados
            std::sort(temp2.begin(), temp2.end());
            std::unique(temp2.begin(), temp2.end());
            this->estructura.push_back(temp2);
        }


    }

}


//Devuelve el error en el entrenamiento
//Comprueba la estructura y forma de la red para utilizar uno u otro algoritmo de entrenamiento
bool Red::train(std::vector<float> X, std::vector<float> YD, bool update ) {	
	if (this->multicapa) {
		return backpropagation(X,YD, update);
    } else {
        return singleTrain(X,YD, update);
	}
}

//Ejecuta un conjunto de pruebas y devuelve el porcentaje de aciertos (efectividad)
float Red::train(std::vector<std::vector<float> > X,
				  std::vector<std::vector<float> > YD, bool update){ 
    unsigned int total_aciertos = 0;
    assert(X.size() == YD.size());
    for (unsigned int i = 0; i < X.size(); i++){
        bool acierto = train(X[i], YD[i], update);
        if (acierto) 
            total_aciertos++;
    }
    float porcentaje = ((float) total_aciertos) / ((float) X.size());
    return porcentaje;
}

bool Red::backpropagation(std::vector<float> X,
                  std::vector<float> YD, bool update){ 

    //Verifico que la cantidad de entradas sea propiamente la que la red necesita
    assert(X.size() == this->adyacencias_entradas.size());    
    //Estimulamos la red y hacemos la corrida hacia adelente

    //Respuestas de cada neurona de la red en correspondencia con this->estructura
    std::vector<std::vector<float> > respuestas;
    std::vector<std::vector<std::vector<float> > > entradas_por_neurona; //necesario para calculos

    unsigned int n = this->estructura.size();
    entradas_por_neurona.resize(n);
    for (unsigned int i = 0; i < n; i++) { //Recorremos por "capa" sobre la estructura
        unsigned int m = this->estructura[i].size();

        std::vector<float> respuestatemp;
        entradas_por_neurona[i].resize(m);
        for (unsigned int j = 0 ; j < m ; j++) {
            //Recorremos cada neurona de la capa i
            std::vector<unsigned int> entradas_ids; //ids entradas
            std::vector<unsigned int> entradas_ids_neuronas; //ids neuronas

            //Obtenemos las neuronas de la capa anterior que se conectan a la actual
            this->getPrev( this->estructura[i][j], entradas_ids_neuronas, entradas_ids );

            std::vector<float> entradas_valor;
            //Buscamos valor de estimulos de las entradas X
            for (unsigned int w = 0; entradas_ids.size(); w++)
                entradas_valor.push_back(X[ entradas_ids[w] ]);

            //Buscamos las respuestas de neuronas anteriores
            for (unsigned int w = 0; entradas_ids_neuronas.size(); w++) {
                unsigned int capa, posicion;
                //Obtenemos la posicion de la neurona w en la matriz de respuestas
                this->getPosition(entradas_ids_neuronas[w], capa, posicion );
                //Guardo ese valor como una entrada para la siguiente capa
                entradas_valor.push_back( respuestas[capa][posicion] );           
            }

            entradas_por_neurona[i][j] = entradas_valor;
            
            //Obtiene la id de la neurona en la capa i, posicion j
            unsigned int id_neurona = this->estructura[i][j];
            
            //Guarda la respuesta de esta neurona, para utilizarla en las neuronas de la capa siguiente
            respuestatemp.push_back(this->neuronas[id_neurona].getResponse(entradas_valor) );

        } //Fin recorrido capa
        respuestas.push_back(respuestatemp);       
    } //Fin de recorrido hacia adelante
    
    //Recorrido hacia atras

    //Vector donde se almacenaran los deltas del recorrido hacia atras
    std::vector<std::vector<float> > deltas;
    deltas.resize(n); //reservamos n filas, correspondiente a las n capas
    
    //Calculo de los deltas (el i es el l del libro)
    for (unsigned int i = n; i >= 0; i--) {
        unsigned int m = this->estructura[i].size();
        deltas[i].resize(m); //reservamos m columnas, correspondiente a las m neuronas de esta capa
        for (unsigned int j = 0; j < m; j++) {
            
            std::vector<float> Wj, X;
            if (i==n) { //Capa de salida

                //Calculo del error
                float error = YD[j] - respuesta[i][j];

                //Obtenemos los pesos de la neurona i,j
                Wj = this->neuronas[ this->estructura[i][j] ].getW();

                //Obtenemos las entradas de la neurona i,j
                X = entradas_por_neurona[i][j];
                //Agrego el bias
                X.insert(X.begin(), -1.0); 

                //Calculo del v_j (local field)
                float localfield = utils::vectorPunto(X,Wj);
                //Evaluamos la derivada de la sigmoidea en el campo escalar
                float sigprima = utils::sigmoideaPrima(localfield);
                //Guardamos el delta de esta neurona
                deltas[i][j] = error * sigprima;
            } else { //Capa oculta
                
                //TODO: 
                  //Obtenemos los pesos de la neurona i,j
                Wj = this->neuronas[ this->estructura[i][j] ].getW();

                //Obtenemos las entradas de la neurona i,j
                X = entradas_por_neurona[i][j];
                //Agrego el bias
                X.insert(X.begin(), -1.0); 

                //Calculo del v_j (local field)
                float localfield = utils::vectorPunto(X,Wj);
                //Evaluamos la derivada de la sigmoidea en el campo escalar
                float sigprima = utils::sigmoideaPrima(localfield);

                //* obtener los deltas de la capa siguiente
                std::vector<float> deltatemp = deltas[i+1];
                
                //* obtener los pesos que conectan la neurona (i,j) con las neuronas de la capa siguiente (i+1,*)
                std::vector<unsigned int> ids_next;
                this->getNext( this->estructura[i][j] , ids_next);

                std::vector<float> Wkj;
                for (unsigned int k = 0; k < ids_next.size(); k++ ) {
                    //Necesito solo los pesos de la conexión de la neurona con la capa siguiente solamente
                    Wkj.push_back( this->neuronas[ ids_next[k] ].getW()[j] );
                }                

                //* hacer producto punto de los deltas y los pesos
                float deltak_wkj = utils::vectorPunto(deltas[i+1],Wkj);

                //* deltas(i,j) = sigprima(localfield)*vectorPunto(deltas(i+1,*),pesos(*,j)
                deltas[i][j] = sigprima * deltak_wkj;

            }
        }
    }
    //Actualizar pesos
    //Para cada capa l:
    //w(n+1) = w(n) + cte_momento*(w(n-1) + cte_aprendizaje*delta(l)*y(l-1)
    

    //**** Reever porque estamos actualizando todos los pesos juntos y no sabemos como obtener y_i^(l-1)
<<<<<<< HEAD

    n = this->estructura.size();
    for (unsigned int i = 0; i < n; i++) { //Recorremos por "capa" sobre la estructura
        unsigned int m = this->estructura[i].size();
        for (unsigned int j = 0 ; j < m ; j++) {
            std::vector<float> term1 = this->neuronas[ this->estructura[i][j] ].getW();
            
            std::vector<float> term2;
            utils::vectorEscalar(this->neuronas[ this->estructura[i][j] ].getWn_1(), this->parametro_momento, term2);
        
            std::vector<float> term3;
            utils::vectorEscalar(entradas_por_neurona[i][j], this->neuronas[  this->estructura[i][j] ].getConstanteAprendizaje() * deltas[i][j], term3);
            
            std::vector<float> term12;
            utils::vectorSuma(term1, term2, term12);
            
            std::vector<float> nuevoW;
            utils::vectorSuma(term12, term3, nuevoW);
=======
    for (unsigned int i = 0; i < n; i++) { //recorro neurona
        std::vector<float> term1 = this->neuronas[i].getW();
        std::vector<float> term2;
        utils::vectorEscalar(this->neuronas[i].getWn_1(), this->parametro_momento, term2);
        
        std::vector<float> term3;
        unsigned int capa, posicion;
        this->getPosition(i, capa, posicion);
        
        utils::vectorEscalar(respuestas[capa-1], this->neuronas[i].getConstanteAprendizaje() * deltas[capa][posicion], term3);

        this->neuronas[i].setW();
>>>>>>> 772b3a61d6b23d9f2cab40136fbc8ae3ca6e15f7

            this->neuronas[ this->estructura[i][j]  ]->setW(nuevoW);
        }
    }

}

//Devuelve true si no hubo error
// true: la red dio el resultado esperado
// false: dio resultado incorrecto
bool Red::singleTrain(std::vector<float> X, std::vector<float> YD, bool update) {
	//El single train supone red monocapa 
	unsigned int n = this->neuronas.size(); //Cantidad de neuronas en la primera capa
	
    assert(n>0); //Control de que al menos haya una neurona
	
    unsigned int ne = this->adyacencias_entradas.size(); //Filas de adyacencia

	assert(ne == X.size()); //Verificamos que se envien la cantidad de entradas necesarias para el entrenamiento de la RED

	bool salida_sin_error = true;
    //Se recorre cada una de las neuronas
	for (unsigned int i = 0; i < n ; i++) {
		std::vector<float> entradan;
	    
        //Recorrer cada entrada y filtrar las entradas que van para cada neurona
		for (unsigned int j = 0; j < ne; j++) {
			if (this->adyacencias_entradas[j][i]) {
				entradan.push_back(X[j]);
			}
		}
		
        //Estimular una neurona y obtener su respuesta // getResponse ya contempla el x0 = -1
		float respuesta = this->neuronas[i].getResponse(entradan, parametro_sigmoidea);
		
		//Obtengo los pesos sinápticos actuales
		std::vector<float> Wi = this->neuronas[i].getW();
        

        // Verifico si hay error en alguna salida
        float error = fabs(YD[i] - respuesta);
		if ((salida_sin_error == true) && (error > EPS)) { //no hubo error aun y son != (hay un error)
			salida_sin_error = false;
		}
		//Calculo de los nuevos pesos
		//Parte Escalar
		float parte_escalar = (YD[i] - respuesta) * ( this->neuronas[i].getConstanteAprendizaje()/2 );

        //Temporal para el producto
		entradan.insert(entradan.begin(), -1); //Agregamos la entrada correspondiente al Bias
		std::vector<float> vesc; 
		utils::vectorEscalar(entradan, parte_escalar, vesc);


        //Temporal para la suma
		std::vector<float> Wnuevo; 
		utils::vectorSuma(Wi, vesc, Wnuevo);

        //Actualizar pesos
		if (update) {
			this->neuronas[i].setW( Wnuevo );
		}
	}
    
    return salida_sin_error;
}

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

//Devuelve true si la neurona idx es una neurona de salida
//Su linea de adyacencias es cero.
//Su columna de adyacencias no es cero. < si no seria una neurona isleña
bool Red::esSalida(unsigned int idx){
    assert(idx < adyacencias.size());
    
    //Comprobamos que su linea de adyacencias hacia adelante sea cero
    for (unsigned int j = 0; j < adyacencias[idx].size(); j++) {
        if (adyacencias[idx][j])
            //si esta conectada hacia adelante a alguna neurona, 
            //entonces no es capa de salida
            return false;
    }

    //Comprobamos que su columna de adyacencias hacia atras sea distinta de cero
    for (unsigned int i = 0; i < adyacencias.size(); i++) {
        if(adyacencias[i][idx])
            //alguna entrada la tiene a su salida, entonces
            //podemos decir que es una neurona de salida
            return true; 
    }
    return false; //es una neurona isleña
}

//Dado un indice de neurona, devuelve las neuronas de su capa siguiente a las que ésta esta conectada
void Red::getNext(unsigned int idx, std::vector<unsigned int> &V){
    assert(idx < adyacencias.size());
    
    std::vector<unsigned int> temp;
    //Recorre la fila de las adyacencias y si esta conectada, la agrega al vector
    for (unsigned int j = 0; j < adyacencias[idx].size(); j++) {
        if (adyacencias[idx][j])
            temp.push_back(j);
    }
    V = temp;
}


//Dado un indice de neurona, devuelve las neuronas de su capa anterior y las entradas, si las hubiere
void Red::getPrev(unsigned int idx, std::vector<unsigned int> &neu, std::vector<unsigned int> &ent){
    assert(idx < adyacencias.size());
    
    std::vector<unsigned int> temp;
    
    //recorre la fila de las adyacencias y si esta conectada(hacia atras), la agrega al vector
    for (unsigned int i = 0; i < adyacencias.size(); i++) {
        if (adyacencias[i][idx])
            temp.push_back(i);
    }
    neu = temp;

    temp.clear();
    
    //recorre la fila de las entradas y si esta conectada (hacia atras), la agrega al vector
    for (unsigned int i = 0; i < adyacencias_entradas.size(); i++) {
        if (adyacencias_entradas[i][idx])
            temp.push_back(i);
    }
    ent = temp;
}

//Devuelve los indices i,j donde esta ubicada la neurona idx
//capa: capa en la que se ubica
//pos: posicion dentro de la capa
void Red::getPosition(unsigned int idx, unsigned int &capa, unsigned int &pos ) {
    for (unsigned int i = 0; i < this->estructura.size(); i++) {
        for (unsigned int j = 0; j < this->estructura[i].size(); j++) {
            if (estructura[i][j] == idx) {
                capa = i;
                pos = j;
                return;
            }
        }
    }
    std::cout<<"No se encontro la posicion de la neurona. El programa acaba de explotar\n";
    assert(false);
}


