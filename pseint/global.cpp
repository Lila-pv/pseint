#include "global.h"

queue <string> predef_input; // entrada predefinida para los tests
vector <string> programa; // programa con sintaxis reorganizada para ejecutar
vector <int> prog_lines;  // correspondencia con el c�digo original (por numeros de lineas)
ofstream ExeInfo; // Archivo de Inforamacion de Ejecucion para el entorno
bool ExeInfoOn=false;
bool wait_key;
int SynErrores=0; // Contador para errores de Sintaxis
bool raw_errors=false;
bool allow_undef_vars=true;
bool force_dot_and_comma=false;
bool word_operators=true;
bool colored_output=false;
bool allow_dinamyc_dimensions=false;
bool allow_concatenation=true;
bool base_zero_arrays=false;
bool lazy_syntax=true;
bool overload_equal=false;
bool coloquial_conditions=false;
bool force_var_definition=false;
bool noinput=false;