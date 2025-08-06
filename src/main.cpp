#include <wx/regex.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/dir.h>
#include <wx/socket.h>
#include <wx/xml/xml.h>
#include <wx/app.h>
#include <wx/process.h>
//#include <wx/bmpbndl.h>
#include <locale.h>
#include "thread.h"
#include "devabstruction.hpp"
#include "broken.xpm"
#include <string>
#include "md5.h"
#include "main.h"
/*
#ifndef wxBitmapBundle
#define wxBitmapBundle BB(X) (X)
#endif
*/
enum{	MAX_DYN_MENU_ITEMS = 4, MAX_TOOLS = 10, TIME_QUANT = 500};
extern Vciml vciMailingList;
InternalConfigStuct strIntConf;
unsigned long ulTimeQuant;
//wxLogStderr EL;
MyFrame::MyFrame( wxWindow* parent ) : elMFrame( parent ),oConfig(0)  {
 setlocale(LC_ALL, "C");
	wxExecute("logplot",wxEXEC_ASYNC, &prLogplot);
 // wxLog::SetActiveTarget(&EL);
 wxLog::SetActiveTarget(new wxLogStderr);
	wxInitAllImageHandlers();
	wxSocketBase::Initialize();
	strIntConf.ulMaxMenuTasks = MAX_DYN_MENU_ITEMS;
	strIntConf.ulMaxTools = MAX_TOOLS;
	ulTimeQuant = TIME_QUANT;
	loadCfg();
	Connect(elTHREAD_MSG, wxEVT_USER_FIRST, 
			wxCommandEventHandler( MyFrame::onThreadMsg));
	Connect( elTHREAD_LIFECYCLE , wxEVT_USER_FIRST, 
			wxCommandEventHandler( MyFrame::onThreadLC));
	Connect( NEW_TASK , wxEVT_USER_FIRST,  wxCommandEventHandler( MyFrame::onNewTask));

	Connect( 0, wxEVT_USER_FIRST + 1, //TODO
			wxCommandEventHandler( MyFrame::onExecReq));
	Connect( mID_USEDTASKS, mID_USEDTASKS+strIntConf.ulMaxMenuTasks, wxEVT_COMMAND_MENU_SELECTED, wxMenuEventHandler( MyFrame::onDynTask) );
	Connect( strIntConf.ulIDTools, strIntConf.ulIDTools+strIntConf.ulMaxTools, wxEVT_COMMAND_TOOL_CLICKED, wxMenuEventHandler( MyFrame::onToolClick) );
	Connect( strIntConf.ulIDTools, strIntConf.ulIDTools+strIntConf.ulMaxTools, wxEVT_COMMAND_TOOL_RCLICKED, wxMenuEventHandler( MyFrame::onToolRClick) );
	Connect( ID_EDIT_TOOL, wxEVT_COMMAND_MENU_SELECTED, wxMenuEventHandler( MyFrame::onEditTool) );
	Connect( ID_REMOVE_TOOL, wxEVT_COMMAND_MENU_SELECTED, wxMenuEventHandler( MyFrame::onRemoveTool) );
	counter = 0;
	loadHandlers();
	pocessHandlers(strIntConf.mapHandlersToUse);
}
struct  HGenStuff: public std::unary_function<const std::pair<wxString,int>&, void>{
	std::map<wxString, DevDesc*>& mapDevs;
       	std::map<wxString, LogDesc*>& mapLogs;
	HandlerLibData & hData;
	HGenStuff(std::map<wxString, DevDesc*>&_mapDevs, std::map<wxString, LogDesc*>&_mapLogs,HandlerLibData&_hData):
		mapDevs(_mapDevs), mapLogs(_mapLogs), hData(_hData) {};
	inline void operator()(const std::pair<wxString,TypeFlag>&paInp){
		if(paInp.second.isSet(HandlerLibData::DEVICE)){
			std::map<std::string,DevInterface*>::iterator itDev;
			if((itDev = hData.HLI.mapDevs.find(std::string(paInp.first.mb_str())))!=hData.HLI.mapDevs.end()){
				mapDevs.insert(std::make_pair(paInp.first, new DevDesc(itDev->second, &hData)));
			}
		}

		if(paInp.second.isSet(HandlerLibData::LOGGER)){
			std::map<std::string,Logger *>::iterator itLog;
			if((itLog = hData.HLI.mapLogs.find(std::string(paInp.first.mb_str())))!=hData.HLI.mapLogs.end()){
				mapLogs.insert(std::make_pair(paInp.first, new LogDesc(itLog->second, &hData)));
			}
		}

	}

};
struct  ChkString: public std::unary_function<std::pair<wxString const,std::map<wxString,TypeFlag> >&, void>{
	wxString str;
	ChkString(wxString _str): str(_str){ }
	inline void operator()(std::pair<wxString const,std::map<wxString,TypeFlag> >&inp){
		wxMessageBox(wxT("\"")+str+wxT("\">><<\"")+inp.first+wxT("\""));
	}

};
struct  HandlerActivator: public std::unary_function<HandlerLibData&, void>{
	HandlerBroker *pHB;
	std::map<wxString, DevDesc*>mapDevs;
       	std::map<wxString, LogDesc*>mapLogs;
	std::map<wxString,std::map<wxString,TypeFlag> > &toUse;
	bool &bSuccess;
	HandlerActivator(HandlerBroker *_pHB, std::map<wxString,std::map<wxString,TypeFlag> > &_toUse, bool &_bSuccess):pHB(_pHB), toUse(_toUse), bSuccess(_bSuccess){ };
	inline void operator()(HandlerLibData&inp){
		std::map<wxString,std::map<wxString,TypeFlag> >::iterator it(toUse.find(inp.md5));
		if(it != toUse.end())
			std::for_each(it->second.begin(),it->second.end(),HGenStuff(mapDevs,mapLogs , inp));
	}
	~HandlerActivator(){
		if (!pHB->checkLock(mapDevs))
			bSuccess = wxYES == wxMessageBox(wxT("Some locks seam to be incompatible. Shall we use new device handlers selection?\n"),wxT("Confirm"), wxYES_NO | wxICON_EXCLAMATION );

		if(bSuccess ) pHB->setAvailHandlers(mapDevs, mapLogs);
	};

};


