///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "handlerpanel.h"

///////////////////////////////////////////////////////////////////////////

HandlerLibPanelProto::HandlerLibPanelProto( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	sboxLibName = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("label") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Devices") ), wxVERTICAL );
	
	wxArrayString checkListDevsChoices;
	checkListDevs = new wxCheckListBox( this, idCHKLISTDEV, wxDefaultPosition, wxDefaultSize, checkListDevsChoices, wxLB_NEEDED_SB );
	sbSizer8->Add( checkListDevs, 1, wxEXPAND, 5 );
	
	bSizer15->Add( sbSizer8, 1, wxEXPAND|wxRIGHT, 5 );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Loggers") ), wxVERTICAL );
	
	wxArrayString checkListLogsChoices;
	checkListLogs = new wxCheckListBox( this, idCHKLISTLOG, wxDefaultPosition, wxDefaultSize, checkListLogsChoices, wxLB_NEEDED_SB );
	sbSizer10->Add( checkListLogs, 1, wxEXPAND, 5 );
	
	bSizer15->Add( sbSizer10, 1, wxEXPAND|wxRIGHT, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("MD5:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer18->Add( m_staticText4, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	textMD5 = new wxTextCtrl( this, wxID_ANY, wxT("00000000000000000000000000000000"), wxDefaultPosition, wxSize( 205,-1 ), wxTE_READONLY );
	bSizer18->Add( textMD5, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer17->Add( bSizer18, 0, wxEXPAND, 5 );
	
	textDescr = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer17->Add( textDescr, 1, wxEXPAND, 5 );
	
	bSizer15->Add( bSizer17, 0, wxEXPAND, 5 );
	
	bSizer16->Add( bSizer15, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	textPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer19->Add( textPath, 1, wxEXPAND|wxLEFT, 5 );
	
//	m_checkBox2 = new wxCheckBox( this, wxID_ANY, wxT("Enabled"), wxDefaultPosition, wxDefaultSize, 0 );
//	bSizer19->Add( m_checkBox2, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );
	
	bSizer16->Add( bSizer19, 0, wxEXPAND|wxTOP, 5 );
	
	sboxLibName->Add( bSizer16, 1, wxEXPAND, 5 );
	
	this->SetSizer( sboxLibName );
	this->Layout();
}

HandlerLibPanelProto::~HandlerLibPanelProto()
{
}
