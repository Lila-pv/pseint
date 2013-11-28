#ifndef EXPORT_PHP_H
#define EXPORT_PHP_H
#include <string>
#include "defines.h"
using namespace std;

/**
* @brief traduce la llamada a una funci�n predefinida a c++
*
* @param name    nombre de la funci�n en pseudoc�digo
* @param args    string con la lista de argumentos (incluye los par�ntesis)
**/
string php_function(string name, string args);



/**
* @brief traduce el algoritmo a PHP
*
* @param prog    argumento de salida, donde colocar las instrucciones traducidas
* @param alg     argumento de entrada, con el algoritmo ya parseado
**/
void translate_php(t_output &prog, t_programa &alg);

#endif

