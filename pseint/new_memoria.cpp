#include "new_memoria.h"

Memoria *memoria;
tipo_var vt_error(false,false,false);
tipo_var vt_desconocido(true,true,true);
tipo_var vt_logica(true,false,false);
tipo_var vt_numerica(false,true,false);
tipo_var vt_caracter(false,false,true);
tipo_var vt_caracter_o_numerica(false,true,true);
tipo_var vt_caracter_o_logica(true,true,true);