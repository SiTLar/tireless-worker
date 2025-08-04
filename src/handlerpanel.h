///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __handlerpanel__
#define __handlerpanel__

#include <wx/string.h>
#include <wx/checklst.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////

#define idCHKLISTDEV 1000
#define idCHKLISTLOG 1001

///////////////////////////////////////////////////////////////////////////////
/// Class HandlerLibPanelProto
///////////////////////////////////////////////////////////////////////////////
class HandlerLibPanelProto : public wxPanel 
{
	private:
	
	protected:
		wxStaticBoxSizer* sboxLibName;
		wxCheckListBox* checkListDevs;
		wxCheckListBox* checkListLogs;
		wxStaticText* m_staticText4;
		wxTextCtrl* textMD5;
		wxTextCtrl* textDescr;
		wxTextCtrl* textPath;
	//	wxCheckBox* m_checkBox2;
	
	public:
		
		HandlerLibPanelProto( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 450,150 ), long style = wxTAB_TRAVERSAL );
		~HandlerLibPanelProto();
	
};

#endif //__handlerpanel__
