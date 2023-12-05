///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "mycontrolls.hpp"

#include "frame.h"

///////////////////////////////////////////////////////////////////////////

elMFrame::elMFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, ID_MENUADDTOOL, wxString( wxT("Add a Tool") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	
	wxMenuItem* m_menuItem10;
	m_menuItem10 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Run...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem10 );
	
	m_menu11 = new wxMenu();
	m_menu1->Append( -1, wxT("Recent scripts"), m_menu11 );
	
	wxMenuItem* m_separator1;
	m_separator1 = m_menu1->AppendSeparator();
	
	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Configuration") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem8 );
	
	wxMenuItem* m_separator3;
	m_separator3 = m_menu1->AppendSeparator();
	
	wxMenuItem* m_menuItem13;
	m_menuItem13 = new wxMenuItem( m_menu1, wxID_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem13 );
	
	m_menubar1->Append( m_menu1, wxT("Main") ); 
	
	m_menu3 = new wxMenu();
	wxMenuItem* m_menuItem17;
	m_menuItem17 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Manual") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem17 );
	
	wxMenuItem* m_menuItem18;
	m_menuItem18 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("Rexx Manual") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem18 );
	
	wxMenuItem* m_separator4;
	m_separator4 = m_menu3->AppendSeparator();
	
	wxMenuItem* m_menuItem19;
	m_menuItem19 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem19 );
	
	m_menubar1->Append( m_menu3, wxT("Help") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	m_toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	m_toolBar1->Realize();
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_listCtrl1 = new myGoodListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VRULES );
	bSizer7->Add( m_listCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("Terminate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button1->Enable( false );
	
	//bSizer8->Add( m_button1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	bSizer8->Add( m_button1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_checkBox1 = new wxCheckBox( this, wxID_ANY, wxT("sure"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_checkBox1, 0, wxALL|wxEXPAND, 5 );
	
	bSizer7->Add( bSizer8, 0, wxEXPAND, 5 );
	
	bSizer6->Add( bSizer7, 1, wxEXPAND, 5 );
	
	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH|wxTE_RICH2 );
	bSizer6->Add( m_textCtrl1, 2, wxALL|wxEXPAND, 5 );
	
	bSizer5->Add( bSizer6, 2, wxEXPAND, 5 );
	
	bSizer71->Add( bSizer5, 1, wxEXPAND, 5 );
	
	bSizer4->Add( bSizer71, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button2 = new wxButton( this, wxID_ANY, wxT("Send!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer61->Add( m_button2, 0, wxALL, 5 );
	
	m_textCtrl2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer61->Add( m_textCtrl2, 1, wxALL, 5 );
	
	bSizer4->Add( bSizer61, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( elMFrame::onClose ) );
	this->Connect( m_menuItem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( elMFrame::onNewTool ) );
	this->Connect( m_menuItem10->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( elMFrame::onNewCommand ) );
	this->Connect( m_menuItem8->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( elMFrame::onConfig ) );
	this->Connect( m_menuItem13->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( elMFrame::OnQuit ) );
	m_listCtrl1->Connect( wxEVT_SIZE, wxSizeEventHandler( elMFrame::onListSized ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( elMFrame::evhTerminateThread ), NULL, this );
	m_checkBox1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( elMFrame::onSureTerminate ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( elMFrame::evhSendMsg ), NULL, this );
	m_textCtrl2->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( elMFrame::evhSendMsg ), NULL, this );
}

elMFrame::~elMFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( elMFrame::onClose ) );
	this->Disconnect( ID_MENUADDTOOL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( elMFrame::onNewTool ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( elMFrame::onNewCommand ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( elMFrame::onConfig ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( elMFrame::OnQuit ) );
	m_listCtrl1->Disconnect( wxEVT_SIZE, wxSizeEventHandler( elMFrame::onListSized ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( elMFrame::evhTerminateThread ), NULL, this );
	m_checkBox1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( elMFrame::onSureTerminate ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( elMFrame::evhSendMsg ), NULL, this );
	m_textCtrl2->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( elMFrame::evhSendMsg ), NULL, this );
	
}

MyDialog1::MyDialog1( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("File Name") ), wxVERTICAL );
	
	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_textCtrl1, 0, wxALL, 5 );
	
	m_button2 = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_button2, 0, wxALL, 5 );
	
	bSizer3->Add( sbSizer2, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Icon") ), wxVERTICAL );
	
	m_bitmap2 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer3->Add( m_bitmap2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer3->Add( m_button1, 0, wxALL, 5 );
	
	bSizer3->Add( sbSizer3, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	bSizer2->Add( m_sdbSizer1, 0, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyDialog1::onBrowseFile ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyDialog1::onAddIcon ), NULL, this );
}

MyDialog1::~MyDialog1()
{
	// Disconnect Events
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyDialog1::onBrowseFile ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyDialog1::onAddIcon ), NULL, this );
	
}

