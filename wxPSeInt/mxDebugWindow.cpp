#include <wx/sizer.h>
#include <wx/stattext.h>
#include "mxDebugWindow.h"
#include "ids.h"
#include <wx/button.h>
#include <wx/scrolbar.h>
#include "DebugManager.h"
#include "mxMainWindow.h"
#include "ConfigManager.h"
#include "mxProcess.h"
#include "mxHelpWindow.h"
#include "mxDesktopVarsEditor.h"
#include "mxEvaluateDialog.h"

mxDebugWindow *debug_panel=NULL;

BEGIN_EVENT_TABLE(mxDebugWindow,wxPanel)
	EVT_BUTTON(mxID_DEBUG_BUTTON, mxDebugWindow::OnDebugButton)
	EVT_BUTTON(mxID_DEBUG_PAUSE, mxDebugWindow::OnDebugPause)
	EVT_BUTTON(mxID_DEBUG_STEP, mxDebugWindow::OnDebugStep)
	EVT_BUTTON(mxID_DEBUG_HELP, mxDebugWindow::OnDebugHelp)
	EVT_BUTTON(mxID_DEBUG_EVALUATE, mxDebugWindow::OnDebugEvaluate)
	EVT_BUTTON(mxID_DEBUG_DESKTOP_VARS, mxDebugWindow::OnDebugDesktopVars)
	EVT_CHECKBOX(mxID_DEBUG_STEP_IN, mxDebugWindow::OnDebugCheckStepIn)
END_EVENT_TABLE()	

