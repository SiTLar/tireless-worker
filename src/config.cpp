#include <wx/xml/xml.h>
#include <wx/stdpaths.h>
#include <wx/dialog.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/sstream.h>
#include <wx/zstream.h>
#include <wx/wfstream.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>
#include <set>
#include <map>
#include <algorithm>
#include "handlerpanel.h"
#include "md5.h"
#include "interface.h"
#include "main.h"
#include "handlerlib.hpp"
#include "broken.xpm"
#include "config.hpp"
#include "default_cfg.h"

extern InternalConfigStuct strIntConf;
extern unsigned long ulTimeQuant;

struct InsertPG : public std::unary_function<PageData&, void>{
	wxFlatNotebook *pNB;
	InsertPG(wxFlatNotebook *inpNB): pNB(inpNB){};
	void operator() ( const PageData& PD){
		pNB->AddPage( PD.pPanel, PD.sName, true );
	}
};

struct GenHandlerPanels : public std::unary_function<HandlerLibData&, void>{
	HandlersHome * pHH;
	GenHandlerPanels(HandlersHome * inp): pHH(inp){};
	void operator() ( const HandlerLibData& inp){
		HandlerLibPanel *pHP = new HandlerLibPanel(pHH->getSW(), pHH->getCount(1) + wxID_HIGHEST, &inp);
		pHH->insertHPanel(pHP);
	};
};

dlgConfig::dlgConfig( wxWindow* parent, wxXmlNode *oConfig,  const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, wxID_LOWEST-1, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_flatNotebook1 = new wxFlatNotebook(this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxFNB_COLORFUL_TABS|wxFNB_FF2|wxFNB_NODRAG|wxFNB_NO_NAV_BUTTONS|wxFNB_NO_X_BUTTON|wxFNB_VC8, wxT("configurator"));

	m_flatNotebook1->SetCustomizeOptions( 0 );
	m_flatNotebook1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	setProps.insert(PageData(wxT("Internals"), m_flatNotebook1));
	pHH = new HandlersHome(m_flatNotebook1);
	m_flatNotebook1->AddPage( pHH, wxT("Handlers"), true );


	setProps.insert(PageData(wxT("Paths"), m_flatNotebook1));
	std::for_each(setProps.begin(),setProps.end(), InsertPG(m_flatNotebook1));
	mainSizer->Add( m_flatNotebook1, 1, wxEXPAND | wxALL, 5 );
	buttonSizer = new wxStdDialogButtonSizer();
	buttonSizerOK = new wxButton( this, wxID_OK );
	buttonSizer->AddButton( buttonSizerOK );
	buttonSizerCancel = new wxButton( this, wxID_CANCEL );
	buttonSizer->AddButton( buttonSizerCancel );
	buttonSizer->Realize();
	mainSizer->Add( buttonSizer, 0, wxEXPAND|wxBOTTOM|wxRIGHT, 5 );

	this->SetSizer( mainSizer );
	this->Layout();

	this->Centre( wxBOTH );
	std::for_each(strIntConf.setHandlerLibs.begin(),strIntConf.setHandlerLibs.end(), GenHandlerPanels(pHH));
	pHH->placeHPanels();
	pHH->applyConfig();
	if (oConfig->GetChildren())
		loadConfig(oConfig->GetChildren());
	Connect(wxID_ANY, wxEVT_PG_SELECTED, wxPropertyGridEventHandler(dlgConfig::onPropSel));
}

dlgConfig::~dlgConfig()
{
	
}
HandlersHome::HandlersHome(wxFlatNotebook*m_flatNotebook1 ): wxPanel( m_flatNotebook1, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0 ), pParent (m_flatNotebook1), count(0){
	m_scrolledWindow2 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL );
	m_scrolledWindow2->SetScrollRate( 5, 5 );
	bSizer1 = new wxBoxSizer(wxVERTICAL ); 
	m_scrolledWindow2->SetSizer(bSizer1);
	pText = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	pText->Wrap( -1 );
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Description") ), wxVERTICAL );
	sbSizer4->Add( pText, 1, wxALL|wxEXPAND, 5 );
	panelSizer = new wxBoxSizer( wxVERTICAL );
	panelSizer->Add( m_scrolledWindow2, 3, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	panelSizer->Add( sbSizer4, 1, wxEXPAND, 5 );
	panelSizer->FitInside( this);
	SetSizer( panelSizer );
	Layout();
}

