// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <mathplot.h>

#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/print.h>
#include <wx/filename.h>
#include <wx/thread.h>
#include <wx/ipc.h>
#include <wx/init.h>
#include <wx/regex.h>
#include <map>
#include <math.h>
#include <wx/socket.h>
//#include "logger.h"
#include "logplotapi.h"
#include "logplotpr.hpp"
#include "mathplot.h"
// #include <time.h>
extern "C"{
#include <stdio.h>
}
// Memory leak debugging
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// derived classes

class MySIN;
class MyCOSinverse;
class MyLissajoux;
class PlotFrame;
class PlotThread;

std::map<GraphHead,GraphHandler> mapGH;
// main program


// PlotFrame

enum {
	ID_QUIT  = 108,
	ID_ABOUT,
	ID_PRINT,
	ID_PRINT_PREVIEW,
	ID_ALIGN_X_AXIS,
	ID_ALIGN_Y_AXIS,
	ID_TOGGLE_GRID,
	ID_TOGGLE_SCROLLBARS,
	ID_TOGGLE_INFO,
	ID_SAVE_SCREENSHOT,
	ID_TOGGLE_LISSAJOUX,
	ID_TOGGLE_SINE,
	ID_TOGGLE_COSINE,
	ID_BLACK_THEME
};

//IMPLEMENT_DYNAMIC_CLASS( PlotFrame, wxFrame )

BEGIN_EVENT_TABLE(PlotFrame,wxFrame)
	EVT_MENU(ID_ABOUT, PlotFrame::OnAbout)
	EVT_MENU(ID_QUIT,  PlotFrame::OnQuit)
	EVT_MENU(ID_PRINT_PREVIEW, PlotFrame::OnPrintPreview)
	EVT_MENU(ID_PRINT, PlotFrame::OnPrint)
	    EVT_MENU( mpID_FIT,       PlotFrame::OnFit)
	EVT_MENU(ID_ALIGN_X_AXIS, PlotFrame::OnAlignXAxis)
	EVT_MENU(ID_ALIGN_Y_AXIS, PlotFrame::OnAlignYAxis)
	EVT_MENU(ID_TOGGLE_GRID, PlotFrame::OnToggleGrid)
	EVT_MENU(ID_TOGGLE_SCROLLBARS, PlotFrame::OnToggleScrollbars)
	EVT_MENU(ID_TOGGLE_INFO, PlotFrame::OnToggleInfoLayer)
	EVT_MENU(ID_SAVE_SCREENSHOT, PlotFrame::OnSaveScreenshot)
	EVT_MENU(ID_BLACK_THEME, PlotFrame::OnBlackTheme)
	EVT_CLOSE(PlotFrame::OnClose)
END_EVENT_TABLE()

	//PlotFrame::~PlotFrame(){owner->frameKilled(owner);};
