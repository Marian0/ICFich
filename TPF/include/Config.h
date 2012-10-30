#ifndef __CONFIG_H__
#define __CONFIG_H__


#include <map>
#include <string>

class Config{
private:
    //map que guarda las configuraciones
    std::map<std::string, std::string> config;
public:
	//Lee el archivo y llena el mapa
    Config(std::string name);    
	//Setea una clave con el valor dado
    void setValue(std::string key, std::string value);
	//Devuelve un valor, como string
    std::string getValue(std::string key);
	//Imprime el mapa entero
    void print();
};


#endif