struct LoadHandlersConf: public std::unary_function<std::pair<const wxString, HandlerLibPanel*>, void>{
	std::map<wxString, std::map<wxString,TypeFlag> > &mapLHConf;
	HandlersHome* pHH;
	LoadHandlersConf(std::map<wxString, std::map<wxString,TypeFlag> > &_mapLHConf, HandlersHome* _pHH): mapLHConf(_mapLHConf), pHH(_pHH){};
	void operator() ( std::pair<const wxString, HandlerLibPanel*> &inp){
		HandlerLibPanel* pHP = inp.second;
		std::map<wxString, std::map<wxString,TypeFlag> >::iterator it = mapLHConf.find(pHP->getMD5());
		if(it != mapLHConf.end() ) pHP->applyConfig(it->second, pHH);
		
	}
};
void HandlersHome::applyConfig(){
	std::for_each(mapHPanels.begin(),mapHPanels.end(), LoadHandlersConf( strIntConf.mapHandlersToUse, this) );

}
void HandlerLibPanel::loadConfigIntoChkList(wxCheckListBox * checkList, std::map<wxString,TypeFlag> &mapConf, HandlersHome*pHH, int flag ) {
	int count = checkList->GetCount();
	for (int idx = 0; idx < count; idx++){
		wxString sLabel = checkList->GetString(idx);
		std::map<wxString,TypeFlag>::iterator it = mapConf.find(sLabel);
		if (it != mapConf.end()?it->second.isSet(flag):0){
			checkList->Check(idx);
			pHH->setSelHandler(sLabel, std::make_pair(idx, checkList), pHH->getIdByFlag(flag));

		}
	}
}	


void HandlerLibPanel::applyConfig (std::map<wxString,TypeFlag> &mapConf, HandlersHome*pHH){ 
	loadConfigIntoChkList(checkListDevs,mapConf, pHH, HandlerLibData::DEVICE );
	loadConfigIntoChkList(checkListLogs,mapConf, pHH, HandlerLibData::LOGGER);

}