PlotFrame::PlotFrame(SharedP<PlotFrame*>&_myTail)
	: wxFrame( (wxFrame *)NULL, -1, wxT("Tireless Worker diagram"), wxDefaultPosition, wxSize(500, 500)), myTail(_myTail), m_bFit(true){


		wxMenu *file_menu = new wxMenu();
		wxMenu *view_menu = new wxMenu();

		file_menu->Append( ID_PRINT_PREVIEW, wxT("Print Pre&view..."));
		file_menu->Append( ID_PRINT, wxT("&Print..."));
		file_menu->Append( ID_SAVE_SCREENSHOT, wxT("Save screenshot"));
		file_menu->AppendSeparator();
		file_menu->Append( ID_ABOUT, wxT("&About..."));
		file_menu->Append( ID_QUIT, wxT("E&xit\tAlt-X"));

		view_menu->Append( mpID_FIT, wxT("&Fit bounding box"), wxT("Set plot view to show all items"));
		view_menu->Append( mpID_ZOOM_IN, wxT("Zoom in"), wxT("Zoom in plot view."));
		view_menu->Append( mpID_ZOOM_OUT, wxT("Zoom out"), wxT("Zoom out plot view."));
		view_menu->AppendSeparator();
		view_menu->Append( ID_ALIGN_X_AXIS, wxT("Switch &X axis align"));
		view_menu->Append( ID_ALIGN_Y_AXIS, wxT("Switch &Y axis align"));
		view_menu->Append( ID_TOGGLE_GRID, wxT("Toggle grid/ticks"));
		view_menu->AppendCheckItem( ID_TOGGLE_SCROLLBARS, wxT("Show Scroll Bars"));
		view_menu->AppendCheckItem( ID_TOGGLE_INFO, wxT("Show overlay info box"));
		view_menu->AppendCheckItem( ID_BLACK_THEME, wxT("Switch to black background theme"));

		wxMenuBar *menu_bar = new wxMenuBar();
		menu_bar->Append(file_menu, wxT("&File"));
		menu_bar->Append(view_menu, wxT("&View"));

		SetMenuBar( menu_bar );
		CreateStatusBar(1);


		// Create two vectors for x,y and fill them with data

		wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		m_plot = new mpWindow( this, -1, wxPoint(0,0), wxSize(100,100), wxSUNKEN_BORDER );
		mpScaleX* xaxis = new mpScaleX(wxT("X"), mpALIGN_BOTTOM, true, mpX_NORMAL);
		mpScaleY* yaxis = new mpScaleY(wxT("Y"), mpALIGN_LEFT, true);
		xaxis->SetFont(graphFont);
		yaxis->SetFont(graphFont);
		xaxis->SetDrawOutsideMargins(false);
		yaxis->SetDrawOutsideMargins(false);
		// Fake axes formatting to test arbitrary format string
		// xaxis->SetLabelFormat(wxT("%.2f €"));
		// yaxis->SetLabelFormat(wxT("%p"));
		m_plot->SetMargins(30, 30, 50, 100);
		//     m_plot->SetMargins(50, 50, 200, 150);
		m_plot->AddLayer(     xaxis );
		m_plot->AddLayer(     yaxis );
		/*
		   m_plot->AddLayer(     new MySIN( 10.0, 220.0 ) );
		   m_plot->AddLayer(     new MyCOSinverse( 10.0, 100.0 ) );
		   m_plot->AddLayer( l = new MyLissajoux( 125.0 ) );
		   */

		//for(int idx = 0; idx < 1000; idx++) fxys->addData((idx-500)*1e-2, exp((idx-500)*1e-2));	
		//Connect(PlotFrame::REDRAW, EVT_CUSTOM, wxCommandEventHandler( PlotFrame::redraw));
		//Connect(PlotFrame::ADDPLOT, EVT_CUSTOM, wxCommandEventHandler( PlotFrame::addPlot));
		//m_plot->AddLayer(     vectorLayer );
		wxBrush hatch(wxColour(200,200,200), wxSOLID);
		//m_plot->AddLayer( nfo = new mpInfoLayer(wxRect(80,20,40,40), &hatch));
		m_plot->AddLayer( nfo = new mpInfoCoords(wxRect(80,20,10,10), wxTRANSPARENT_BRUSH)); //&hatch));
		nfo->SetVisible(false);
		wxBrush hatch2(wxColour(163,208,212), wxSOLID);
		mpInfoLegend* leg;
		m_plot->AddLayer( leg = new mpInfoLegend(wxRect(200,20,40,40), wxTRANSPARENT_BRUSH)); //&hatch2));
		leg->SetVisible(true);

		// m_plot->EnableCoordTooltip(true);
		// set a nice pen for the lissajoux


		wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

		topsizer->Add( m_plot, 1, wxEXPAND );

		SetAutoLayout( TRUE );
		SetSizer( topsizer );
		axesPos[0] = 0;
		axesPos[1] = 0;
		ticks = true;

		ehFitHook = new wxEvtHandler();
		ehFitHook->Connect(mpID_FIT,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PlotFrame::FitOn), NULL, this);
		ehFitHook->Connect(mpID_ZOOM_IN,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PlotFrame::FitOff), NULL, this);
		ehFitHook->Connect(mpID_CENTER,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PlotFrame::FitOff), NULL, this);
		ehFitHook->Connect(mpID_ZOOM_OUT,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PlotFrame::FitOff), NULL, this);
		ehFitHook->Connect(mpID_LOCKASPECT,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PlotFrame::FitOff), NULL, this); 
		ehFitHook->Connect(wxEVT_RIGHT_DOWN, wxCommandEventHandler(PlotFrame::FitOff), NULL, this);
		ehFitHook->Connect(wxEVT_LEFT_DOWN, wxCommandEventHandler(PlotFrame::FitOff), NULL, this);

	
		//PushEventHandler(ehFitHook);
		m_plot->PushEventHandler(ehFitHook);

		m_plot->EnableDoubleBuffer(true);
		m_plot->SetMPScrollbars(false);
		m_plot->Fit();

		//double* bbx = new double[4];
		//m_plot->GetBoundingBox(bbx);
		//wxLogMessage(wxT("bounding box: X = %f, %f; Y = %f, %f"), bbx[0], bbx[1], bbx[2], bbx[3]);
		//delete [] bbx;
	}