bool MyFrame::pocessHandlers(std::map<wxString, std::map<wxString,TypeFlag> > &mapHandlersToUse){
	std::set<HandlerLibData>::iterator it = strIntConf.setHandlerLibs.begin();
	std::set<HandlerLibData>::iterator itEnd = strIntConf.setHandlerLibs.end();
	bool bSuccess = true;
	HandlerActivator hGen(fGetHBroker(),mapHandlersToUse, bSuccess);
	while(it != itEnd)
		hGen(*const_cast<HandlerLibData*>(&(*it++)));	
	return bSuccess;
}

void MyFrame::loadHandlers(){
	wxDynamicLibrary dllDev;
	wxDir dirHandlers(strIntConf.sHandlersDir);
	wxString sLibName;
	if (!dirHandlers.GetFirst( &sLibName, wxEmptyString, wxDIR_FILES)) return;
	do {
		MD5 md5Op;
		wxString sFullPath = strIntConf.sHandlersDir + wxFileName::GetPathSeparator() +sLibName;
		wxString md5 = wxString(md5Op.digestFile((sFullPath).mb_str(wxConvLibc)),wxConvUTF8 );
		if (strIntConf.setHandlerLibs.find(HandlerLibData(md5))!=strIntConf.setHandlerLibs.end()) continue;
		if(!dllDev.Load( sFullPath )) 
			wxLogError(wxString::Format(wxT("Error Loading ")) + sLibName); 
		else{
			void (*dynLoad)(HandlerLibInterface *) = reinterpret_cast<void (*)(HandlerLibInterface* )>(dllDev.GetSymbol(wxT("dynLoad")));
			if(dynLoad) {
				HandlerLibInterface HLI;
				dynLoad(&HLI);
				strIntConf.setHandlerLibs.insert( HandlerLibData( md5, sLibName, 
							strIntConf.sHandlersDir,
							HLI,dllDev.Detach()));
			}else dllDev.Unload();

		}
	}while(dirHandlers.GetNext( &sLibName));

}

struct  Includer: public std::unary_function<wxString &, void>{
	wxString& iVictim;