PageData::PageData(const wxString &inName, wxFlatNotebook*m_flatNotebook1 ): sName(inName){
	pPanel = new wxPanel( m_flatNotebook1, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	wxBoxSizer* panelSizer = new wxBoxSizer( wxVERTICAL );
	pPG = new wxPropertyGrid(pPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_BOLD_MODIFIED|wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER);
	panelSizer->Add( pPG, 3, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	wxStaticBoxSizer* sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( pPanel, wxID_ANY, wxT("Description") ), wxVERTICAL );
	pText = new wxStaticText( pPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	pText->Wrap( -1 );
	sbSizer4->Add( pText, 1, wxALL|wxEXPAND, 5 );
	panelSizer->Add( sbSizer4, 1, wxEXPAND, 5 );
	pPanel->SetSizer( panelSizer );
	pPanel->Layout();
	panelSizer->Fit( pPanel );
}

void MyFrame::writeDefaultCfg(wxString &sPath){
	wxFile fileCfg(sPath, wxFile::write);
	if(!fileCfg.IsOpened()){
		strIntConf.sCfgFilePath = wxStandardPaths::Get().GetUserConfigDir()+ wxFileName::GetPathSeparator() +wxT("twconfig.xmz") ;
		fileCfg.Open(strIntConf.sCfgFilePath, wxFile::write);
	}
	wxFileOutputStream fstream(fileCfg);
	//wxZlibOutputStream(fstream).Write(pcDefaultCfgFile, sizeof(pcDefaultCfgFile)-1);
	fstream.Write(pcDefaultCfgFile, sizeof(pcDefaultCfgFile)-1);
};

void MyFrame::loadCfg(){
	m_toolBar1->ClearTools();


	wxXmlDocument xmlProgCfg;
	strIntConf.sCfgFilePath = wxStandardPaths::Get().GetUserConfigDir()+ wxFileName::GetPathSeparator() +wxT("twconfig.xmz") ;

	wxFileInputStream fstream(strIntConf.sCfgFilePath);
	//wxZlibInputStream zstream(fstream);
	if(wxFileName::FileExists(strIntConf.sCfgFilePath)) xmlProgCfg.Load(fstream);
	if(!xmlProgCfg.IsOk()){
		wxMessageBox(wxT("Configuration file is corrupted.\n\n Writing blanck configuration file:\n")
				+strIntConf.sCfgFilePath , wxT("Configuration Error"),wxICON_EXCLAMATION );
		writeDefaultCfg(strIntConf.sCfgFilePath);
		wxFileInputStream fdefstream(strIntConf.sCfgFilePath);
		//wxZlibInputStream zdefstream(fdefstream);
		xmlProgCfg.Load(fdefstream);
	}
	//xmlProgCfg.SetFileEncoding(wxT("utf8"));
	wxXmlNode *pcNode = xmlProgCfg.GetRoot()->GetChildren();
	while(pcNode){
		if(pcNode->GetName() == wxT("config")){
			oConfig = new wxXmlNode(*pcNode);
			oConfig->SetType(wxXML_ELEMENT_NODE);
		}
		pcNode = pcNode->GetNext();
	}

	processCfg();

	pcNode = xmlProgCfg.GetRoot()->GetChildren();

	while(pcNode){
		wxXmlProperty *pcProperty ;
		unsigned long count;
		wxString sCNodeName = pcNode->GetName();
		if(sCNodeName == wxT("tools")){
			unsigned long ulPos = 0;
			count = strIntConf.ulMaxTools;
			wxString sPath, sLable, sImage(wxEmptyString);
			wxXmlNode *toolNode = pcNode->GetChildren(); 
			while(toolNode && count--){
				pcProperty = toolNode->GetProperties();
				while(pcProperty){
					if (pcProperty->GetName() == wxT("path")) sPath= pcProperty->GetValue();
					else if (pcProperty->GetName() == wxT("descr")) sLable = pcProperty->GetValue();
					else if (pcProperty->GetName() == wxT("pos")) pcProperty->GetValue().ToULong(&ulPos);
					else if (pcProperty->GetName() == wxT("image")) sImage = pcProperty->GetValue();
					pcProperty = pcProperty->GetNext();
				}
				if(!sPath.IsEmpty()/* && !sLable.IsEmpty()*/){
					wxBitmap  bmp(broken_xpm);
					if(!sImage.IsEmpty()) {
						wxStringInputStream stream(sImage);
						wxImage img(stream, wxBITMAP_TYPE_XPM );
						bmp = wxBitmap(img);
					}
					m_toolBar1->InsertTool( ulPos, strIntConf.ulIDTools +ulPos, bmp, wxNullBitmap, false, NULL, sPath, sLable);
				}
				sPath.Empty(); sImage.Empty(); sLable.Empty(); ulPos = 0;
				toolNode= toolNode->GetNext();
			}

		} else if(sCNodeName == wxT("menutasks")){
			wxString sName, sPath;
			wxXmlNode *menuNode = pcNode->GetChildren();
			wxMenuItem *miNewItem;
			count = strIntConf.ulMaxMenuTasks;
			long iPos = -1, id = 0;
			while(menuNode && count--){
				pcProperty = menuNode->GetProperties();
				while(pcProperty){
					if (pcProperty->GetName() == wxT("path")) sPath = pcProperty->GetValue();
					else if (pcProperty->GetName() == wxT("name")) sName = pcProperty->GetValue();
					else if (pcProperty->GetName() == wxT("id"))  pcProperty->GetValue().ToLong(&id);
					else if (pcProperty->GetName() == wxT("pos"))  pcProperty->GetValue().ToLong(&iPos);
					pcProperty = pcProperty->GetNext();
				}
				if(!sName.IsEmpty() && !sPath.IsEmpty() && id){
					miNewItem = new wxMenuItem( m_menu11, id, sName, sPath, wxITEM_NORMAL );
					aiDynTasks[ id - mID_USEDTASKS] = miNewItem;
					m_menu11->Insert( iPos, miNewItem);
				}
				sName.Empty(); sPath.Empty(); id = 0; iPos = -1;
				menuNode= menuNode->GetNext();
			}
		} else if(sCNodeName == wxT("Handlers")){
			wxXmlNode *handlerLibNode = pcNode->GetChildren();
			while(handlerLibNode ){
				std::map<wxString,TypeFlag> mapToUse;
				wxXmlNode *typeNode = handlerLibNode->GetChildren();
				while(typeNode){
					wxString sTName = typeNode->GetName();
					wxXmlNode *handlerNode = typeNode->GetChildren();
					int iType = 0;
					while(handlerNode){
						if(sTName == wxT("devices")) iType = HandlerLibData::DEVICE;
						else if(sTName == wxT("logs")) iType = HandlerLibData::LOGGER;
						mapToUse[handlerNode->GetName()].set(iType);
						handlerNode=handlerNode->GetNext();
					}
					typeNode = typeNode->GetNext();
				}
				strIntConf.mapHandlersToUse[handlerLibNode->GetPropVal(wxT("md5"), wxT("error"))] = mapToUse;
				handlerLibNode = handlerLibNode->GetNext();
			}

		}
		pcNode = pcNode->GetNext();
	}
	m_toolBar1->Realize();
	if(m_toolBar1->GetToolsCount() == strIntConf.ulMaxTools)  GetMenuBar()->Enable(ID_MENUADDTOOL, false);

};


void MyFrame::processCfg(){
	if (!oConfig)return;
	wxXmlNode *pcNode;
	wxXmlNode *cfgNode = oConfig->GetChildren();
	wxString sVal;
	while(cfgNode){
		cfgNode->SetType(wxXML_ELEMENT_NODE);
		if(cfgNode->GetName() == wxT("Paths")){
			pcNode = cfgNode->GetChildren();
			while(pcNode){
				pcNode->SetType(wxXML_ELEMENT_NODE);
				if(pcNode->GetName() == wxT("home")){
					strIntConf.sHomeDir = pcNode->GetPropVal(wxT("value"), wxEmptyString);
					if (strIntConf.sHomeDir.IsEmpty()){
						strIntConf.sHomeDir = wxStandardPaths::Get().GetDocumentsDir()+ wxFileName::GetPathSeparator() +wxT("TirelessWorker");
						pcNode->DeleteProperty(wxT("value"));
						pcNode->AddProperty(wxT("value"),strIntConf.sHomeDir); 
					}
				} else if(pcNode->GetName() == wxT("scripts")){
					strIntConf.sScriptsDir = pcNode->GetPropVal(wxT("value"), wxEmptyString);
					if (strIntConf.sScriptsDir.IsEmpty()){ 
						strIntConf.sScriptsDir = strIntConf.sHomeDir+wxFileName::GetPathSeparator() + wxT("scripts");
						pcNode->DeleteProperty(wxT("value"));
						pcNode->AddProperty(wxT("value"),strIntConf.sScriptsDir);
					}
				}else if(pcNode->GetName() == wxT("handlers")){
					strIntConf.sHandlersDir = pcNode->GetPropVal(wxT("value"), wxEmptyString);
					if (strIntConf.sHandlersDir.IsEmpty()){ 
						pcNode->DeleteProperty(wxT("value"));
						strIntConf.sHandlersDir =  wxStandardPaths::Get().GetPluginsDir() +wxFileName::GetPathSeparator() + wxT("handlers");
						pcNode->AddProperty(wxT("value"),strIntConf.sHandlersDir);
					}
				}else if(pcNode->GetName() == wxT("logs")){
					strIntConf.sLogsDir = pcNode->GetPropVal(wxT("value"), wxEmptyString);
					if (strIntConf.sLogsDir.IsEmpty()){ 
						pcNode->DeleteProperty(wxT("value"));
						strIntConf.sLogsDir = strIntConf.sHomeDir+wxFileName::GetPathSeparator() + wxT("logs");
						pcNode->AddProperty(wxT("value"),strIntConf.sLogsDir);
					}
				}else if(pcNode->GetName() == wxT("res")){
					strIntConf.sResDir = pcNode->GetPropVal(wxT("value"), wxEmptyString);
					if (strIntConf.sResDir.IsEmpty()){ 
						strIntConf.sResDir =  wxStandardPaths::Get().GetResourcesDir() +wxFileName::GetPathSeparator() + wxT("res");
						pcNode->DeleteProperty(wxT("value"));
						pcNode->AddProperty(wxT("value"),strIntConf.sResDir );
					}
				}
				pcNode = pcNode->GetNext();
			}

		} else if(cfgNode->GetName() == wxT("Internals")){
			pcNode = cfgNode->GetChildren();
			while(pcNode){
				pcNode->SetType(wxXML_ELEMENT_NODE);
				if(pcNode->GetName() == wxT("maxdynmenu"))
					pcNode->GetPropVal(wxT("value"), wxEmptyString).ToULong(&strIntConf.ulMaxMenuTasks);
				else if(pcNode->GetName() == wxT("maxtools"))
					pcNode->GetPropVal(wxT("value"), wxEmptyString).ToULong(&strIntConf.ulMaxTools);
				else if(pcNode->GetName() == wxT("timequant"))
					pcNode->GetPropVal(wxT("value"), wxEmptyString).ToULong(&ulTimeQuant);
				pcNode = pcNode->GetNext();
			}

		}
		cfgNode = cfgNode->GetNext();
	}	
	if (!wxDir::Exists(strIntConf.sHomeDir))wxMkdir(strIntConf.sHomeDir, 0755);
	if (!wxDir::Exists(strIntConf.sScriptsDir))wxMkdir(strIntConf.sScriptsDir, 0755);
	if (!wxDir::Exists(strIntConf.sHandlersDir))wxMkdir(strIntConf.sHandlersDir, 0755);
	if (!wxDir::Exists(strIntConf.sLogsDir))wxMkdir( strIntConf.sLogsDir, 0755);
	wxSetWorkingDirectory(strIntConf.sLogsDir);
	wxMenuItemList mlist(m_menu11->GetMenuItems());
	if (!(mlist.IsEmpty())){
		wxMenuItemList::compatibility_iterator it(mlist.GetFirst()); 

		while(it){
			m_menu11->Destroy(it->GetData());
			it = it->GetNext();
		}
	}
	delete[] aiDynTasks;
	aiDynTasks = new wxMenuItem* [strIntConf.ulMaxMenuTasks];
	strIntConf.ulIDTools = mID_USEDTASKS+strIntConf.ulMaxMenuTasks; //TODO
}

struct SaveHandlerToXml : public std::unary_function<std::pair<const wxString,TypeFlag> , void>{

	wxXmlNode* nodeDevs;
	wxXmlNode* nodeLogs;
	SaveHandlerToXml(wxXmlNode* _nodeDevs, wxXmlNode* _nodeLogs): nodeDevs(_nodeDevs), nodeLogs(_nodeLogs) {};
	void operator() ( std::pair<const wxString, TypeFlag > & inp){
		if(inp.second.isSet(HandlerLibData::DEVICE))new wxXmlNode(nodeDevs, wxXML_ELEMENT_NODE, inp.first);
		if(inp.second.isSet(HandlerLibData::LOGGER))new wxXmlNode(nodeLogs, wxXML_ELEMENT_NODE, inp.first);
	}
};

struct SaveHandlerLibToXml : public std::unary_function<std::pair<const wxString, std::map< wxString,TypeFlag> > , void>{
	wxXmlNode * node;
	SaveHandlerLibToXml(wxXmlNode *_node): node(_node){};
	void operator() ( std::pair<const wxString, std::map<wxString,TypeFlag> > & inp){
		wxXmlNode* nodeLib = new wxXmlNode(node, wxXML_ELEMENT_NODE, wxT("library"));
		wxXmlNode* nodeDevs = new wxXmlNode(nodeLib, wxXML_ELEMENT_NODE, wxT("devices"));
		wxXmlNode* nodeLogs = new wxXmlNode(nodeLib, wxXML_ELEMENT_NODE, wxT("logs"));
		nodeLib->AddProperty(new wxXmlProperty(wxT("md5"), inp.first));
		std::for_each(inp.second.begin(), inp.second.end(),SaveHandlerToXml(nodeDevs, nodeLogs) );
	}
};

void MyFrame::saveCfg(){
	wxXmlDocument xmlCfg;
	wxString strImage;
	wxXmlNode* nodeRoot = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("root"));
	wxXmlNode* node;

	if(oConfig)nodeRoot->AddChild(new wxXmlNode(*oConfig));
	wxXmlNode* nodeTools = new wxXmlNode(nodeRoot, wxXML_ELEMENT_NODE, wxT("tools"));
	int iCTool = m_toolBar1->GetToolsCount();
	while(iCTool){
		node = new wxXmlNode(nodeTools, wxXML_ELEMENT_NODE, wxT("tool"));
		node->AddProperty(new wxXmlProperty(wxT("path"),m_toolBar1->GetToolShortHelp(--iCTool + strIntConf.ulIDTools) ));
		node->AddProperty(new wxXmlProperty(wxT("descr"), m_toolBar1->GetToolLongHelp(strIntConf.ulIDTools +iCTool)));
		node->AddProperty(new wxXmlProperty(wxT("pos"), wxString::Format(wxT("%d"), iCTool)));
		wxToolBarToolBase* pTool = m_toolBar1->FindById(strIntConf.ulIDTools +iCTool);
		if (pTool){
			strImage.Empty();
			wxStringOutputStream stream(&strImage);
			pTool->GetBitmap().ConvertToImage().SaveFile(stream,wxBITMAP_TYPE_XPM );
			node->AddProperty(new wxXmlProperty(wxT("image"), strImage));
		}

	}

	wxXmlNode* nodeMenus = new wxXmlNode(nodeRoot, wxXML_ELEMENT_NODE, wxT("menutasks"));

	int iCMenu = m_menu11->GetMenuItemCount();
	while(iCMenu){
		node = new wxXmlNode(nodeMenus, wxXML_ELEMENT_NODE, wxT("menu"));
		wxMenuItem* pCMenu = m_menu11->FindItemByPosition(--iCMenu); 
		node->AddProperty(new wxXmlProperty(wxT("id"), wxString::Format(wxT("%d"), pCMenu->GetId())));
		node->AddProperty(new wxXmlProperty(wxT("pos"), wxString::Format(wxT("%d"), iCMenu)));
		node->AddProperty(new wxXmlProperty(wxT("path"), pCMenu->GetHelp()));
		node->AddProperty(new wxXmlProperty(wxT("name"), pCMenu->GetItemLabelText()));
		//nodeMenu->AddChild(node);
	}


	wxXmlNode* nodeHandlers = new wxXmlNode(nodeRoot, wxXML_ELEMENT_NODE, wxT("Handlers"));

	std::for_each(strIntConf.mapHandlersToUse.begin(), strIntConf.mapHandlersToUse.end(), SaveHandlerLibToXml(nodeHandlers));

	xmlCfg.SetRoot(nodeRoot);
	//wxFileOutputStream fstream(strIntConf.sCfgFilePath);
	//wxZlibOutputStream zstream(fstream);
	wxFileOutputStream zstream(strIntConf.sCfgFilePath);
	xmlCfg.Save(zstream);

}
void MyFrame::onConfig( wxCommandEvent& ){
	saveCfg();
	loadHandlers();
	dlgConfig dlg(this, oConfig);
	if (dlg.ShowModal() == wxID_CANCEL) return;
	delete oConfig;
	oConfig = dlg.getCfg();
	std::map<wxString, std::map<wxString,TypeFlag> > mapHandlersToUse;
	dlg.supplyHandlers(mapHandlersToUse);
	if (pocessHandlers(mapHandlersToUse))
		strIntConf.mapHandlersToUse = mapHandlersToUse;
	saveCfg();
	loadCfg();

}

struct GenConf : public std::unary_function<PageData&, void>{
	wxXmlNode* parentNode;
	GenConf(wxXmlNode*inp): parentNode(inp){};
	void operator() ( const PageData& PD){
		wxXmlNode* rootNode= new wxXmlNode( parentNode, wxXML_ELEMENT_NODE, PD.sName);
		wxPGVIterator pgIt = PD.pPG->GetVIterator(wxPG_ITERATE_ALL );
		while(!pgIt.AtEnd()){
			wxPGProperty * pgP = pgIt.GetProperty();
			wxXmlNode* node = new wxXmlNode(rootNode, wxXML_ELEMENT_NODE, pgP->GetName());
			node->AddProperty(new wxXmlProperty(wxT("type"), PD.pPG->GetPropertyShortClassName(pgP->GetId())));
			node->AddProperty(new wxXmlProperty(wxT("value"),pgP->GetValueAsString()));
			node->AddProperty(new wxXmlProperty(wxT("descr"),pgP->GetLabel()));
			node->AddProperty(new wxXmlProperty(wxT("help"),pgP->GetHelpString()));
			pgIt.Next();
		}
	}
};


wxXmlNode* dlgConfig::getCfg(){
	wxXmlNode* xmlCfgNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("config"));
	std::for_each(setProps.begin(), setProps.end(), GenConf(xmlCfgNode));
	return xmlCfgNode;

}