void PlotFrame::redraw(){
	m_plot->UpdateAll();
	if(m_bFit)m_plot->Fit();
	Refresh();


}

void PlotFrame::addPlot(FXYSeq *fxys){
	//wxPen mypen(*wxBLUE, 5, wxTRANSPARENT);
	//FXYSeq *fxys = new FXYSeq(wxT("LogPlot: it works!"));
	//FXYSeq *fxys = new FXYSeq();
	m_plot->AddLayer( fxys );
}

void PlotFrame::OnQuit( wxCommandEvent &WXUNUSED(event) ) {
	//wxMutexLocker ml(mtx);
	Close( TRUE );
}


void PlotFrame::OnFit( wxCommandEvent &WXUNUSED(event) )
{
	m_bFit = true;
	m_plot->Fit();
}
void PlotFrame::OnAbout( wxCommandEvent &WXUNUSED(event) )
{
	wxMessageBox( wxT("Tireless Worker graphical logging utility.\nBased on wxWidgets mathplot sample\n(c) 2003 David Schalig\n(c) 2007-2009 Davide Rondini and wxMathPlot team"));
}

void PlotFrame::OnAlignXAxis( wxCommandEvent &WXUNUSED(event) )
{
	axesPos[0] = (int) (axesPos[0]+1)%5;
	wxString temp;
	temp.sprintf(wxT("axesPos = %d\n"), axesPos);
	mpScaleX* xaxis = ((mpScaleX*)(m_plot->GetLayer(0)));
	mpScaleY* yaxis = ((mpScaleY*)(m_plot->GetLayer(1)));
	if (axesPos[0] == 0) {
		xaxis->SetAlign(mpALIGN_BORDER_BOTTOM);
		m_plot->SetMarginTop(0);
		m_plot->SetMarginBottom(0);
	}
	if (axesPos[0] == 1) {
		//((mpScaleX*)(m_plot->GetLayer(0)))->SetAlign(mpALIGN_BOTTOM);
		xaxis->SetAlign(mpALIGN_BOTTOM);
		m_plot->SetMarginTop(0);
		m_plot->SetMarginBottom(50);
	}
	if (axesPos[0] == 2) {
		//((mpScaleX*)(m_plot->GetLayer(0)))->SetAlign(mpALIGN_CENTER);
		xaxis->SetAlign(mpALIGN_CENTER);
		m_plot->SetMarginTop(0);
		m_plot->SetMarginBottom(0);
	}
	if (axesPos[0] == 3) {
		//((mpScaleX*)(m_plot->GetLayer(0)))->SetAlign(mpALIGN_TOP);
		xaxis->SetAlign(mpALIGN_TOP);
		m_plot->SetMarginTop(50);
		m_plot->SetMarginBottom(0);
	}
	if (axesPos[0] == 4) {
		((mpScaleX*)(m_plot->GetLayer(0)))->SetAlign(mpALIGN_BORDER_TOP);
		xaxis->SetAlign(mpALIGN_BORDER_TOP);
		m_plot->SetMarginTop(0);
		m_plot->SetMarginBottom(0);
	}
	m_plot->UpdateAll();
}