	Includer( wxString &i): iVictim(i){};
	inline void operator()(const wxString& sIncl){
		MyFrame::LoadScript(sIncl, iVictim);
	};
};
bool MyFrame::LoadScript(const wxString &filename, wxString &sGoodScript){
	wxTextFile file;
	wxFileName fname(filename);
	wxString str, sPath = fname.GetPath();
	std::list<wxString> lIncludes;
	wxRegEx reRegexdo(wxT("\\mdo\\M"), wxRE_ADVANCED + wxRE_ICASE );
	wxRegEx reRegexInclude(wxT("#include \"(.+?)\""), wxRE_ADVANCED + wxRE_ICASE );
	wxRegEx reRegexSleep(wxT("([\\w\\?\\!\\.\\_]*\\s*=\\s*)*(call\\s*)?sleep[\\s\\(]\\s*([-_.\\w]+)(\\s*\\))?"), wxRE_ADVANCED + wxRE_ICASE );
	if (!file.Open(filename)){
		wxMessageBox(wxT("Can't load ")+filename, wxT("Error loading script"),wxICON_ERROR );
		return false; 
	};
	for ( str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine() ) {
		if (reRegexInclude.Matches(str)) {
			lIncludes.push_back(sPath+ wxFileName::GetPathSeparators()+reRegexInclude.GetMatch(str, 1));
			reRegexInclude.ReplaceAll(&str, wxT("/*&*/"));
		}
		sGoodScript << str;
		if (reRegexdo.Matches(str)) sGoodScript << wxT(";call ChkDeath 0; \r\n");
		else sGoodScript << wxT("\r\n");

	}
	sGoodScript << str;
	reRegexSleep.ReplaceAll(&sGoodScript, wxT(";call ChkDeath \\3;"));
	file.Close();

	std::for_each(lIncludes.begin(), lIncludes.end(), Includer(sGoodScript));
	return true;
}
void MyFrame::WriteText(const wxString& str, const wxTextAttr& sty){

	m_textCtrl1->SetDefaultStyle(sty);
	m_textCtrl1->AppendText(str);
}
bool MyFrame::CreateThread(MyFrame *m_frame, const wxString&sFPath, unsigned long &TID){
	wxString sScript;
	if (!LoadScript(sFPath, sScript) )return false;
	
	wxString sFName = wxFileName(sFPath).GetFullName();
	MyThread *thread = new MyThread(m_frame, sScript, sFName, sFPath);
	if ( thread->Create() != wxTHREAD_NO_ERROR ){
		wxLogError(wxT("Can't create thread!"));
		return false;
	}
	TID = vciMailingList.insert(thread);
	thread->setID(TID);

	m_listCtrl1->AddItem(sFName, TID);
	return true;
}


void MyFrame::onThreadLC(wxCommandEvent&evt){
	ThreadMsg* thM = static_cast<ThreadMsg*>(evt.GetClientData());

	long item = m_listCtrl1->FindItem(-1,thM->pthSpeaker);
	wxString str (wxDateTime::Now().Format(wxT("[%F %H-%M-%S]")));
	WriteText(str,*wxBLUE);
	str = wxT(" Task done ");
	WriteText(str,*wxRED);
	str = thM->sName;
	str += wxString::Format(wxT(" (TID#%ld)\n"), thM->pthSpeaker);
	WriteText(str,*wxBLUE);
	if (item !=-1)
		m_listCtrl1->DeleteI(item);
	delete thM;
}

void MyFrame::onSureTerminate(wxCommandEvent& WXUNUSED(evt)){
	m_button1->Enable( true );
	return;
}
void MyFrame::onDynTask(wxMenuEvent& evt){
	wxMenuItem *cTask = aiDynTasks[evt.GetId() - mID_USEDTASKS];
	unsigned long TID;
	if (!startTask(cTask->GetHelp(), TID)) return;
	wxMenuItem *victim = 0;
	m_menu11->Remove(cTask);
	m_menu11->Prepend(cTask); 

	return;
}

bool MyFrame::startTask(wxString str,unsigned long &TID){	
	if (!CreateThread(this,str, TID)) return false;
	if ( vciMailingList[TID]->Run() != wxTHREAD_NO_ERROR ){
		wxLogError(wxT("Can't start thread!"));
		WriteText(wxT("Error starting a new task\n"), wxTextAttr(*wxRED));
	}
	return true;
}

