#include <vector>
#include <string>
#include "Neurona.h"

#define EPS 0.1
class Red {
	private:
		//Vector de todas las neuronas que componen la Red
		std::vector<Neurona> neuronas;

		//Matriz de adyacencia para saber las conectividades entre Neuronas.
		std::vector<std::vector<bool> > adyacencias;

		//Matriz de adyacencia para saber las conectividades de las Entradas con las Neuronas.
		std::vector<std::vector<bool> > adyacencias_entradas;

        //Matriz para identificar la estructura de la Red
        std::vector<std::vector<unsigned int> > estructura;

		// Nombre identificador de la Red (Sirve como nombre de archivo de datos)
		std::string identificador;
		
		//Variable calculada para identificar que algoritmo de entrenamiento usar
		// en base a la estructura de la Red neuronal
		bool multicapa;

        //Parametro utilizado en Neurona::getResponse si se usa la funcion sigmoidea
        float parametro_sigmoidea;
	public:

        //Construye la red leyendola desde un archivo pasado por argumento
        Red(std::string nombre_archivo,
            std::string identificador,
             float tasa_aprendizaje,
             unsigned int int_funcion_activacion,
             float par_sigmoidea
            ); 
		
        //En base a las adyacencias construimos la Red y sus interrelaciones con las neuronas y las entradas.
		Red(
			std::vector<std::vector<bool> > adyacencias,
			std::vector<std::vector<bool> > adyacencias_entradas, 
			std::string identificador,
			float tasa_aprendizaje = 0.5,
			unsigned int int_funcion_activacion = Neurona::FUNCION_SIGNO,
            float par_sigmoidea = 50
		);

        //Genera la estructura de la red, construyendo cada una de las neuronas
        void structureGenerator( float tasa_aprendizaje, unsigned int int_funcion_activacion); 
        
        //Imprime las matrices que dan forma a la estructura de la red
        void printStructure();
        
        //Lee la estructura del archivo dado, modificando las matrices adyacencias y adyacencias_entradas
        void readStructure(std::string nombre_archivo);

		//Devuelve el error en el entrenamiento
		//Comprueba la estructura y forma de la red para utilizar uno u otro algoritmo de entrenamiento
		bool train(std::vector<float> X, std::vector<float> YD, bool update = true);
		float train(std::vector<std::vector<float> > X, std::vector<std::vector<float> > YD, bool update = true);

		bool singleTrain(std::vector<float> X, std::vector<float> YD, bool update = true);
		void backpropagation() {}; //soon
		
		//Devuelvo el arreglo de neuronas para algunas cuestiones de graficación.
		void getNeurons(std::vector<Neurona> &N);
		
		//Grabo las neuronas por cuestiones de Entrenamiento optimo
		void setNeurons(std::vector<Neurona> &N);
       
        //Devuelve true si la neurona de indice idx es neurona de salida
        bool esSalida(unsigned int idx);
        
        //Devuelve un vector de indices de neuronas y otro de entradas que alimentan a la neurona idx
        void getPrev(unsigned int idx, std::vector<unsigned int> &neu, std::vector<unsigned int> &ent);

        //Devuelve un vector de indices a donde la neurona idx le envia su salida
        void getNext(unsigned int idx, std::vector<unsigned int> &V);
        
        //Devuelve la posicion de la neurona idx en la estructura
        void getPosition(unsigned int idx, unsigned int &capa, unsigned int &pos );
};