void dlgConfig::supplyHandlers(std::map<wxString, std::map<wxString,TypeFlag> >&mapOut){
	pHH->supplyHandlers(mapOut);
}



void dlgConfig::switchPage( wxFlatNotebookEvent& event){

}


void dlgConfig::loadConfig(wxXmlNode* pcNode){
	while(pcNode){
		if(pcNode->GetName() == wxT("Paths"))
			loadCGrid(setProps.find(PageData(wxT("Paths")))->pPG, pcNode->GetChildren());
		else if(pcNode->GetName() == wxT("Internals"))
			loadCGrid(setProps.find(PageData(wxT("Internals")))->pPG, pcNode->GetChildren());
		pcNode = pcNode->GetNext();
	};
}
void dlgConfig::loadCGrid(wxPropertyGrid* pGrid, wxXmlNode* pcNode){
	while(pcNode){
		wxXmlProperty *pcProperty = pcNode->GetProperties(); wxXmlProperty *pcPrVal = 0;
		wxPGProperty * pGridProperty = 0;
		wxString sLable, sValue, sHelp, sType;
		while(pcProperty){
			if (pcProperty->GetName() == wxT("help")) sHelp = pcProperty->GetValue();
			else if (pcProperty->GetName() == wxT("descr")) sLable = pcProperty->GetValue();
			else if (pcProperty->GetName() == wxT("value")){ 
				sValue = pcProperty->GetValue();
				pcPrVal = pcProperty;
			}else if (pcProperty->GetName() == wxT("type")){
				sType = pcProperty->GetValue();
				if(sType == wxT("Dir")) pGridProperty = new wxDirProperty;
				else if(sType == wxT("Int")) pGridProperty = new wxIntProperty;
				else if(sType == wxT("Long")) pGridProperty = new wxIntProperty;
			}
			pcProperty = pcProperty->GetNext();
		};

		if(pGridProperty){
			wxVariant vValue;
			if(sType == wxT("Int")){ 
				long tmp;
				sValue.ToLong(&tmp);
				vValue = tmp;
			} else vValue = sValue;
			pGridProperty->SetName(pcNode->GetName());
			pGridProperty->SetLabel(sLable);
			pGridProperty->SetValue(vValue);
			if(!pcPrVal) {
				pcPrVal = new wxXmlProperty(wxT("value"),wxEmptyString);
				pcNode->AddProperty(pcPrVal);
			}
			pGridProperty->SetClientData(pcPrVal);

			pGrid->Append(pGridProperty);
			pGrid->SetPropertyHelpString(pGridProperty, sHelp);
		}

		pcNode = pcNode->GetNext();
	}
}
void dlgConfig::onChkListTog(wxCommandEvent& event){
	wxCheckListBox* checkList = dynamic_cast<wxCheckListBox*>(event.GetEventObject());
	if (!checkList)wxMessageBox(wxT("oops"));
	pHH->setSelHandler(checkList->GetString(event.GetInt()), std::make_pair(event.GetInt(), checkList), event.GetId());

}
void dlgConfig::onChkListSel(wxCommandEvent& event){
	wxCheckListBox* checkList = dynamic_cast<wxCheckListBox*>(event.GetEventObject());
	if (!checkList)wxMessageBox(wxT("oops"));
	HandlerLibPanel* pHP = dynamic_cast<HandlerLibPanel*>(checkList->GetParent()); 
	if (!pHP)wxFAIL_MSG(wxT("fail")); 
	wxString str;
	std::map<wxString, wxString>::iterator it;
	pHH->pText->SetLabel(wxEmptyString);
	switch (event.GetId()){
		case idCHKLISTDEV:
			it = pHP->mapDevInfo.find(checkList->GetString(event.GetInt()) );
			if(it == pHP->mapDevInfo.end()) return; 
			break;
		case idCHKLISTLOG:
			it = pHP->mapLogInfo.find(checkList->GetString(event.GetInt()) );
			if(it == pHP->mapLogInfo.end()) return;
			break;
		default:
			wxFAIL_MSG(wxT("fail")); 
			return;
	}
	pHH->pText->SetLabel(it->second);

}
	void dlgConfig::onPropSel(wxPropertyGridEvent& event){
		if(event.HasProperty())
			setProps.find(PageData(m_flatNotebook1->GetPageText(m_flatNotebook1->GetSelection())))->pText->SetLabel(event.GetProperty()->GetHelpString());
	}

