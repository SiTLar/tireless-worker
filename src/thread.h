#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/string.h> 
#include <wx/numdlg.h>
#include <wx/progdlg.h>
#include <wx/init.h>
#include <wx/thread.h>
#include <set>
#include <map>
#include <list>
#include <vector>
#include "vecid.hpp"
#include "mycontrolls.hpp"
extern "C" {
#define INCL_RXFUNC
#define INCL_RXSYSEXIT
#include "rexxsaa.h" 
#include "defines.h"
#include "rxpack.h"
}

typedef  std::pair<const char*, APIRET APIENTRY (*)( RFH_ARG0_TYPE , RFH_ARG1_TYPE , RFH_ARG2_TYPE , RFH_ARG3_TYPE , RFH_ARG4_TYPE )> RXFDATA;

class DevDesc;
class LogDesc;
class MyThread;
class MyFrame;
enum{
	NEW_FRAME = 0,
	elTHREAD_MSG ,
	THREAD_START_THREAD,
	NEW_TASK,
	elTHREAD_LIFECYCLE, 
};

/*
 struct VcimlApply{
	virtual operator()()
};
*/
class Vciml{ 
	vecid<MyThread*> buffer;
	public:
	Vciml(): buffer(0){};
	wxMutex mtx;
	//inline const std::vector< std::pair<> > * getVector(){return s;};
	inline int erase(unsigned long TID ){wxMutexLocker ml(mtx); return buffer.erase(TID);};
	inline unsigned long insert( MyThread* const &pTh){wxMutexLocker ml(mtx); return buffer.insert(pTh);}; 
	inline MyThread* item(unsigned long TID){wxMutexLocker ml(mtx); return buffer.item(TID);};
	inline MyThread* operator[](unsigned long TID){wxMutexLocker ml(mtx); return buffer.item(TID);};
	
	//inline iterator begin() { return Items->begin(); };
	//inline iterator end() { return Items->end(); };

};

struct stDL{

	std::map<wxString,DevDesc*> *pDevList;
	std::map<wxString,LogDesc*> *pLogList;
	Vciml *pvciMailingList;
};
//WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);
typedef unsigned long DEVID;
/*
   class VecLogs{
   public:
   LogFile *get(long){return new LogFile};
   };
   */
extern "C" RexxExitHandler say_exit; 
class MyThread : public wxThread {
	wxDateTime dtCreated;
	unsigned long myID;
	std::multimap<wxString,unsigned long>::iterator itTList;
	public:
	//MyThread(MyFrame* parent, wxString* psScript, const wxString &name):wxThread(wxTHREAD_DETACHED), 
	MyThread(wxFrame* parent, wxString sScript, const wxString &name):wxThread(wxTHREAD_DETACHED), 
	m_parent(parent), sName(name), sRunningScript(sScript), fTerminated(false){
		dtCreated.SetToCurrent();
		logs = new vecid<LogDesc *>(0);
	}

	const wxDateTime GetCreation(){
		return dtCreated;
	};
	void setID(unsigned long);
	unsigned long getID(){return myID;};
	std::list<wxString*> lstMail;
	wxMutex mtxMail;
	wxSemaphore semMail;
	vecid<LogDesc*> *logs;
	virtual void OnExit();
	virtual ExitCode Entry();
	//MyFrame* m_parent;
	
	wxFrame* m_parent;
	wxString sName;
	wxString sRunningScript;
	bool fTerminated;
};

const wxString getNameByTID(unsigned long );
class ThreadMsg{
	wxString * stMessage;
	unsigned long ulMessage;
	public:
	enum{
		TEXT_OUT,
		ERROR_OUT,
		NEW_FRAME,
		TERMINATED
	};

	//ThreadMsg(unsigned long pTh): pthSpeaker(pTh), sName(pTh->sName){ stMessage = 0;};
	ThreadMsg(unsigned long pTh): stMessage(0), pthSpeaker(pTh), sName(getNameByTID(pTh)), action(TEXT_OUT){ };
	ThreadMsg(unsigned long pTh, const wxString &sMsg, int todo = TEXT_OUT): pthSpeaker(pTh), sName(getNameByTID(pTh)), action(todo){ stMessage = new wxString(sMsg);};
	ThreadMsg(unsigned long pTh, wxString*psMsg, int todo = TEXT_OUT): stMessage (psMsg), pthSpeaker(pTh),  sName(getNameByTID(pTh)), action(todo) {};
	ThreadMsg(unsigned long pTh, unsigned long ulMsg, int todo): stMessage(0), ulMessage( ulMsg), pthSpeaker(pTh),  sName(getNameByTID(pTh)), action(todo) {};
	~ThreadMsg(){ if (stMessage) delete stMessage;};
	wxString &GetStr(){return *stMessage;};
	unsigned long GetUL(){return ulMessage;};
	void SetStr(wxString *pstr){ if (stMessage) delete stMessage; stMessage = pstr;};
	void SetStr(const wxString &str){ if (stMessage) delete stMessage; stMessage = new wxString(str);};
	void SetUL(unsigned long ul){ ulMessage = ul;};
	const unsigned long pthSpeaker;
	const wxString sName;
	const int action;

};
struct NewTaskReq{
	wxSemaphore ready;
	wxString * const psTaskName;
	unsigned long  const owner;
	NewTaskReq(unsigned long  pth, wxString*str): psTaskName(str), owner(pth) {};
	~NewTaskReq(){ delete psTaskName;};
	unsigned long  TID;
};
// Define a new application type
