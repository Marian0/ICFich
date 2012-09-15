#include <vector>
#include <numeric>
#include <algorithm>
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
    genFullStructure(nombre_archivo);
//    readStructure(nombre_archivo); 
    
    //Se guarda el id
	this->identificador = identificador;
    
    //Se guarda el parametro para la funcion sigmoidea
    this->parametro_sigmoidea = par_sigmoidea;
    
    //Se guarda el parametro de momento
    this->parametro_momento = parametro_momento;
    
    //Genera la estructura, creando las neuronas
    structureGenerator(tasa_aprendizaje, int_funcion_activacion); 
}

Red::Red(std::vector<std::vector<bool> > adyacencias,
		 std::vector<std::vector<bool> > adyacencias_entradas, 
		 std::string identificador,
		 float tasa_aprendizaje,
		 unsigned int int_funcion_activacion,
         float par_sigmoidea,
         float parametro_momento
		) {
   

    //Se guarda el parametro para la funcion sigmoidea
    this->parametro_sigmoidea = par_sigmoidea;
 
    //Se guarda el parametro de momento
    this->parametro_momento = parametro_momento;

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
    unsigned int na = adyacencias_entradas.size(); //Cantidad de entradas
    if (na > 0) {
        unsigned int ma = adyacencias_entradas[0].size(); //Cantidad de Neuronas

        //Detectamos la primera capa de neuronas (conectadas con entradas)
        std::vector<unsigned int> temp;
        for (unsigned int i = 0; i < ma; i++ ) {
            for (unsigned int j=0; j < na ; j++) {
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
                
                temp2.insert(temp2.end(), V.begin(), V.end());
            }  
            if (temp2.size() == 0)
                break; //No hay más capas porque a la derecha no hay nada.

            //Eliminamos duplicados
            std::sort(temp2.begin(), temp2.end());
            std::vector<unsigned int>::iterator it = std::unique(temp2.begin(), temp2.end());
            temp2.resize(it - temp2.begin());
            this->estructura.push_back(temp2);
            temp = temp2;
        }
    }

    //Inicializa el map
    for (unsigned int i = 0; i < this->estructura.size(); i++) {
        for (unsigned int j = 0; j < this->estructura[i].size(); j++) {
            unsigned int id_neurona = this->estructura[i][j];
            std::vector<float> v_t;
            unsigned int cantidad_pesos = this->neuronas[id_neurona].getW().size();
            //le insertamos todos ceros
            for (unsigned int k = 0; k < cantidad_pesos; k++) {
                v_t.push_back(0.0);
            }
            this->deltas_w_ji[ id_neurona ] = v_t;
        }
    }
}


//Devuelve el error en el entrenamiento
//Comprueba la estructura y forma de la red para utilizar uno u otro algoritmo de entrenamiento
bool Red::train(std::vector<float> X, std::vector<float> YD, bool update ) {	
	if (this->multicapa) {
		return backpropagation(X, YD, update);
    } else {
        return singleTrain(X,YD, update);
	}
}

//Wrapper
float Red::train(std::vector<std::vector<float> > X,
                  std::vector<std::vector<float> > YD, bool update){
    
    std::vector<std::vector<float> > ultimas_salidas;
    return this->train(X,YD, ultimas_salidas, update);


} 