struct  LoadChkBox: public std::unary_function<wxString const &, void>{
	wxCheckListBox *dest;
	LoadChkBox(wxCheckListBox *inpDest): dest(inpDest){};
	inline void operator()( wxString const &input){
		dest->Append(input);
	}

};
struct  GenInfo: public std::unary_function< std::pair<std::string, std::string>  const &, void>{
	std::map< wxString,wxString > & dest;
	GenInfo( std::map< wxString,wxString > & _dest  ): dest(_dest){};
	inline void operator()(std::pair<std::string, std::string> const &inp ){
		dest[wxString::FromUTF8(inp.first.c_str())] = wxString::FromUTF8(inp.second.c_str());
	}

};

HandlerLibPanel::HandlerLibPanel( wxWindow* parent, int id, const HandlerLibData* inp):
	HandlerLibPanelProto(parent, id), phData(inp){

		sboxLibName->GetStaticBox()->SetLabel(inp->sName);
		textMD5->SetValue(inp->md5);
		textDescr->SetValue(inp->sDescr);
		textPath->SetValue(inp->sPath+ wxFileName::GetPathSeparator() + inp->sName);

		{
			std::map<std::string,DevInterface*>::const_iterator it = inp->HLI.mapDevs.begin();
			LoadChkBox lcb(checkListDevs);
			while(it != inp->HLI.mapDevs.end()) lcb(wxString((*it++).first.c_str(), wxConvUTF8));

		}

		{
			std::map<std::string,Logger *>::const_iterator it = inp->HLI.mapLogs.begin();
			LoadChkBox lcb(checkListLogs);
			while(it != inp->HLI.mapLogs.end()) lcb(wxString((*it++).first.c_str(), wxConvUTF8));
		}
		std::for_each( inp->HLI.mapDevsInfo.begin(), inp->HLI.mapDevsInfo.end(), GenInfo(mapDevInfo) );
		std::for_each( inp->HLI.mapLogsInfo.begin(), inp->HLI.mapLogsInfo.end(), GenInfo(mapLogInfo) );


		checkListDevs->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( dlgConfig::onChkListTog), NULL, wxWindow::FindWindowById(wxID_LOWEST -1));
		checkListDevs->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( dlgConfig::onChkListSel), NULL, wxWindow::FindWindowById(wxID_LOWEST -1));
		checkListLogs->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( dlgConfig::onChkListTog), NULL,wxWindow::FindWindowById(wxID_LOWEST -1));
		checkListLogs->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( dlgConfig::onChkListSel), NULL,wxWindow::FindWindowById(wxID_LOWEST -1));

	}