void PlotFrame::OnAlignYAxis( wxCommandEvent &WXUNUSED(event) )
{
	axesPos[1] = (int) (axesPos[1]+1)%5;
	wxString temp;
	temp.sprintf(wxT("axesPos = %d\n"), axesPos);
	mpScaleX* xaxis = ((mpScaleX*)(m_plot->GetLayer(0)));
	mpScaleY* yaxis = ((mpScaleY*)(m_plot->GetLayer(1)));
	if (axesPos[1] == 0) {
		//((mpScaleY*)(m_plot->GetLayer(1)))->SetAlign(mpALIGN_BORDER_LEFT);
		yaxis->SetAlign(mpALIGN_BORDER_LEFT);
		m_plot->SetMarginLeft(0);
		m_plot->SetMarginRight(0);
	}
	if (axesPos[1] == 1) {
		//((mpScaleY*)(m_plot->GetLayer(1)))->SetAlign(mpALIGN_LEFT);
		yaxis->SetAlign(mpALIGN_LEFT);
		m_plot->SetMarginLeft(70);
		m_plot->SetMarginRight(0);
	}
	if (axesPos[1] == 2) {
		//((mpScaleY*)(m_plot->GetLayer(1)))->SetAlign(mpALIGN_CENTER);
		yaxis->SetAlign(mpALIGN_CENTER);
		m_plot->SetMarginLeft(0);
		m_plot->SetMarginRight(0);
	}
	if (axesPos[1] == 3) {
		//((mpScaleY*)(m_plot->GetLayer(1)))->SetAlign(mpALIGN_RIGHT);
		yaxis->SetAlign(mpALIGN_RIGHT);
		m_plot->SetMarginLeft(0);
		m_plot->SetMarginRight(70);
	}
	if (axesPos[1] == 4) {
		//((mpScaleY*)(m_plot->GetLayer(1)))->SetAlign(mpALIGN_BORDER_RIGHT);
		yaxis->SetAlign(mpALIGN_BORDER_RIGHT);
		m_plot->SetMarginLeft(0);
		m_plot->SetMarginRight(0);
	}
	m_plot->UpdateAll();
}

void PlotFrame::OnToggleGrid( wxCommandEvent &WXUNUSED(event) )
{
	ticks = !ticks;
	((mpScaleX*)(m_plot->GetLayer(0)))->SetTicks(ticks);
	((mpScaleY*)(m_plot->GetLayer(1)))->SetTicks(ticks);
	m_plot->UpdateAll();
}

void PlotFrame::OnToggleScrollbars(wxCommandEvent& event)
{
	if (event.IsChecked())
		m_plot->SetMPScrollbars(true);
	else
		m_plot->SetMPScrollbars(false);
	event.Skip();
}

void PlotFrame::OnToggleInfoLayer(wxCommandEvent& event)
{
	if (event.IsChecked())
		nfo->SetVisible(true);
	else
		nfo->SetVisible(false);
	m_plot->UpdateAll();
	event.Skip();
}

void PlotFrame::OnBlackTheme(wxCommandEvent& )
{
	//wxColor black(0,0,0);
	//wxColor white(255,255,255);
	wxColour grey(96, 96, 96);
	/*wxBrush* brush = new wxBrush(*wxTRANSPARENT_BRUSH)*/;
	m_plot->SetColourTheme(*wxBLACK, *wxWHITE, grey);
	m_plot->UpdateAll();
}

void PlotFrame::OnPrintPreview( wxCommandEvent &WXUNUSED(event))
{
	// Pass two printout objects: for preview, and possible printing.
	mpPrintout *plotPrint = new mpPrintout(m_plot);
	mpPrintout *plotPrintPreview = new mpPrintout(m_plot);
	wxPrintPreview *preview = new wxPrintPreview(plotPrintPreview, plotPrint);
	wxPreviewFrame *frame = new wxPreviewFrame(preview, this, wxT("Print Plot"), wxPoint(100, 100), wxSize(600, 650));
	frame->Centre(wxBOTH);
	frame->Initialize();
	frame->Show(true);
}

void PlotFrame::OnPrint( wxCommandEvent& WXUNUSED(event) )
{
	wxPrinter printer;
	mpPrintout printout(m_plot, wxT("Plot print"));
	printer.Print(this, &printout, true);
}

