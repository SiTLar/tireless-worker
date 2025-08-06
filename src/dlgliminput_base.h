///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/valgen.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class dlgLimInputbase
///////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
#define mpID_SET_LIMITS mpID_HELP_MOUSE+1
=======
>>>>>>> origin/master
class dlgLimInputbase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText9;
		wxTextCtrl* m_X_min;
		wxTextCtrl* m_X_max;
		wxStaticText* m_staticText10;
		wxTextCtrl* m_Y_min;
		wxTextCtrl* m_Y_max;
		wxStdDialogButtonSizer* m_sdbSizer3;
		wxButton* m_sdbSizer3OK;
		wxButton* m_sdbSizer3Cancel;

	public:

		dlgLimInputbase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Input limits"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 304,170 ), long style = wxDEFAULT_DIALOG_STYLE );

		~dlgLimInputbase();

};

