#include <iostream>
#include <cstdlib>
#include <vector>
//#include "utils.h"

int main(int argc, char *argv[])
{
    std::vector<float> V;

    for (unsigned int i = 0; i < 100; i++)
        V.push_back(i*random());

    V[2] = 123;


    std::cout<<V[2];

    //FERDI:::
    //ERES TAN EXCELENTEEE...
    //ERESSS MI MEEELODIAAAA


    std::cout<<"Bienvenidos al Ejercicio 4"<<std::endl;
    
    getwchar();
    return 0;
}
