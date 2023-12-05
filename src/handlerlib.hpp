#ifndef __handlerlib__
#define __handlerlib__
#include <wx/dynlib.h>
#include <wx/thread.h>
#include <wx/string.h> 
class DevDesc;
class LogDesc;
struct HandlerLibData{
	//std::map<std::string,DevInterface*> mapDevs;
	//std::map<std::string,Logger *> mapLogs;
	wxDllType dllHandle;
	wxString sName;
	wxString md5;
	wxString sPath;
	wxString sDescr;
	wxMutex *pMtxCount;
	HandlerLibInterface HLI;
	HandlerLibData(wxString const & _md5): md5(_md5){};
	HandlerLibData(wxString const & _md5, wxString const & _sName,wxString const & _sPath, 
			HandlerLibInterface&, wxDllType _dllHandle);	
	inline bool operator<(HandlerLibData const & rhl)const {return md5<rhl.md5;}
	int *pInstances;
	inline void operator++(){inc();};
	inline void operator--(){dec();};
	enum {
		UNFEDINED = 0,
		DEVICE = 1,
		LOGGER = 2
	};

	void inc();
	void dec();
};
#endif