void MyFrame::onNewCommand(wxCommandEvent& WXUNUSED(evt)){

	wxString str = wxFileSelector(wxT("Choose a script file"), strIntConf.sScriptsDir);
	if ( str.empty() )return;
	wxFileName fnTask(str);
	str = fnTask.GetFullName();
	wxString sPath = fnTask.GetFullPath();
	unsigned long TID;
	if (!startTask(sPath, TID))return;
	wxMenuItemList::iterator itMenuItem = m_menu11->GetMenuItems().begin();
	wxMenuItemList::iterator itMenuEnd = m_menu11->GetMenuItems().end();
	for(;itMenuItem != itMenuEnd;++itMenuItem )
		if((*itMenuItem)->GetHelp() == sPath) break;
	wxMenuItem *miNewItem;
	if(itMenuItem == itMenuEnd) {	
		int items = m_menu11->GetMenuItemCount();
		if (items ==  strIntConf.ulMaxMenuTasks ) {
			wxMenuItem *victim = m_menu11->FindItemByPosition( strIntConf.ulMaxMenuTasks - 1);
			m_menu11->Remove(victim);
			miNewItem = new wxMenuItem( m_menu11, victim->GetId(), str, sPath, wxITEM_NORMAL );
			delete victim;
		}else
			miNewItem = new wxMenuItem( m_menu11, mID_USEDTASKS+items, str, sPath, wxITEM_NORMAL );
		aiDynTasks[ miNewItem->GetId() - mID_USEDTASKS] = miNewItem;
		m_menu11->Prepend(miNewItem);
	}else{

		wxMenuItem *victim = *itMenuItem;
		m_menu11->Remove(victim);
		m_menu11->Prepend(victim);

	}

}
void MyFrame::onExecReq(wxCommandEvent& evt) {
}

void MyFrame::onNewTool( wxCommandEvent& ){
	wxString str;
	dlgCreateTool dlg(this);
	if (dlg.ShowModal() != wxID_OK) return;
	wxString sPath(dlg.getFname());
	wxFileName fnTask(sPath);
	wxBitmap * bmp = new wxBitmap(dlg.getBitmap());
	wxBitmapBundle BB(*bmp);
    m_toolBar1->AddTool( strIntConf.ulIDTools + m_toolBar1->GetToolsCount(),
            fnTask.GetFullName(), /*label*/
            BB/*dlg.getBitmap()*/,/*bitmap*/
            BB/*dlg.getBitmap()*/,/*bitmap disapled*/
            wxITEM_NORMAL, /*kind*/
            fnTask.GetFullName(),/*shortHelp*/
            sPath,/*longHelp*/
            new wxVariant(dlg.sImagePath));
	m_toolBar1->Realize();
	if(m_toolBar1->GetToolsCount() == strIntConf.ulMaxTools)  GetMenuBar()->Enable(ID_MENUADDTOOL, false);
} 

void MyFrame::onToolClick (wxMenuEvent& evt){
	unsigned long TID;
	startTask( m_toolBar1->GetToolLongHelp(evt.GetId()), TID);
}

void MyFrame::onToolRClick(wxMenuEvent& evt){
	wxPoint point = m_toolBar1->ScreenToClient(wxGetMousePosition());
	wxMenu menu;
	menu.Append(ID_EDIT_TOOL, wxT("Edit Tool"));
	menu.Append(ID_REMOVE_TOOL, wxT("Remove Tool"));
	idCTool = evt.GetId();
	PopupMenu(&menu, point.x, point.y);
}

void MyFrame::onEditTool( wxMenuEvent& ){
	dlgCreateTool dlg(this);
	wxBitmap  bmp(broken_xpm);
	dlg.setFname(m_toolBar1->GetToolShortHelp(idCTool));
	dlg.setBitmap(&bmp);
	if (dlg.ShowModal() != wxID_OK) return;

	wxString sPath(dlg.getFname());
	wxFileName fnTask(sPath);
	int pos = m_toolBar1->GetToolPos(idCTool);
	m_toolBar1->DeleteTool(idCTool);
	wxBitmapBundle BB(dlg.getBitmap());
	m_toolBar1->AddTool( idCTool, fnTask.GetFullName(),BB , wxNullBitmap, wxITEM_NORMAL,fnTask.GetFullName(), sPath/*description*/,new wxVariant(dlg.sImagePath));
	m_toolBar1->Realize();
	m_toolBar1->InsertTool( pos, m_toolBar1->RemoveTool(idCTool));
	m_toolBar1->Realize();
}

