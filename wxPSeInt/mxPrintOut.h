#ifndef MXPRINTOUT_H
#define MXPRINTOUT_H
#include<wx/print.h>

class mxSource;

extern wxPrintDialogData *printDialogData;
//extern wxPageSetupDialogData *pageSetupData;

/**
* @brief Gestiona la impresi�n de un fuente
* 
* Gestiona la impresi�n de un fuente utilizando la infraestructura de impresi�n
* de wxWidgets. Se encarga de calcular la cantidad de paginas, donde comienza
* cada una, y dialogar con el mxSource para renderizarlas en el dc.
* el codigo de esta clase esta basado en el ejemplo de stc que viene con 
* wxWidgets, aunque tiene varias modificaciones.
**/
class mxPrintOut: public wxPrintout {
	
public:
	
	//! constructor
	mxPrintOut (mxSource *source, wxString title = _T(""));
	
	//! evento: imprimir una pagina
	bool OnPrintPage (int page);
	//! evento: inicializar la impresi�n
	bool OnBeginDocument (int startPage, int endPage);
	
	//! determinar si existe una p�gina (fake)
	bool HasPage (int page);
	//! evento: recuperar informaci�n de la impresi�n
	void GetPageInfo (int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);
	
private:
	mxSource *source; ///< fuente a imprimir
	std::vector<int> pages;
	wxRect m_pageRect; ///< area de la p�gina completa
	wxRect m_printRect; ///< area de impresi�n
	
	bool PrintScaling (wxDC *dc);
};

#endif

