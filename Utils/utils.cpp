#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>



/*
Entrada: Path al archivo csv
Salida: Devuelve un vector de vector<double>  con el contenido de un archivo CSV
*/
/*
std::vector<std::vector<double> > parse_csv(char *filename) {
        std::ifstream f;
        f.open(filename);
        
        if(!f.is_open()){
                std::cout<<"No se pudo abrir el archivo: "<<filename<<std::endl;
                getchar();
                //exit(-1);
        }
		std::vector<std::vector<double> > X = std::vector<std::vector<double> >();
        double value;
        std::string s;
        //Lectura de los datos de entrenamiento
        while(getline(f,s)){
			std::stringstream ss(s);
			std::vector<double> tempy;
                while(ss>>value){
                        tempy.push_back(value);
                        if(ss.peek() == ',')
                                ss.ignore();
                }
                X.push_back(tempy);
        }
        return X;
}
*/

//Genera un numero aleatorio en el rango [a,b].
// NOTA: solo funciona para numeros de punto flotante
template<typename T>
T randomDecimal(T a, T b) {
	T random = ((T) rand()) / (T) RAND_MAX;
	T diff = b - a;
	T r = random * diff;
	return a + r;
}

//Funciones de activacion

//Funcion signo
double signo(double x){
	if(x < 0)
		return -1;
	else
		return 1;
}

//Funcion sigmodea
double sigmodea(double x){
	//...
	return 1;
}