void PlotFrame::OnSaveScreenshot(wxCommandEvent& event)
{
	wxFileDialog fileDialog(this, _("Save a screenshot"), wxT(""), wxT(""), wxT("BMP image (*.bmp) | *.bmp|JPEG image (*.jpg) | *.jpeg;*.jpg|PNG image (*.png) | *.png"), wxFD_SAVE);
	if(fileDialog.ShowModal() == wxID_OK) {
		wxFileName namePath(fileDialog.GetPath());
		int fileType = wxBITMAP_TYPE_BMP;
		if( namePath.GetExt().CmpNoCase(wxT("jpeg")) == 0 ) fileType = wxBITMAP_TYPE_JPEG;
		if( namePath.GetExt().CmpNoCase(wxT("jpg")) == 0 )  fileType = wxBITMAP_TYPE_JPEG;
		if( namePath.GetExt().CmpNoCase(wxT("png")) == 0 )  fileType = wxBITMAP_TYPE_PNG;
		wxSize imgSize(500,500);
		m_plot->SaveScreenshot(fileDialog.GetPath(), fileType, imgSize, false);
	}
	event.Skip();
}
void PlotFrame::FitOn(wxCommandEvent& evt){
	m_bFit = true;
	evt.Skip(true);
}
void PlotFrame::FitOff(wxCommandEvent& evt){
	m_bFit = false;
	evt.Skip(true);
}


void LPApp::OnSockInput(wxSocketEvent& evt ) {

	if (evt.GetSocketEvent() != wxSOCKET_INPUT ) return;
	//wxSocketBase *pSock = evt.GetSocket();
	std::map<GraphHead,GraphHandler>::iterator it;
	UnionMessages data;
	wxSocketError err = wxSOCKET_NOERROR ;
	wxIPV4address address;
	pSockServer->RecvFrom( address, &data, sizeof(data));
	if (pSockServer->Error())
		err = pSockServer->LastError();
	switch (data.cCommand){
		case 'C':

			if (pSockServer->LastCount() == sizeof(LPCreatePlot)){
				it = mapGH.find(GraphHead(data.C.ulTID,data.C.ulLID));
				if (it == mapGH.end()) 
					it = mapGH.insert(std::make_pair(GraphHead(data.C.ulTID,data.C.ulLID),GraphHandler())).first;

			}

			break;
		case 'A':

			if (pSockServer->LastCount() == sizeof(LPAttach)){
				it = mapGH.find(GraphHead(data.A.ulTIDatt,data.A.ulLIDatt));
				if (it == mapGH.end()) 
					it = mapGH.insert(std::make_pair(GraphHead(data.A.ulTIDatt,data.A.ulLIDatt),GraphHandler())).first;
				std::map<GraphHead,GraphHandler>::iterator it_me = mapGH.find(GraphHead(data.A.ulTID,data.A.ulLID));
				if (it_me == mapGH.end()) 
					it_me = mapGH.insert(std::make_pair(GraphHead(data.A.ulTID,data.A.ulLID),GraphHandler(it->second, 1))).first;
				else it_me->second.attach(it->second);

			}

			break;
		case 'W':
			if (pSockServer->LastCount() == sizeof(LPAddPoint)){
				it = mapGH.find(GraphHead(data.W.ulTID,data.W.ulLID));
				if (it == mapGH.end()) 
					it = mapGH.insert(std::make_pair(GraphHead(data.W.ulTID,data.W.ulLID),GraphHandler())).first;
				it->second.addData(data.W.x,data.W.y);
			}

			break;

		case 'T':
			if (pSockServer->LastCount() == sizeof(LPString)){
				it = mapGH.find(GraphHead(data.S.ulTID,data.S.ulLID));
				if (it == mapGH.end()) 
					it = mapGH.insert(std::make_pair(GraphHead(data.S.ulTID,data.S.ulLID),GraphHandler())).first;
				it->second.setTitle(data.S.length, data.S.str);
			}
			break;
		case 'L':
			if (pSockServer->LastCount() == sizeof(LPString)){
				it = mapGH.find(GraphHead(data.S.ulTID,data.S.ulLID));
				if (it == mapGH.end()) 
					it = mapGH.insert(std::make_pair(GraphHead(data.S.ulTID,data.S.ulLID),GraphHandler())).first;
				it->second.setLabel(data.S.length, data.S.str);
			}
			break;

		case 'O':
			if (pSockServer->LastCount() == sizeof(LPString)){
				it = mapGH.find(GraphHead(data.S.ulTID,data.S.ulLID));
				if (it == mapGH.end()) 
					it = mapGH.insert(std::make_pair(GraphHead(data.S.ulTID,data.S.ulLID),GraphHandler())).first;
				it->second.setColour(data.S.length, data.S.str);
			}

			break;
		default:
			break;
	}
}
PlotFrame::~PlotFrame(){
	//	wxMessageBox(wxT("TADA"));
};
void PlotFrame::OnClose(wxCloseEvent &WXUNUSED(event) ) {
	*myTail = NULL;
	Destroy();
}

