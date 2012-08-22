#include "Config.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cassert>

Config::Config(std::string name){
	std::ifstream file (name.c_str());
	assert(file.is_open());
	std::string line;
	while (getline(file, line)) {
		if (line.size() > 0 && line[0] == '#') {//comentario, ignoro
			//std::cout<<line<<std::endl;
			continue;
		}
		else {
			unsigned int separator = line.find('=');
			std::string key = line.substr(0, separator);
			std::string value = line.substr(separator+1, line.size());
			setValue(key, value);
		}
	}
	file.close();
}

void Config::setValue(std::string key, std::string value){
	config[key] = value;
}

std::string Config::getValue(std::string key){
	std::map<std::string, std::string>::iterator p = config.find(key);
	if(p != config.end())
		return p->second;
	else
		return std::string("");
}

void Config::print(){
	std::map<std::string, std::string>::iterator p = config.begin();
	while (p != config.end()){
		std::cout<<p->first<<" = "<<p->second<<std::endl;
		p++;
	}
}
