#ifndef __main__
#define __main__
#include <iterator>
#include <wx/dynlib.h>
#include "handlerlib.hpp"
/**
@file
Subclass of elMFrame, which is generated by wxFormBuilder.
*/
#include "frame.h"
//#include <list>

//// end generated include

class MyThread;
class MyFrame;
class DevInterface;
/** Implementing elMFrame */
/*class MTListItem{
	~MTListItem();
	public:
		MTListItem(MyThread * pInThr):pThr(pInThr), lTID(pInThr->GetId()){};
		void destroy() const {
			delete pThr;
			delete this; 
		};
	MyThread * pThr;
	unsigned long lTID;
};
*/

enum{
	IDS_START = wxID_HIGHEST,
	ID_EDIT_TOOL,
	ID_REMOVE_TOOL,
	mID_USEDTASKS
};
class DevDesc;
class Logger;


class TypeFlag{
	unsigned int flag;
	public:
	TypeFlag():flag(0){}
	inline unsigned int set(unsigned int inp){return flag |= inp;};
	inline unsigned int isSet(unsigned int inp) const{return flag & inp;};
};
struct InternalConfigStuct{

	wxString sCfgFilePath, sHomeDir, sScriptsDir, sHandlersDir, sResDir, sLogsDir ;
	unsigned long ulMaxMenuTasks, ulMaxTools, ulIDTools;
	std::set<HandlerLibData> setHandlerLibs;
	std::map<wxString, std::map<wxString,TypeFlag> > mapHandlersToUse;
	//std::map<wxString,wxString> mapHandlersToUse;
};
class MyApp : public wxApp
{
	public:
		MyApp(){};
		virtual ~MyApp(){};

		virtual bool OnInit();

		MyFrame* m_frame;
	private:

};
DECLARE_APP(MyApp)
	class MyFrame : public elMFrame
{
	protected:
		// Handlers for elMFrame events.
		wxMenuItem ** aiDynTasks;
		void loadCfg();
		void saveCfg();
		virtual void evhTerminateThread( wxCommandEvent& event );
		virtual void evhSendMsg( wxCommandEvent& event );
		void WriteText(const wxString& ,const wxTextAttr&sty = wxTextAttr(*wxBLACK));
		bool CreateThread(MyFrame *, const wxString&, unsigned long &);
		virtual void onNewCommand(wxCommandEvent& );
		void onThreadLC(wxCommandEvent& );
		void onThreadMsg(wxCommandEvent& );
		void onNewFrame(wxCommandEvent& );
		void onNewTask(wxCommandEvent& );
		virtual void onListSized( wxSizeEvent & );
		virtual void onSureTerminate(wxCommandEvent &);
		void onDynTask(wxMenuEvent &);
		virtual void onNewTool( wxCommandEvent& ) ;
		void onEditTool( wxMenuEvent& ) ;
		void onRemoveTool( wxMenuEvent& ) ;
		void onToolClick( wxMenuEvent& );
		void onToolRClick( wxMenuEvent& );
		virtual void onExecReq( wxCommandEvent & );
		virtual void onConfig( wxCommandEvent & );
		virtual void onClose( wxCloseEvent & );
		virtual void OnQuit(wxCommandEvent&);
		bool startTask(wxString, unsigned long &);
		void writeDefaultCfg(wxString &);
		void loadHandlers();
		bool pocessHandlers(std::map<wxString, std::map<wxString,TypeFlag> > &);
		static bool LoadScript(const wxString & , wxString &);	
		int idCTool;
		int counter;	
		wxXmlNode *oConfig;



		//std::list<*MTListItem> stlpLMyThr;
		//std::list<*MyThread> stlpLMyThr;
		friend class Includer;	
	public:
		/** Constructor */
		MyFrame( wxWindow* parent );
		void processCfg();

		//// end generated class members

};

class dlgCreateTool: public MyDialog1 {
	public:
		wxString sImagePath;
		dlgCreateTool(wxFrame *);
		wxString getFname();
		wxBitmap getBitmap();
		void setFname(wxString );
		void setBitmap(wxBitmap* );
		void onBrowseFile( wxCommandEvent& );
		void onAddIcon( wxCommandEvent& );
};


#endif // __elmain__
