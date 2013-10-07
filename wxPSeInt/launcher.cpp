#include <sys/wait.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
using namespace std;

const char *binname="wxPSeInt.bin";

void fix_argv(char *argv[]) {
	int n=strlen(argv[0])-1;
	char *argv0=new char[n+15];
	while (n>0&&argv[0][n]!='/') n--;
	if (argv[0][n]=='/') strncpy(argv0,argv[0],++n);
	strcpy(argv0+n,binname);
	argv[0]=argv0;
}


int main(int argc, char *argv[]) {
	setenv("UBUNTU_MENUPROXY","",1);
	setenv("LIBOVERLAY_SCROLLBAR","0",1);
	setenv("LANG","",1); // for fixing the problem with utf8 locale and ansi wx build
	setenv("LC_CTYPE","",1); // for fixing the problem with utf8 locale and ansi wx build
	fix_argv(argv);
	pid_t pid=fork();
	if (pid==0) {
		execvp(argv[0],argv);
	} else {
		int ret=0;
		waitpid(pid,&ret,0);
		if (ret) { // si algo fall� en la ejecuci�n, ver si faltan bibliotecas
			string s;
			s+="ldd \""; s+=argv[0]; s+="\""; 
			s+=" | grep \"not found\" >/dev/null";
			if (system(s.c_str())==0) {
				cerr<<endl<<endl;
				cerr<<"Puede que su sistema no tenga todas las bibliotecas necesarias para ejecutar PSeInt."<<endl;
				cerr<<"Instale las bibliotecas faltantes con el gestor de paquetes de su distribuci�n."<<endl;
				cerr<<"Las bibliotecas faltantes son:"<<endl;
				string s; s+="ldd \""; s+=argv[0]; s+="\""; 
				s+=" | grep \"not found\" | cut -d . -f 1";
				system(s.c_str());
			}
		}
		
	}
	return 0;
}

