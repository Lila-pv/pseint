// * Este ejemplo es para ver cuales son las funciones equivalentes
// a las predefinidas en PSeInt.
// * Indicar en cada caso si hay que agregar alg�n #include, import
// o similar a la cabecera del programa
// * Si una funci�n no existe, pero es f�cil de implementar, ofrecer
// la implementaci�n para introducir cuando sea necesario. Por ejemplo,
// en C++ no hay funci�n para pasar toda una cadena a may�sculas pero
// se puede implementar f�cilmente aplicando toupper (que opera sobre
// un �nica caracter) a cada caracter de la cadena mediante un ciclo
// for.
// * Para las funciones trigonom�tricas, indicar si el argumento se
// recibe en grados o radianes.
// * En algunos casos pueden reemplazarse por expresiones. Por ejemplo,
// en c++ puedo obtener el truncado casteando a entero, o puedo concatenar
// strings con el operador +, o necesito aplicarle un mod al resultado de
// rand para tener un valor m�xim como en la funci�n Azar. Estos cambios
// no ameritan declarar una nueva funci�n, ya que pueden hacerse in-place
// sin generar mucho ruido.
// * Ninguna de las funciones de pseint modifican el argumento (el pasaje
// de parametros es siempre por copia).

Proceso sin_titulo
	x<-1.5;
	Escribir rc(a); // raiz cuadrada
	Escribir abs(i); // valor absoluto
	Escribir ln(x); // logaritmo natural
	Escribir exp(x); // exponencial, e^x
	Escribir sen(x); // seno
	Escribir cos(x); // coseno
	Escribir tan(x); // tangente
	Escribir asen(x); // arco-seno
	Escribir acos(x); // arco-coseno
	Escribir atan(x); // arco-tangente
	Escribir redon(x); // redondo a entero
	Escribir trunc(x); // truncamiento a entero
	Escribir azar(x); // numero al azar entre 0 y x-1 inclusive
	s<-"Hola";
	Escribir Longitud(s); // cantidad de letras
	Escribir May�sculas(s); // retorna una copia toda en mayusculas
	Escribir May�sculas(s); // retorna una copia toda en minusculas
	Escribir SubCadena(s,1,2); // retorna la cadena que desde la posicion 1 (segunda en base 0) a la 2 (tercera en base 0): "ol"
	Escribir concatenar(s," Mundo");
	Escribir ConvertirANumero("15.5"); // retorna un real (15.5)
	Escribir ConvertirATexto(15.5); // retorna una cadena ("15.5")
FinProceso
