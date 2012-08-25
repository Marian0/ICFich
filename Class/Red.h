#include <vector>
#include <string>
#include "Neurona.h"

#define EPS 0.001
class Red {
	private:
		//Vector de todas las neuronas que componen la Red
		std::vector<Neurona> neuronas;

		//Matriz de adyacencia para saber las conectividades entre Neuronas.
		std::vector<std::vector<bool> > adyacencias;

		//Matriz de adyacencia para saber las conectividades de las Entradas con las Neuronas.
		std::vector<std::vector<bool> > adyacencias_entradas;

		// Nombre identificador de la Red (Sirve como nombre de archivo de datos)
		std::string identificador;
		
		//Variable calculada para identificar que algoritmo de entrenamiento usar
		// en base a la estructura de la Red neuronal
		bool multicapa;
	public:

        //Construye la red leyendola desde un archivo pasado por argumento
        Red(std::string nombre_archivo,
            std::string identificador,
             double tasa_aprendizaje,
             unsigned int int_funcion_activacion
            ); 
		
        //En base a las adyacencias construimos la Red y sus interrelaciones con las neuronas y las entradas.
		Red(
			std::vector<std::vector<bool> > adyacencias,
			std::vector<std::vector<bool> > adyacencias_entradas, 
			std::string identificador,
			double tasa_aprendizaje = 0.5,
			unsigned int int_funcion_activacion = Neurona::FUNCION_SIGNO 
		);

        //Genera la estructura de la red, construyendo cada una de las neuronas
        void structureGenerator( double tasa_aprendizaje, unsigned int int_funcion_activacion); 
        
        //Imprime las matrices que dan forma a la estructura de la red
        void printStructure();
        
        //Lee la estructura del archivo dado, modificando las matrices adyacencias y adyacencias_entradas
        void readStructure(std::string nombre_archivo);

		//Devuelve el error en el entrenamiento
		//Comprueba la estructura y forma de la red para utilizar uno u otro algoritmo de entrenamiento
		bool train(std::vector<double> X, std::vector<double> YD, bool update = true);
		double train(std::vector<std::vector<double> > X, std::vector<std::vector<double> > YD, bool update = true);

		bool singleTrain(std::vector<double> X, std::vector<double> YD, bool update = true);
		void backpropagation() {}; //soon
		
		//Devuelvo el arreglo de neuronas para algunas cuestiones de graficación.
		void getNeurons(std::vector<Neurona> &N);
		
		//Grabo las neuronas por cuestiones de Entrenamiento optimo
		void setNeurons(std::vector<Neurona> &N);
};
