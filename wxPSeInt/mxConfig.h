#ifndef MXCONFIG_H
#define MXCONFIG_H
#include <wx/checkbox.h>
#include <wx/dialog.h>

class mxConfig:wxDialog {
private:
	wxCheckBox *chk_force_init_vars;
	wxCheckBox *chk_force_define_vars;
	wxCheckBox *chk_force_dot_and_comma;
	wxCheckBox *chk_base_zero_arrays;
	wxCheckBox *chk_allow_concatenation;
	wxCheckBox *chk_overload_equal;
	wxCheckBox *chk_coloquial_conditions;
	wxCheckBox *chk_lazy_syntax;
	wxCheckBox *chk_word_operators;
	wxCheckBox *chk_allow_dinamyc_dimensions;
public:
	mxConfig(wxWindow *parent);
	~mxConfig();
	void OnClose(wxCloseEvent &evt);
	void OnOkButton(wxCommandEvent &evt);
	void OnCancelButton(wxCommandEvent &evt);
	DECLARE_EVENT_TABLE();
};

#endif