MyDialog111_unsused::MyDialog111_unsused( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel1 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_gPaths = new wxGrid( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_gPaths->CreateGrid( 0, 2 );
	m_gPaths->EnableEditing( false );
	m_gPaths->EnableGridLines( true );
	m_gPaths->EnableDragGridSize( false );
	m_gPaths->SetMargins( 0, 0 );
	
	// Columns
	m_gPaths->EnableDragColMove( false );
	m_gPaths->EnableDragColSize( true );
	m_gPaths->SetColLabelSize( 0 );
	m_gPaths->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gPaths->EnableDragRowSize( false );
	m_gPaths->SetRowLabelSize( 0 );
	m_gPaths->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gPaths->SetDefaultCellFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_gPaths->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer2->Add( m_gPaths, 3, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Discrption") ), wxVERTICAL );
	
	m_staticTextPaths = new wxStaticText( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	m_staticTextPaths->Wrap( -1 );
	sbSizer4->Add( m_staticTextPaths, 1, wxALL|wxEXPAND, 5 );
	
	bSizer2->Add( sbSizer4, 1, wxEXPAND, 5 );
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_notebook1->AddPage( m_panel1, wxT("Paths"), true );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_listCtrl211 = new wxListCtrl( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer5->Add( m_listCtrl211, 3, wxALL|wxEXPAND, 5 );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer5 );
	m_panel2->Layout();
	bSizer5->Fit( m_panel2 );
	m_notebook1->AddPage( m_panel2, wxT("Handlers"), false );
	m_panel3 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_gInternals = new wxGrid( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_gInternals->CreateGrid( 0, 2 );
	m_gInternals->EnableEditing( true );
	m_gInternals->EnableGridLines( true );
	m_gInternals->EnableDragGridSize( false );
	m_gInternals->SetMargins( 0, 0 );
	
	// Columns
	m_gInternals->EnableDragColMove( false );
	m_gInternals->EnableDragColSize( true );
	m_gInternals->SetColLabelSize( 0 );
	m_gInternals->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gInternals->EnableDragRowSize( false );
	m_gInternals->SetRowLabelSize( 0 );
	m_gInternals->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gInternals->SetDefaultCellFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_gInternals->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer4->Add( m_gInternals, 3, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Discription") ), wxVERTICAL );
	
	m_staticTextInternal = new wxStaticText( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	m_staticTextInternal->Wrap( -1 );
	sbSizer3->Add( m_staticTextInternal, 1, wxALL|wxEXPAND, 5 );
	
	bSizer4->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	m_panel3->SetSizer( bSizer4 );
	m_panel3->Layout();
	bSizer4->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, wxT("Internals"), false );
	
	bSizer1->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );
	
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();
	bSizer1->Add( m_sdbSizer2, 0, wxEXPAND|wxBOTTOM|wxRIGHT, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_gPaths->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( MyDialog111_unsused::showHelp ), NULL, this );
	m_gInternals->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( MyDialog111_unsused::showHelp ), NULL, this );
}

MyDialog111_unsused::~MyDialog111_unsused()
{
	// Disconnect Events
	m_gPaths->Disconnect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( MyDialog111_unsused::showHelp ), NULL, this );
	m_gInternals->Disconnect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( MyDialog111_unsused::showHelp ), NULL, this );
	
}
