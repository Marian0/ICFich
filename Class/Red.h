#include <vector>
#include <string>
#include "Neurona.h"

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

		//En base a las adyacencias construimos la Red y sus interrelaciones con las neuronas y las entradas.
		Red(
			std::vector<std::vector<bool> > adyacencias,
			std::vector<std::vector<bool> > adyacencias_entradas, 
			std::string identificador,
			double tasa_aprendizaje = 0.5,
			unsigned int int_funcion_activacion = Neurona::FUNCION_SIGNO 
		);

		void saveData();
		void readData();

		//Devuelve el error en el entrenamiento
		//Comprueba la estructura y forma de la red para utilizar uno u otro algoritmo de entrenamiento
		void train(std::vector<double> X, std::vector<double> YD);
		void train(std::vector<std::vector<double> > X, std::vector<std::vector<double> > YD);

		void singleTrain(std::vector<double> X, std::vector<double> YD);
		void backpropagation() {}; //soon
		
		//Devuelvo el arreglo de neuronas para algunas cuestiones de graficación.
		void getNeuronas(std::vector<Neurona> &N);

};
