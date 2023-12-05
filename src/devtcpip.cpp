/*
extern "C" {
#define INCL_RXFUNC
#define INCL_RXSYSEXIT
#include "rexxsaa.h" 
#include "defines.h"
#include "rxpack.h"
}
*/
#include <cstdio>
#include <map>
#include <string>
#include <cstring>
#include <functional>
#include <wx/log.h>
#include <wx/dynlib.h>
#include "interface.h"
#include "devtcpip.hpp"
DevTCPIP::~DevTCPIP(){
	if (poSocket->IsConnected ())
		disconnect();
};
bool DevTCPIP::connect(const std::string& strInit) {
	bool rc;
	poSocket = new wxSocketClient(wxSOCKET_BLOCK);
	
	do{
		int iPort;
		std::string hostPart; 
		size_t colonPos = strInit.find(':');

		if(colonPos != std::string::npos) {
		     hostPart = strInit.substr(0,colonPos);
		     rc = sscanf(strInit.substr(colonPos+1).c_str(), "%d", &iPort);
		}else{
			rc = false;
			break;
		}

		if(oAddr.Hostname(wxString::FromAscii(hostPart.c_str()))&&oAddr.Service(iPort))
			rc = poSocket->Connect(oAddr);
		else rc = false ;
	}while(false);
	poSocket->Notify(false);
	//poSocket->SetFlags(wxSOCKET_BLOCK);
	//poSocket->SetTimeout(30);
	return rc;
};

std::string DevTCPIP::makeUniqueDev(const std::string& strInit) const {

	return strInit;
}
std::string DevTCPIP::makeBusLock(const std::string& strInit) const {
	return strInit;
}
std::string DevTCPIP::logError(	wxSocketError lastError ){

	switch(lastError){
		case wxSOCKET_INVOP:	
			return std::string("Invalid operation.");

		case wxSOCKET_IOERR:

			return std::string("Input/Output error.");
			break;
		case wxSOCKET_INVADDR:	
			return std::string("Invalid address passed to wxSocket.");
			break;

		case wxSOCKET_INVSOCK:	
			return std::string("Invalid socket (uninitialized).");
			break;

		case wxSOCKET_NOHOST :
			return std::string("No corresponding host.");
			break;
		case wxSOCKET_INVPORT :
			return std::string("Invalid port.");
			break;

		case wxSOCKET_WOULDBLOCK :
			//return std::string("The socket is non-blocking and the operation would block.");
			break;

		case wxSOCKET_TIMEDOUT:	
			return std::string("The timeout for this operation expired.");
			break;

		case wxSOCKET_MEMERR:
			return std::string("Memory exhausted.");
			break;
	}
	return std::string("No error.");
}
bool DevTCPIP::attribute(Attr* pAttrStr){

	do{
		std::string sAttr = std::string(pAttrStr->Attr_ID->strptr, pAttrStr->Attr_ID->strlength );
		std::string sVal = std::string(pAttrStr->Attr_VAL->strptr,pAttrStr->Attr_VAL->strlength) ;
		if (sAttr.empty() && sVal.empty())return false;
		std::map<std::string, bool (*) ( DevInterface*,const  std::string &),IcaseCmp>::iterator it (mapAttr.find(sAttr));
		if(it == mapAttr.end()) return false;
		if(!it->second(this, sVal)) return false;
		pAttrStr = pAttrStr->next;
	}while(pAttrStr);
	
	return true;

};

bool DevTCPIP::write(const std::string& str){
	poSocket->Write(str.c_str(), str.length());
	poSocket->Write(sTerm.c_str(), sTerm.length());
	wxSocketError lastError = poSocket->LastError();
	return !poSocket->Error() || (lastError == wxSOCKET_NOERROR ) || (poSocket->LastError() != wxSOCKET_WOULDBLOCK );

};  

bool DevTCPIP::read(std::string*str, std::string*err, int iToRead) {
	int iRed = 0;
	if(!poSocket->IsData()) return true;
	if (iToRead == 0) iToRead = 1024;
	iToRead = iToRead<1024?iToRead:1024;

	unsigned char buf[1024] = {0};
	poSocket->Read(buf, iToRead );
	iRed = poSocket->LastCount();
	if(poSocket->Error()){
		wxSocketError lastError = poSocket->LastError();
		if((lastError != wxSOCKET_NOERROR ) &&(lastError != wxSOCKET_WOULDBLOCK )){
			*err = logError(lastError);
			return false;
		}
	}
	*str = std::string(reinterpret_cast<char *>(buf),iRed);
	return true;
};
std::map<std::string, bool (*) ( DevInterface*, const std::string&), IcaseCmp > DevTCPIP::mapAttr;
DevTCPIP::DevTCPIP():DevInterface(),poSocket(NULL){
	sTerm = "\x0d\x0a";
	mapAttr[std::string("SET_TERM")] = &setTerm;
	mapAttr[std::string("SET_TIMEOUT")] = &setTimeout;
};