mxDebugWindow::mxDebugWindow(wxWindow *parent):wxPanel(parent,wxID_ANY) {
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new wxStaticText(this,wxID_ANY,_T(" Estado:")),wxSizerFlags().Proportion(0).Expand().Border(wxTOP,10));
	debug_status = new wxStaticText(this,wxID_ANY,_T("No iniciada"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
	sizer->Add(debug_status,wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	sizer->Add(dp_button_run = new wxButton(this,mxID_DEBUG_BUTTON,_T("Comenzar")),wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	sizer->Add(dp_button_pause = new wxButton(this,mxID_DEBUG_PAUSE,_T("Pausar/Continuar")),wxSizerFlags().Proportion(0).Expand());
	sizer->Add(dp_button_step = new wxButton(this,mxID_DEBUG_STEP,_T("Primer Paso")),wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	sizer->Add(dp_button_desktop_vars = new wxButton(this,mxID_DEBUG_DESKTOP_VARS,_T("Prueba de Esc...")),wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	dp_button_desktop_vars->SetToolTip(utils->FixTooltip("Con este bot�n puede configurar una tabla con un conjunto de variables o expresiones para que ser�n evaluadas en cada paso de la ejecuci�n paso a paso y registradas en dicha tabla autom�ticamente para analizar luego la evoluci�n de los datos y el algoritmo."));
	sizer->Add(dp_button_evaluate = new wxButton(this,mxID_DEBUG_EVALUATE,_T("Evaluar...")),wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	dp_button_evaluate->SetToolTip(utils->FixTooltip("Puede utilizar este bot�n para evaluar una expresi�n o conocer el valor de una variable durante la ejecuci�n paso a paso. Para ello debe primero pausar el algoritmo."));
	sizer->Add(new wxStaticText(this,wxID_ANY,_T(" Velocidad:")),wxSizerFlags().Proportion(0).Expand().Border(wxTOP,10));
	debug_speed=new wxScrollBar(this,mxID_DEBUG_SLIDER);
	debug_speed->SetScrollbar(0,1,100,10);
	debug_speed->SetToolTip(utils->FixTooltip("Con este slider puede variar la velocidad con la que avanza autom�ticamente la ejecuci�n paso a paso."));
	sizer->Add(debug_speed,wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	dp_check_step_in=new wxCheckBox(this,mxID_DEBUG_STEP_IN,"Entrar en subprocesos");
	dp_check_step_in->SetToolTip(utils->FixTooltip("Cuando esta opci�n est� activada y el proceso llega a la llamada de una funci�n entra en dicha funci�n y muestra pasa a paso c�mo se ejecuta la misma, mientras que si est� desactivada ejecuta la llamada completa en un solo paso sin mostrar la ejecuci�n de la misma."));
	if (!config->lang.enable_user_functions) dp_check_step_in->Hide();
	sizer->Add(dp_check_step_in,wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	sizer->Add(new wxButton(this,mxID_DEBUG_HELP,_T("Ayuda...")),wxSizerFlags().Proportion(0).Expand().Border(wxTOP,10));
	SetState(DS_STOPPED);
	this->SetSizerAndFit(sizer);
	evaluate_window = new mxEvaluateDialog(parent);
}

void mxDebugWindow::SetSpeed(int speed) {
	debug_speed->SetThumbPosition(speed);
}

void mxDebugWindow::SetState(ds_enum state) {
	ds_state = state;
	switch (state) {
	case DS_STARTING:
		dp_button_run->SetLabel(_T("Finalizar"));
		dp_button_run->SetToolTip(utils->FixTooltip("Utilice este bot�n para detener definitivamente la ejecuci�n del algoritmo y abandonar el modo de ejecuci�n paso a paso."));
		debug_status->SetLabel(_T("Iniciando"));
		dp_button_step->Disable();
		dp_button_step->SetLabel(_T("Avanzar un Paso"));
		dp_button_step->SetToolTip(utils->FixTooltip("Utilice este bot�n para avanzar ejecutar solamente la siguiente instrucci�n del algoritmo."));
		dp_button_desktop_vars->Enable(false);
		break;
	case DS_STOPPED: 
		dp_button_run->SetLabel(_T("Comenzar"));
		dp_button_run->SetToolTip(utils->FixTooltip("Utilice este bot�n para que el algoritmo comience a ejecutarse autom�ticamente y paso a paso, se�alando cada instrucci�n que ejecuta, seg�n la velocidad definida en el men� configuraci�n."));
		dp_button_step->SetLabel(_T("Primer Paso"));
		dp_button_step->SetToolTip(utils->FixTooltip("Utilice este bot�n para ejecutar solo la primer instrucci�n del algoritmo y pausar inmediatamente la ejecuci�n del mismo."));
		dp_button_step->Enable();
		dp_button_desktop_vars->Enable();
		dp_button_evaluate->Disable();
		dp_button_step->Enable();
		dp_button_pause->Disable();
		debug_status->SetLabel(_T("No Iniciada"));
		break;
	case DS_FINALIZED:
		dp_button_run->SetLabel(_T("Cerrar"));
		dp_button_run->SetToolTip(utils->FixTooltip("Ha finalizado la ejecuci�n del algoritmo. Utilice este bot�n para cerrar la ventana de la ejecuci�n del mismo."));
		dp_button_pause->Disable();
		dp_button_step->Disable();
		debug_status->SetLabel(_T("Finalizada"));
		break;
	case DS_PAUSED:
		dp_button_step->Enable(true);
		dp_button_pause->Enable(true);
		dp_button_pause->SetFocus();
		dp_button_step->SetFocus();
		dp_button_pause->SetLabel(_T("Continuar"));
		dp_button_pause->SetToolTip(utils->FixTooltip("Utilice este bot�n para que el algoritmo contin�e avanzando paso a paso autom�ticamente."));
		dp_button_evaluate->Enable();
		debug_status->SetLabel(_T("Pausado"));
		break;
	case DS_RESUMED:
		dp_button_step->Enable(false);
		dp_button_pause->Enable(true);
		dp_button_pause->SetLabel(_T("Pausar"));
		dp_button_pause->SetToolTip(utils->FixTooltip("Utilice este bot�n para detener temporalmente la ejecuci�n del algoritmo. Al detener el algoritmo puede observar el valor de las variables con el bot�n Evaluar."));
		dp_button_evaluate->Disable();
		debug_status->SetLabel(_T("Ejecutando"));
		break;
	case DS_STEP:
		dp_button_pause->Disable();
		dp_button_evaluate->Disable();
		debug_status->SetLabel(_T("Ejecutando"));
		break;
	case DS_NONE:
		debug_status->SetLabel(_T("Desconocido"));
	}
}

void mxDebugWindow::OnDebugButton(wxCommandEvent &evt) {
	if (debug->debugging) {
		debug->Stop();
	} else {
		mxSource *src=main_window->GetCurrentSource();
		if (src) StartDebugging(src,false);
	}
}

void mxDebugWindow::OnDebugPause(wxCommandEvent &evt) {
	if (ds_state==DS_STEP) return;
	debug->Pause();
}

void mxDebugWindow::OnDebugStep(wxCommandEvent &evt) {
	if (ds_state==DS_STEP) return;
	if (debug->debugging) {
		debug->Step();
	} else {
		mxSource *src=main_window->GetCurrentSource();
		if (src) StartDebugging(src,true);
	}
}

void mxDebugWindow::StartDebugging(mxSource *source, bool paused) {
	debug_status->SetLabel(_T("iniciando..."));
	bool mod = source->GetModify();
	source->SaveFile(config->temp_file);
	source->SetModify(mod);
	debug->should_pause = paused;
	if ( (new mxProcess(source,source->GetPageText()))->Debug(config->temp_file, true) )
		SetState(DS_STARTING);
}
void mxDebugWindow::OnDebugHelp(wxCommandEvent &evt) {
	if (!helpw) helpw = new mxHelpWindow();
	helpw->ShowHelp(_T("debug.html"));
}
void mxDebugWindow::OnDebugDesktopVars(wxCommandEvent &evt) {
	new mxDesktopVarsEditor(this);
}
void mxDebugWindow::OnDebugEvaluate(wxCommandEvent &evt) {
	evaluate_window->Show();
}

void mxDebugWindow::SetEvaluationValue (wxString val) {
	evaluate_window->SetEvaluationValue(val);
}

bool mxDebugWindow::ProfileChanged ( ) {
	dp_check_step_in->Show(config->lang.enable_user_functions);
	Layout();
}

void mxDebugWindow::OnDebugCheckStepIn(wxCommandEvent &evt) {
	evt.Skip();
	debug->SetStepIn(dp_check_step_in->GetValue());
}
