///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __elframe__
#define __elframe__

class myGoodListCtrl;

#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/frame.h>
#include <wx/statbox.h>
#include <wx/statbmp.h>
#include <wx/dialog.h>
#include <wx/grid.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/notebook.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class elMFrame
///////////////////////////////////////////////////////////////////////////////
class elMFrame : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			ID_MENUADDTOOL = 1000,
		};
		
		wxStatusBar* m_statusBar1;
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu11;
		wxMenu* m_menu3;
		wxToolBar* m_toolBar1;
		myGoodListCtrl* m_listCtrl1;
		wxButton* m_button1;
		wxCheckBox* m_checkBox1;
		wxTextCtrl* m_textCtrl1;
		wxButton* m_button2;
		wxTextCtrl* m_textCtrl2;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void onNewTool( wxCommandEvent& event ) { event.Skip(); }
		virtual void onNewCommand( wxCommandEvent& event ) { event.Skip(); }
		virtual void onConfig( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void onListSized( wxSizeEvent& event ) { event.Skip(); }
		virtual void evhTerminateThread( wxCommandEvent& event ) { event.Skip(); }
		virtual void onSureTerminate( wxCommandEvent& event ) { event.Skip(); }
		virtual void evhSendMsg( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		elMFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Tireless Worker"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 632,299 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~elMFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyDialog1
///////////////////////////////////////////////////////////////////////////////
class MyDialog1 : public wxDialog 
{
	private:
	
	protected:
		wxTextCtrl* m_textCtrl1;
		wxButton* m_button2;
		wxStaticBitmap* m_bitmap2;
		wxButton* m_button1;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onBrowseFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void onAddIcon( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MyDialog1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Create a New Tool"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~MyDialog1();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyDialog111_unsused
///////////////////////////////////////////////////////////////////////////////
class MyDialog111_unsused : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebook1;
		wxPanel* m_panel1;
		wxGrid* m_gPaths;
		wxStaticText* m_staticTextPaths;
		wxPanel* m_panel2;
		wxListCtrl* m_listCtrl211;
		
		wxPanel* m_panel3;
		wxGrid* m_gInternals;
		wxStaticText* m_staticTextInternal;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void showHelp( wxGridEvent& event ) { event.Skip(); }
		
	
	public:
		
		MyDialog111_unsused( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 349,383 ), long style = wxDEFAULT_DIALOG_STYLE );
		~MyDialog111_unsused();
	
};

#endif //__elframe__
