extern "C"{
#include <stdio.h>
#include <string.h>
//#include "rexxsaa.h"
}
#include <wx/process.h>
#include <wx/wx.h>
#include <wx/socket.h>
#include "logplotapi.h"
#include "fxyseq.h"



class LogProcess: public wxProcess {
	LogProcess*&myhome;
	public:
	LogProcess(LogProcess*&_myhome):myhome(_myhome) {
		Redirect();
	};

	//virtual void OnTerminate(int pid, int status){
	virtual void OnTerminate(int , int ){
		//wxLogStatus(m_parent, _T("Process %u terminated with exit code %d."), pid,  status);

		// we're not needed any more
		delete this;
	}
	~LogProcess(){myhome = 0;};

};


class LogPlot: public Logger{

	LogPlot();
	friend void fnGenDevs(HandlerLibInterface*);
	wxDateTime dtStart;
	wxDatagramSocket  *pUdpSocket;
	wxIPV4address server;
	//std::vector<std::string> *splitString(const std::string& sInput, char token);
	static int setLabel (Logger *l,const std::string& );
	static int setColour (Logger *l,const std::string& );
	static int attach (Logger *l,const std::string& );
	public:
	virtual bool init(const std::string &); 
	virtual bool OK(){return true;};
	bool send(wxChar*data, int size);
	virtual Logger * clone() {return new LogPlot(*this);};

	virtual ~LogPlot();
	virtual int write(const std::string& sData);
};