GraphHandler::GraphHandler(): ppFrame (new PlotFrame*) , ppFxys (new FXYSeq*) {
	*ppFrame = NULL;
	*ppFxys = NULL;
	colour = *wxBLUE;
	init();
};
GraphHandler::GraphHandler(GraphHandler &inp, int){
	attach(inp);
}
void GraphHandler::init(){
	if (!*ppFrame) *ppFrame = new PlotFrame(ppFrame);
	if (!*ppFxys) {
		*ppFxys = new FXYSeq(ppFxys);
		(*ppFxys)->SetPen( wxPen(colour, 5, wxSOLID));
		(*ppFxys)->SetBrush( wxBrush(colour, wxTRANSPARENT));
		(*ppFxys)->SetName(sLabel );
		(*ppFrame)->Show(true);
		(*ppFrame)->addPlot(*ppFxys);
	};
	if(!m_sTitle.IsEmpty())
		(*ppFrame)->SetTitle(m_sTitle);


}
//GraphHandler::~GraphHandler(){ --tail;}
void GraphHandler::addData(double x, double y) {

	init();
	(*ppFxys)->addData(x,y);
	(*ppFrame)->redraw();
};
void GraphHandler::attach(GraphHandler&inp) {
	ppFrame = inp.ppFrame;
	//if (!*ppFxys) *ppFxys = new FXYSeq(ppFxys);
	ppFxys = new FXYSeq*;
	*ppFxys = new FXYSeq(ppFxys);
	(*ppFxys)->SetPen( wxPen(*wxGREEN, 5, wxSOLID));
	(*ppFxys)->SetBrush( wxBrush(*wxGREEN, wxTRANSPARENT));
	(*ppFxys)->SetName(sLabel );
	(*ppFrame)->Show(true);
	(*ppFrame)->addPlot(*ppFxys);
}
void GraphHandler::setTitle(int len, char * str) {

	m_sTitle = wxString::FromUTF8(str, len);
	init();

};
void GraphHandler::setLabel(int len, char * str) {

	init();
	sLabel = wxString::FromUTF8(str, len);

	(*ppFxys)->SetName(sLabel );
	(*ppFrame)->redraw();
};
void GraphHandler::setColour(int len, char * str) {

	init();
	colour.Set(wxString::FromUTF8(str, len));
	(*ppFxys)->SetPen( wxPen(colour, 5, wxSOLID));
	(*ppFxys)->SetBrush( wxBrush(colour, wxTRANSPARENT));
	(*ppFrame)->redraw();
};

bool LPApp::OnInit(){
	//wxFrame * wxframe = new wxFrame((wxFrame *)NULL, -1, wxT("Tireless Worker diagram"), wxDefaultPosition, wxSize(500, 500));
	//wxframe->Show(true);
	SetExitOnFrameDelete(false);
	//return ddeServer.Create(wxT("/tmp/tw_logplot.sock"));
	Connect(  wxEVT_SOCKET, wxSocketEventHandler(LPApp::OnSockInput));
	//wxframe->Connect(  wxEVT_SOCKET, wxSocketEventHandler(LPApp::OnSockInput));
	wxIPV4address addrServer;
	addrServer.Hostname(wxT("localhost"));
	addrServer.Service(48000);
	pSockServer = new wxDatagramSocket(addrServer, wxSOCKET_NOWAIT);
	//pSockServer = new wxDatagramSocket(addrServer, wxSOCKET_NONE);
	pSockServer->SetEventHandler(*this, -1);
	//pSockServer->SetEventHandler(*wxframe, -1);
	//SetTopWindow(wxframe);
	pSockServer->SetNotify(wxSOCKET_INPUT_FLAG);
	pSockServer->Notify(true);
	return pSockServer->IsOk();
}

int LPApp::OnExit(){
	return 0;
}

IMPLEMENT_APP(LPApp)