HandlerLibPanel::~HandlerLibPanel() {

	checkListDevs->Disconnect(  wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( dlgConfig::onChkListTog), NULL, wxWindow::FindWindowById(wxID_LOWEST -1));
	checkListDevs->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( dlgConfig::onChkListSel), NULL, wxWindow::FindWindowById(wxID_LOWEST -1));
	checkListLogs->Disconnect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( dlgConfig::onChkListTog), NULL ,wxWindow::FindWindowById(wxID_LOWEST -1));
	checkListLogs->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( dlgConfig::onChkListSel), NULL,wxWindow::FindWindowById(wxID_LOWEST -1));
}
inline void HandlerLibPanel::chkboxToMap(std::map<wxString,TypeFlag> &mapOut, wxCheckListBox *checkList, int iType){
	int max = checkList->GetCount();
	for(int idx = 0 ; idx < max ; idx++)
		if (checkList->IsChecked(idx))
			mapOut[checkList->GetString(idx)].set(iType);
}

void HandlerLibPanel::getSelHandlersMap(std::map<wxString,TypeFlag> &mapOut){
	chkboxToMap (mapOut, checkListDevs, HandlerLibData::DEVICE);
	chkboxToMap (mapOut, checkListLogs, HandlerLibData::LOGGER);
}