void MyFrame::onRemoveTool( wxMenuEvent& ){
	wxVariant* vCImagePath =dynamic_cast<wxVariant*> (m_toolBar1->GetToolClientData(idCTool));  
	if(vCImagePath )
		delete vCImagePath;

	m_toolBar1->DeleteTool(idCTool);
}
void MyFrame::onThreadMsg(wxCommandEvent& evt){
	ThreadMsg *msg =static_cast<ThreadMsg*>(evt.GetClientData());
	wxString str (wxDateTime::Now().Format(wxT("[%F %H-%M-%S]")));
	str << wxString::Format(wxT(" (TID#%ld) "), msg->pthSpeaker);//->GetId());
	str << msg->sName;
	WriteText(str, wxTextAttr(*wxBLUE));
	switch(msg->action){
		case ThreadMsg::TEXT_OUT:
			WriteText(msg->GetStr()+wxT("\n"));
			break;
		case ThreadMsg::ERROR_OUT:
			WriteText(msg->GetStr()+wxT("\n"),wxTextAttr(*wxRED) );
			break;
		case ThreadMsg::TERMINATED:
			WriteText(wxT(" Terminated by user\n"),wxTextAttr(*wxRED) );
			break;

		default:
			WriteText(wxT(" ERROR\n"), wxTextAttr(*wxRED));
	}

	delete msg;
}

void MyFrame::onNewTask(wxCommandEvent& evt){
	wxString str (wxDateTime::Now().Format(wxT("[%F %H-%M-%S]")));
	NewTaskReq* req = reinterpret_cast<NewTaskReq*>(evt.GetClientData());
	str << wxString::Format(wxT(" (TID#%ld) "), req->owner);//->GetId());
	//str << *(req->psTaskName); 
	WriteText(str, wxTextAttr(*wxBLUE));

	unsigned long TID;
	if (!startTask(*req->psTaskName, TID)) return;

	req->TID = TID;
	req->ready.Post();
	WriteText(wxString::Format(wxT("has started a new task (TID#%ld)\n"), TID), wxTextAttr(*wxBLUE));
}

void MyFrame::onListSized(wxSizeEvent &  WXUNUSED(event) ){m_listCtrl1->onSized();}
void MyFrame::evhTerminateThread( wxCommandEvent& WXUNUSED(event) ) {
	m_checkBox1->SetValue(false);
	m_button1->Enable( false );
	long victim = m_listCtrl1->GetSelected();
	if (victim != -1){
		MyThread *pThr = vciMailingList[m_listCtrl1->GetItemData(victim)];
		if(pThr) pThr->Delete();
		

	}

}
void MyFrame::evhSendMsg( wxCommandEvent& WXUNUSED(event) ) {
	long victim = m_listCtrl1->GetSelected();
	if (victim != -1){
		MyThread *pThr = vciMailingList[m_listCtrl1->GetItemData(victim)];
		unsigned long TID = pThr->getID();
		wxMutexLocker ml(vciMailingList[TID]->mtxMail);
		vciMailingList[TID]->semMail.Post();
		vciMailingList[TID]->lstMail.push_back(new wxString(wxT("SYSTEM;")+m_textCtrl2->GetValue()));
	}

}
void MyFrame::onClose(wxCloseEvent& WXUNUSED(event)) {
	saveCfg();
	wxSocketBase::Shutdown();
	Destroy();
}
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	Close(true);
}




bool MyApp::OnInit(){
	m_frame = new MyFrame(NULL);
	m_frame->Show(true);

	SetTopWindow(m_frame);

	return true;
} 
/// Dialogs
dlgCreateTool::dlgCreateTool(wxFrame *parent):MyDialog1(parent), sImagePath(wxEmptyString) {
	m_bitmap2->SetBitmap(broken_xpm);
}

wxString dlgCreateTool::getFname(){
	return m_textCtrl1->GetValue();
}
void dlgCreateTool::setFname(wxString str){
	m_textCtrl1->SetValue(str);
}
void dlgCreateTool::setBitmap(wxBitmap* img ){
	m_bitmap2->SetBitmap(*img);
}
void dlgCreateTool::onBrowseFile( wxCommandEvent& ){
	wxString str = wxFileSelector(wxT("Choose a file for the Tool"), strIntConf.sScriptsDir);
	if ( str.empty() )return;
	wxFileName fnTask(str);
	str = fnTask.GetFullPath();
	m_textCtrl1->SetValue(str);
}

void dlgCreateTool::onAddIcon( wxCommandEvent& ){
	sImagePath = wxFileSelector(wxT("Choose a file for the Tool icon"), strIntConf.sResDir.IsEmpty()? strIntConf.sHomeDir:strIntConf.sResDir+ wxFileName::GetPathSeparator()+wxT("icons"));
	if ( sImagePath.empty() )return;
	wxImage img(sImagePath );
	img.Rescale(16,16);
	m_bitmap2->SetBitmap(img);
}

wxBitmap dlgCreateTool::getBitmap(){
	return m_bitmap2->GetBitmap();
}



IMPLEMENT_APP(MyApp)
