     // -------------------------------------- //
     //         PIPEH PSeudo Inerprete         //
     //      http://pseint.sourceforge.net     //
     // -------------------------------------- //

#include "version.h"
#include "zcurlib.h"
#include "common.h"
#include "intercambio.h"
#include "new_memoria.h"

#include "global.h"
#include "utils.h"
//#include "expresiones.h"
#include <csignal>
#include "new_evaluar.h"
#include "SynCheck.h"
#include "Ejecutar.h"
#include "new_funciones.h"
using namespace std;

void on_signal(int s) {
	Inter.SetFinished(true);
	Inter.UnInit();
	exit(s);
}
	 
//----------------------------------------------------------------------------
// ************************* Programa Principal ******************************
//----------------------------------------------------------------------------
int main(int argc, char* argv[]) {

	signal(2,on_signal);
	signal(6,on_signal);
	signal(9,on_signal);
	signal(15,on_signal);

	if (argc==2 && string(argv[1])=="--install-test") {cout<<"OK";return 0;}
	if (argc==3 && string(argv[1])==string("--version")) {
		ofstream fil(argv[2]);
		fil<<"PIPEH PSeInt "<<VERSION<<"-"ARCHITECTURE"\n by Pablo Novara - zaskar_84@yahoo.com.ar\n";
		fil.close();
		exit(0);
	}
	if (argc==2 && string(argv[1])==string("--version")) {
		cout<<"PIPEH PSeInt "<<VERSION<<"-"ARCHITECTURE"\n by Pablo Novara - zaskar_84@yahoo.com.ar\n";
		exit(0);
	}

	bool check=true, run=true, user=true, log_file=false, error=false, draw=false, undef_vars=true, var_definition=false;
	int fil_count=0,delay=0; char *fil_args[5];
	for (int i=1;i<argc;i++) {
		if (argv[i][0]=='-') {
			string str(argv[i]);
			if (str.substr(0,8)=="--delay=")
				delay=atoi(str.substr(8,str.size()-8).c_str());
			else if (str.substr(0,8)=="--input=")
				predef_input.push(str.substr(8,str.size()-8));
			else if (str.substr(0,7)=="--port=")
				Inter.SetPort(atoi(str.substr(7,str.size()-7).c_str()));
			else if (str=="--rawerrors")
				raw_errors=true;
			else if (str=="--norun")
				run=false;
			else if (str=="--forcedefinevars")
				var_definition=true;
			else if (str=="--color")
				colored_output=true;
			else if (str=="--nowordoperators")
				word_operators=false;
			else if (str=="--allowconcatenation")
				allow_concatenation=true;
			else if (str=="--overloadequal")
				overload_equal=true;
			else if (str=="--allowddims")
				allow_dinamyc_dimensions=true;
			else if (str=="--basezeroarrays")
				base_zero_arrays=true;
			else if (str=="--nocheck")
				check=false;
			else if (str=="--nouser")
				user=false;
			else if (str=="--noinput")
				noinput=true;
			else if (str=="--nolazysyntax")
				lazy_syntax=false;
			else if (str=="--forceinitvars")
				undef_vars=false;
			else if (str=="--forcedotandcomma")
				force_dot_and_comma=true;
			else if (str=="--coloquialconditions")
				coloquial_conditions=word_operators=true;
			else if (str=="--draw") {
				draw=true;
				run=false;
			} else if (str=="--easteregg") {
				cerr<<"Bazinga!"<<endl;
				return 0;
			} else 
				error=true;
		} else {
			fil_args[fil_count++]=argv[i];
		}
	}
	error = error&&((fil_count<(draw?2:1))||(fil_count>(draw?3:2)));
	log_file = fil_count>(draw?2:1);
#ifdef __APPLE__
	wait_key=false;
#else
	wait_key=user;
#endif

	// comprobar parametros
	string cadena;
	if (error || fil_count==0) {
		cout<<"Use: pseint FileName.psc [<options>] [LogFile]\n";
		cout<<"     pseint FileName.psc --draw DrawFile.psd [LogFile]\n";
		cout<<" <options> puede ser una o mas de las siguientes:"<<endl;
		cout<<"      --color                utilizar colores para formatear la salida"<<endl;
		cout<<"      --nocheck              no revisar la sintaxis"<<endl;
		cout<<"      --norun                no ejecutar"<<endl;
		cout<<"      --nowordoperators      no permitir utilzar las palabras y,o y no en lugar de &,| y ~"<<endl;
		cout<<"      --nouser               no mostrar mensajes de estado ni esperar un tecla al final"<<endl;
		cout<<"      --forcedefinevars      no permitir utilizar variables sin definir antes su tipo"<<endl;
		cout<<"      --forceinitvars        no permitir utilizar variables sin inicializar"<<endl;
		cout<<"      --forcedotandcomma     controlar el uso del punto y coma en las instrucciones secuenciales"<<endl;
		cout<<"      --allowddims           permitir utilizar variables para dimensionar arreglos"<<endl;
		cout<<"      --nolazysyntax         no permitir sintaxis flexible (omitir algunas palabras, o utilizar sinonimos)"<<endl;
		cout<<"      --basezeroarrays       trabajar con arreglos en base 0"<<endl;
		cout<<"      --overloadequal        permitir asignar con ="<<endl;
		cout<<"      --coloquialconditions  permite condiciones en lenguaje coloquial (ej: \"x es par\")"<<endl;
		cout<<"      --delay=<num>          define el retardo entre instrucciones para la ejecucion paso a paso"<<endl;
		cout<<"      --forcevardefinition   obliga a definir explicitamente los tipos de variable"<<endl;
		cout<<"      --port=<num>           define el puerto tpc para comunicar controlar la depuracion"<<endl;
		cout<<"      --rawerrors            muestra los errores sin descripcion, para testing automatizado"<<endl;
		cout<<"      --noinput              en lugar realizar las lecturas desde el teclado, lo hace desde los argumentos"<<endl;
		cout<<"      --input=<str>          sirve para predefinir uno o m�s valores de entrada para acciones LEER"<<endl;
		exit(1);
	}
	
	if (colored_output) { 
#ifndef __APPLE__
		setBackColor(COLOR_BLACK); 
#endif
		clrscr();
	}
	
	if (log_file) {
		ExeInfo.open(fil_args[draw?2:1]);
		if (ExeInfo.is_open()) ExeInfoOn=true;
	}

	// inicializaciones varias
	int errores;
	memoria = new Memoria;
	LoadFunciones();
	srand(time(NULL));
	
	// Leer el archivo	
	char *filename=fil_args[0];
	char buffer[1024];
	ifstream archivo(filename);
	if (!archivo.is_open()) {
		if (colored_output) setForeColor(COLOR_ERROR);
		cout<<"ERROR: No se pudo abrir el archivo."<<endl;
		if (ExeInfoOn) {
			ExeInfo<<"ERROR: No se pudo abrir el archivo."<<endl;
			if (user) {
				hideCursor();
				if (wait_key) getKey();
				showCursor();
			}
		}
		exit(1);
	}
	
	if (user && check) {
		if (colored_output) setForeColor(COLOR_INFO);
		cout<<"Leyendo archivo y comprobando sintaxis...\n";
	}
	while (!archivo.eof()) {
		archivo.getline(buffer,256);
		programa.push_back(buffer);
		prog_lines.push_back(0);
		Inter.AddLine(buffer);
	}
	archivo.close();

	// comprobar sintaxis y ejecutar
	errores=SynCheck();
	Inter.SetDelay(delay);
	

	//ejecutar
	if (errores==0) {
	// salida para diagrama
		if (draw) {
			ofstream dibujo(fil_args[1]);
			for (unsigned int i=0;i<programa.size();i++)
				dibujo<<programa[i]<<endl;
			dibujo.close();
			if (user) {
				if (colored_output) setForeColor(COLOR_INFO);
				cout<<"Dibujo guardado.\n";
			}
			ExeInfo.close();
			ExeInfoOn=false;
		} else if (run) {
			allow_undef_vars=undef_vars;
			force_var_definition=var_definition;
			if (ExeInfoOn) if (user) ExeInfo<<"*** Ejecucion Iniciada. ***\n";
			if (user) {
				if (colored_output) setForeColor(COLOR_INFO);
				cout<<"*** Ejecucion Iniciada. ***\n";
			}
			memoria->FakeReset();
//			Inter.SetLineNumber(prog_lines[0]);
			Inter.SetStarted();
			Ejecutar(2,programa.size()-2);
//			Inter.SetLineNumber(prog_lines[programa.size()-1]);
			Inter.SetFinished();
			if (ExeInfoOn) ExeInfo<<"*** Ejecucion Finalizada. ***";
			if (user) {
				if (colored_output) setForeColor(COLOR_INFO);
				cout<<endl<<"*** Ejecucion Finalizada. ***\n";
			}
		}
	} else {
		if (user) cout<<endl;
		if (errores==1) {
			if (ExeInfoOn) if (user) ExeInfo<<"*** Se encontro 1 error. ***";
			if (user) {
				if (colored_output) setForeColor(COLOR_INFO);
				cout<<"*** Se encontro 1 error. ***\n";
			}
		} else {
			if (user) {
				if (colored_output) setForeColor(COLOR_INFO);
				cout<<"*** Se encontraron "<<errores<<" errores. ***\n";
			}
			if (ExeInfoOn) if (user) ExeInfo<<"*** Se encontraron "<<errores<<" errores. ***";
		}
	}
	// esperar un enter si es necesario
	if (ExeInfoOn) {   
		ExeInfo.close();
	}
	if (user && !draw) {
		hideCursor();
		if (wait_key) getKey();
		showCursor();
	}
	
	return 0;
}


//--------------------------------------------------------------------------------------