//Ejecuta un conjunto de pruebas y devuelve el porcentaje de aciertos (efectividad)
float Red::train(std::vector<std::vector<float> > X,
				  std::vector<std::vector<float> > YD, std::vector<std::vector<float> > & last_output, bool update){ 
    unsigned int total_aciertos = 0;
    assert(X.size() == YD.size());
    last_output.clear(); //limpio el historico de salidas reales
    for (unsigned int i = 0; i < X.size(); i++){
        bool acierto = train(X[i], YD[i], update);
        last_output.push_back(this->last_output); //agrego la salida de cada entrenamiento
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
    
    bool salida_sin_error = true;
    
    //Estimulamos la red y hacemos la corrida hacia adelente

    //Respuestas de cada neurona de la red en correspondencia con this->estructura
    std::vector<std::vector<float> > respuestas;
    //Entradas que le entran a cada neurona (salidas de capas anteriores)
    std::vector<std::vector<std::vector<float> > > entradas_por_neurona; //necesario para calculos

    unsigned int n = this->estructura.size();
    //entradas_por_neurona tiene que tener el mismo tamaño que la cantidad de capas
    entradas_por_neurona.resize(n);

    for (unsigned int i = 0; i < n; i++) { //Recorremos por "capa" sobre la estructura
        unsigned int m = this->estructura[i].size();

        std::vector<float> respuestatemp;
        entradas_por_neurona[i].resize(m); //reservamos espacio para todas las neuronas de esta capa
        for (unsigned int j = 0 ; j < m ; j++) {
            //Recorremos cada neurona de la capa i
            std::vector<unsigned int> entradas_ids; //ids entradas
            std::vector<unsigned int> entradas_ids_neuronas; //ids neuronas

            //Obtenemos las neuronas de la capa anterior que se conectan a la actual
            //Si estan en la primer capa, entradas_ids_neuronas sera vacío
            //En el caso normal, si estan en una capa distinta a la primera, entradas_ids sera vacío
            this->getPrev( this->estructura[i][j], entradas_ids_neuronas, entradas_ids );
            
            //A continuacion recorremos las entradas y neuronas anteriores, quedandonos con sus salidas,
            //las cuales seran entradas a la neurona i,j

            std::vector<float> entradas_valor;
            //Buscamos valor de estimulos de las entradas X
            for (unsigned int w = 0; w < entradas_ids.size(); w++)
                entradas_valor.push_back(X[ entradas_ids[w] ]);
            
            //Buscamos las respuestas de neuronas anteriores
            for (unsigned int w = 0; w < entradas_ids_neuronas.size(); w++) {
                unsigned int capa, posicion;
                //Obtenemos la posicion de la neurona w en la matriz de respuestas
                this->getPosition(entradas_ids_neuronas[w], capa, posicion );
                //Guardo ese valor como una entrada para la siguiente capa
                //Respuestas ya tiene los valores de las capas anteriores
                entradas_valor.push_back( respuestas[capa][posicion] );
            }

            //en entradas_valor quedaron guardados los valores de las entradas a la neurona actual
            //(tanto de entradas a la red como de salidas de otras neuronas de capas anteriores)
            //Guardo esos valores en la matriz entradas_por_neuronas
            entradas_por_neurona[i][j] = entradas_valor;
            
            
            //Obtiene la id de la neurona en la capa i, posicion j
            unsigned int id_neurona = this->estructura[i][j];
           
            //Guarda la respuesta de esta neurona, para utilizarla en las neuronas de la capa siguiente
            respuestatemp.push_back(this->neuronas[id_neurona].getResponse(entradas_valor, parametro_sigmoidea) );
            
            /*
            std::cout<<"\t\t\t\t\t\t\t\t\t\t\t\tId neurona: "<<id_neurona<<'\n';
            std::cout<<"Entradas por neurona (i,j): ("<<i<<','<<j<<')'; utils::printVector(entradas_por_neurona[i][j]);
            std::cout<<"Entradas_Valor: "; utils::printVector(entradas_valor);
            std::cout<<"Respuesta: "; utils::printVector(respuestatemp);
            */

        } //Fin recorrido capa
        //guarda todas las respuestas de la capa i, que se utilizaran en la capa i+1
        respuestas.push_back(respuestatemp);       
    } //Fin de recorrido hacia adelante
   

    //Recorrido hacia atras

    //Vector donde se almacenaran los deltas del recorrido hacia atras
    std::vector<std::vector<float> > deltas;
    deltas.resize(n); //reservamos n filas, correspondiente a las n capas
  
    //Limpio la ultima salida.
    this->last_output.clear();

    //Calculo de los deltas (el i es el l del libro)
    for (int i = n-1; i >= 0; i--) {
        unsigned int m = this->estructura[i].size();
        deltas[i].resize(m); //reservamos m columnas, correspondiente a las m neuronas de esta capa
        for (unsigned int j = 0; j < m; j++) {
            std::vector<float> Wj, X;
            if (i == n-1) { //Capa de salida

                //Calculo del error
                //float error = respuestas[i][j] - YD[j];
                float error = YD[j] - respuestas[i][j];

                //Voy guardando las ultimas salidas por cuestiones de graficacion
                this->last_output.push_back(respuestas[i][j]);
                
                if ((salida_sin_error == true) && (fabs(error) > EPS)) { //no hubo error aun y son != (hay un error)
        			salida_sin_error = false;
		        }

                //Obtenemos los pesos de la neurona i,j
                Wj = this->neuronas[ this->estructura[i][j] ].getW();

                //Obtenemos las entradas de la neurona i,j
                //que son las salidas de la capa i-1
                X = entradas_por_neurona[i][j]; //X = y_j^(i-1)
                //Agrego el bias
                //X.insert(X.begin(), -1.0); 
                X.insert(X.begin(), 1.0); 

                //Calculo del v_j (local field)
                float localfield = utils::vectorPunto(X,Wj);
                //Evaluamos la derivada de la sigmoidea en el campo escalar
                
                float sigprima = utils::sigmoideaPrima(localfield, parametro_sigmoidea);
                //float sigprima = utils::sigmoideaPrima(respuestas[i][j]);
                
                /*
                std::cout<<"(i,j) = "<<i<<' '<<j<<"\nX: ";utils::printVector(X); std::cout<<"Wj: "; utils::printVector(Wj); 
                std::cout<<"Localfield = "<<localfield<<" Sigprima = "<<sigprima<<'\n';
                */

                //Guardamos el delta de esta neurona
                deltas[i][j] = sigprima * error;
                
            } else { //Capa oculta
                
                //Obtenemos los pesos de la neurona i,j
                Wj = this->neuronas[ this->estructura[i][j] ].getW();

                //Obtenemos las entradas de la neurona i,j
                X = entradas_por_neurona[i][j];
                //Agrego el bias
                //X.insert(X.begin(), -1.0); 
                X.insert(X.begin(), 1.0); 

                //Calculo del v_j (local field)
                float localfield = utils::vectorPunto(X,Wj);
                //Evaluamos la derivada de la sigmoidea en el campo escalar
                
                //float sigprima = utils::sigmoideaPrima(respuestas[i][j]);
                float sigprima = utils::sigmoideaPrima(localfield, parametro_sigmoidea);
                
                /*
                std::cout<<"(i,j) = "<<i<<' '<<j<<"\nX: ";utils::printVector(X); std::cout<<"Wj: "; utils::printVector(Wj); 
                std::cout<<"Localfield = "<<localfield<<" Sigprima = "<<sigprima<<'\n';
                */

                //obtener los deltas de la capa siguiente
                std::vector<float> deltatemp = deltas[i+1];
                
                //Obtener los pesos que conectan la neurona (i,j) con las neuronas de la capa siguiente (i+1,*)
                
                //obtenemos los ids de las neuronas de la capa siguiente
                std::vector<unsigned int> ids_next;
                this->getNext( this->estructura[i][j] , ids_next);

                //Obtenemos los pesos que queriamos, de la neurona i,j conectada a las neuronas de i+1,*
                std::vector<float> Wkj;
                for (unsigned int k = 0; k < ids_next.size(); k++ ) {
                    //Necesito solo los pesos de la conexión de la neurona con la capa siguiente solamente
                    Wkj.push_back( this->neuronas[ ids_next[k] ].getW()[j+1] );
                }                

                //hacer producto punto de los deltas y los pesos

                float deltak_wkj = utils::vectorPunto(deltas[i+1],Wkj);

                //deltas(i,j) = sigprima(localfield)*vectorPunto(deltas(i+1,*),pesos(*,j)
                
                deltas[i][j] = sigprima * deltak_wkj;
                
                //std::cout<<sigprima<<' '<<deltak_wkj<<"\n";
            }
        }
    }

    //utils::printVectorVector(deltas);
    //
    //Actualizar pesos
    //Para cada capa l:
    //delta(w(n)) = cte_aprendizaje*delta(l)*y(l-1)
    //w(n+1) = w(n) + cte_momento*delta(w(n-1)) + cte_aprendizaje*delta(l)*y(l-1)
    n = this->estructura.size();
    for (unsigned int i = 0; i < n; i++) { //Recorremos por "capa" sobre la estructura
        unsigned int m = this->estructura[i].size();
        for (unsigned int j = 0 ; j < m ; j++) {
            unsigned int id_neurona = this->estructura[i][j];

            std::vector<float> term1 = this->neuronas[ id_neurona ].getW();
            
            std::vector<float> term2, term2_t;
            term2_t = this->deltas_w_ji[ id_neurona ];

            utils::vectorEscalar(term2_t, this->parametro_momento, term2);

            //std::vector<float> pesos_anteriores = this->neuronas[ this->estructura[i][j] ].getWn_1();
//            utils::vectorEscalar(pesos_anteriores, this->parametro_momento, term2);
    
        
            std::vector<float> term3;
            //entradas por neurona = y_i(l-1)
            std::vector<float> entradas_ij = entradas_por_neurona[i][j];
            //entradas_ij.insert(entradas_ij.begin(), -1.0); //agrego el bias
            entradas_ij.insert(entradas_ij.begin(), 1.0); //agrego el bias
            
            utils::vectorEscalar(entradas_ij , this->neuronas[  id_neurona ].getConstanteAprendizaje() * deltas[i][j], term3);
           
            std::vector<float> term12;
            utils::vectorSuma(term1, term2, term12);
                
            //Guarda el delta actual para la sgte iteracion
            std::vector<float> term23;
            utils::vectorSuma(term2, term3, term23);
            this->deltas_w_ji[ id_neurona ] = term23;
            
            /*
            std::cout<<"(i,j) = "<<i<<' '<<j<<'\n';
            std::cout<<"term1: "; utils::printVector(term1);
            std::cout<<"term3: "; utils::printVector(term3);
         */

            std::vector<float> nuevoW;
            utils::vectorSuma(term12, term3, nuevoW);
           
            if (update) { //si quiero actualizar...
                this->neuronas[ id_neurona ].setW(nuevoW);
            }
        }
    }
//    std::getchar();
    return salida_sin_error;
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

    std::cout<<"Estructura por capas:\n";
    for (unsigned int i = 0; i < this->estructura.size(); i++) {
        for (unsigned int j = 0; j < this->estructura[i].size(); j++) {
            std::cout<<this->estructura[i][j]<<' ';
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


//Lee una estructura toda conexa desde un archivo
//La primer linea es la cantidad de entradas e
//La segunda linea es la cantidad de capas c
//En la tercer linea hay c valores, correspondientes a la cantidad de neuronas que hay en esa capa.
//Genera las matrices de adyacencias y adyacencias_entradas
void Red::genFullStructure(std::string nombre_archivo) {
    std::ifstream file(nombre_archivo.c_str());
    assert(file.is_open());
    unsigned int cant_entradas;
    unsigned int cant_capas;
    std::vector<unsigned int> neuronas_por_capa;
    
    file>>cant_entradas;
    file>>cant_capas;

    for (unsigned int i = 0; i < cant_capas; i++) {
        unsigned int new_val;
        file>>new_val;
        neuronas_por_capa.push_back(new_val);
    }
    file.close();
    
    
    unsigned int cant_neuronas = std::accumulate(neuronas_por_capa.begin(), neuronas_por_capa.end(), 0);

    std::vector<unsigned int> posiciones;
    //posiciones.resize(cant_neuronas);
    //en cada posicion de posiciones[i], pone la capa a la que pertenece
    for (unsigned int i = 0; i < cant_capas; i++) {
        //inserta el numero de capa tantas veces como neuronas haya en esa capa
        posiciones.insert(posiciones.end(), neuronas_por_capa[i], i);
    }

    
    //Redimensiona la matriz
    std::vector<std::vector<bool> > ady;
    ady.resize(cant_neuronas);
    for (unsigned int i = 0; i < cant_neuronas; i++) ady[i].resize(cant_neuronas,0);

    //Para cada neurona, me fijo cuales neuronas tiene en su capa siguiente y le asigno 1
    for (unsigned int i = 0; i < cant_neuronas; i++) {
        unsigned int capa_neurona_i = posiciones[i];
        for (unsigned int j = i+1; j < cant_neuronas; j++) {
            unsigned int capa_neurona_j = posiciones[j];
            if (capa_neurona_j == capa_neurona_i+1) { //si la neurona j esta en la capa siguiente
                ady[i][j] = 1;
            }
        }
    }

    std::vector<std::vector<bool> > ady_ent;
    //Generacion de la matriz de entradas
    ady_ent.resize(cant_entradas);
    for (unsigned int i = 0; i < cant_entradas; i++) ady_ent[i].resize(cant_neuronas, 0);
    
    for (unsigned int i = 0; i < cant_neuronas; i++) {
        unsigned int capa_neurona_i = posiciones[i];
        if (capa_neurona_i == 0) { //si la neurona i esta en la primer capa
            for (unsigned int k = 0; k < cant_entradas; k++) //le pongo en 1 cada una de las entradas
                ady_ent[k][i] = 1;
        }
    }
   

    //limpieza por las dudas
    this->adyacencias.clear();
    this->adyacencias_entradas.clear();

    //Asigna la nueva estructura
    this->adyacencias = ady;
    this->adyacencias_entradas = ady_ent;

    
    /* Impresiones educativas
    std::cout<<"Lectura del archivo "<<nombre_archivo<<"\nCantidad de Capas = "<<
            cant_capas<<"\nCantidad de Entradas = "<<cant_entradas<<"\nNeuronas por capa: ";
    utils::printVector(neuronas_por_capa);
    std::cout<<"Posiciones: "; utils::printVector(posiciones);
    std::cout<<"Adyacencias:\n";
    for(unsigned int i = 0; i < this->adyacencias.size(); i++){
        for (unsigned int j = 0; j < this->adyacencias[i].size(); j++)
            std::cout<<this->adyacencias[i][j]<<' ';
        std::cout<<'\n';
    }
    
    std::cout<<"Adyacencias Entradas:\n";
    for(unsigned int i = 0; i < this->adyacencias_entradas.size(); i++){
        for (unsigned int j = 0; j < this->adyacencias_entradas[i].size(); j++)
            std::cout<<this->adyacencias_entradas[i][j]<<' ';
        std::cout<<'\n';
    }
    */
    //std::getchar();
}