inline void HandlersHome::insertHPanel( HandlerLibPanel*phPanel){
	mapHPanels[phPanel->getMD5()] = phPanel;
}
struct LoadMapOut : public std::unary_function<std::pair<wxString const,HandlerLibPanel *> , void>{
	std::map<wxString, std::map<wxString,TypeFlag> >&mapOut;
	LoadMapOut(std::map<wxString, std::map<wxString,TypeFlag> >& _mapOut): mapOut(_mapOut){};
	void operator()(std::pair<wxString const,HandlerLibPanel *>pahPanel){
		pahPanel.second->getSelHandlersMap(mapOut[pahPanel.second->getMD5()] );
	}
};
void HandlersHome::supplyHandlers(std::map<wxString, std::map<wxString,TypeFlag> >&mapOut){
	std::for_each(mapHPanels.begin(), mapHPanels.end(), LoadMapOut(mapOut));	
}
void HandlersHome::setSelHandler(wxString sName, std::pair<int, wxCheckListBox * >const& inp, int iType){

	std::map<wxString, std::pair<int, wxCheckListBox * > > * pmapSelHandlers;
	switch (iType){
		case idCHKLISTDEV :
			pmapSelHandlers = &mapSelDevs;
			break;
		case idCHKLISTLOG :
			pmapSelHandlers = &mapSelLogs;
			break;
	}


	std::map<wxString,  std::pair<int, wxCheckListBox * > >::iterator it = pmapSelHandlers->find(sName);

	if( it != pmapSelHandlers->end()){
		if (inp.second->IsChecked(inp.first)) {
			it->second.second->Check(it->second.first, false);
			inp.second->Check(inp.first, true);
		}else {
			pmapSelHandlers->erase(it);
			return;
		}
	}
	(*pmapSelHandlers)[sName] = inp;
}

struct AddHandlerLibPanels : public std::unary_function<std::pair<wxString const,HandlerLibPanel *> , void>{
	wxSizer * bSizer1;
	AddHandlerLibPanels(wxSizer * _bSizer1):  bSizer1( _bSizer1){};
	void operator()(std::pair<wxString const,HandlerLibPanel *>pahPanel){
		bSizer1->Add( pahPanel.second, 0, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxFIXED_MINSIZE, 5 );
	}
};
void HandlersHome::placeHPanels(){
	std::for_each(mapHPanels.begin(),mapHPanels.end(), AddHandlerLibPanels(bSizer1));
	bSizer1->FitInside( m_scrolledWindow2 );
	m_scrolledWindow2->Layout();
}

