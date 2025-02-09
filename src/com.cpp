#include <wx/wx.h>
#include <wx/string.h>
#include <wx/regex.h>
#include <wx/log.h>
extern "C"{
#include <stdio.h>
#include <string.h>
#include <rexxsaa.h>
}
//#include <wx/thread.h>

#include <map>
#include <string>
#include "interface.h"
#include "com.hpp"
extern "C"{
#include <errno.h>
#include <fcntl.h>
#include <windows.h>
#include <commctrl.h>
}
std::string DevSerial::makeUniqueDev(const std::string& strInit) const {
	return makeBusLock(strInit);
}
std::string DevSerial::makeBusLock(const std::string& strInit) const{
	std::string strRet;
	std::vector< std::string> vParam ;
	if (splitString(strInit, ':', vParam))strRet = vParam[0];
	else strRet = "BAD_INIT";
	return strRet;
};

bool DevSerial::connect(const std::string& strInit) {
	DCB dcb;
	int iStage = 0;
	
	memset(&dcb, 0, sizeof(dcb));
	long lBaud = CBR_9600;
	DWORD fRtsControl = RTS_CONTROL_DISABLE, fDtrControl = DTR_CONTROL_DISABLE, fOutxCtsFlow = false, fOutxDsrFlow = false, fOutX = false, fInX = false;
	BYTE cByteSize = 8, cParity = NOPARITY, cStopBits = ONESTOPBIT;
	//wxString sFname = strInit.Mid(6);
	wxLogDebug( wxT("COM stage #%d starting com. strInit=%s"), iStage++,wxString(strInit.c_str(), wxConvUTF8));//0
	std::vector< std::string> vParam ;
	//wxRegEx reOptions(wxT("(\\w+)(?:::)?([0-9]+)?(?:::)?(\\w{3})?(?:::)?(SW|RTS|DTR|NONE)?"),wxRE_ADVANCED + wxRE_ICASE  );
	//if (!reOptions.Matches(strInit)) return false;
	if (!splitString(strInit, ':', vParam )) return false;
	wxLogDebug( wxString::Format(wxT("COM stage #%d"), iStage++));
	//handle = CreateFile(wxString(wxT("\\\\.\\")+reOptions.GetMatch(strInit, 1)).wc_str() , GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );
	handle = CreateFileA(std::string(std::string("\\\\.\\")+vParam[0]).c_str() , GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );

	wxLogDebug(wxString(std::string(std::string("\\\\.\\")+vParam[0]).c_str(), wxConvUTF8));
	wxLogDebug( wxT("COM stage #%d"), iStage++);
	if (handle == INVALID_HANDLE_VALUE) return false;
	GetCommTimeouts(handle, &initTimeouts);
	wxLogDebug( wxString::Format(wxT("COM stage #%d\nhandle opened"), iStage++));
	//if (!reOptions.GetMatch(strInit, 2).IsEmpty()){
	if(vParam.size() > 1){
		//lBaud = canonizeBaud(wxString(reOptions.GetMatch(strInit, 2))); 
		lBaud = canonizeBaud(wxString(vParam[1].c_str(), wxConvUTF8)); 
		if (lBaud < 0){
			PurgeComm(handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
			CloseHandle(handle);	
			return false;
		};

	}

	wxLogDebug( wxT("COM stage #%d Baud OK"), iStage++);
	//if (!reOptions.GetMatch(strInit, 3).IsEmpty()){
	if(vParam.size() > 2){
		//wxString sMode(reOptions.GetMatch(strInit, 3));
		wxString sMode(wxString(vParam[2].c_str(), wxConvUTF8)); 
		long lSize;
		if (!(sMode.Mid(0,1).ToLong(&lSize))) {
			PurgeComm(handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
			CloseHandle(handle);	
			return false;
		};
		if((lSize<5)|(lSize>8))  {
			PurgeComm(handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
			CloseHandle(handle);	
			return false;
		};
		wxLogDebug( wxString::Format(wxT("COM stage #%d Data Bits OK"), iStage++));
		cByteSize = (BYTE)lSize;
		if (!(sMode.Mid(1,1).CmpNoCase(wxT("o")))) cParity = ODDPARITY;
		else if (!(sMode.Mid(1,1).CmpNoCase(wxT("e")))) cParity = EVENPARITY;
		else if (!(sMode.Mid(1,1).CmpNoCase(wxT("s")))) cParity = SPACEPARITY;
		else if (!(sMode.Mid(1,1).CmpNoCase(wxT("m")))) cParity = MARKPARITY;
		else if (!(sMode.Mid(1,1).CmpNoCase(wxT("n")))) cParity = NOPARITY;
		else return false;
		wxLogDebug( wxString::Format(wxT("COM stage #%d Parity OK"), iStage++));
		if(sMode.Mid(2,1) == wxT("2")) cStopBits = TWOSTOPBITS;
		else cStopBits = ONESTOPBIT;
		if(vParam.size() > 3){

			//wxLogDebug( wxString::Format(wxT("COM stage #%d Flow control=%s"), iStage++, reOptions.GetMatch(strInit, 4) ));
			wxLogDebug( wxString::Format(wxT("COM stage #%d Flow control=%s"), iStage++, wxString(vParam[3].c_str(), wxConvUTF8))); 
			//wxString sFlowControll(reOptions.GetMatch(strInit, 4));
			wxString sFlowControll( wxString(vParam[3].c_str(), wxConvUTF8));
			if (!sFlowControll.CmpNoCase(wxT("RTS"))){
				fRtsControl = RTS_CONTROL_ENABLE;
				fDtrControl = DTR_CONTROL_DISABLE;
				fOutxCtsFlow = true;
				fOutxDsrFlow = false;
				fOutX = false;
				fInX = false;
			}else if (!sFlowControll.CmpNoCase(wxT("DTR"))){
				fRtsControl = RTS_CONTROL_DISABLE;
				fDtrControl = DTR_CONTROL_ENABLE;
				fOutxCtsFlow = false;
				fOutxDsrFlow = true;
				fOutX = false;
				fInX = false;
			}else if (!sFlowControll.CmpNoCase(wxT("SW"))){
				fRtsControl = RTS_CONTROL_DISABLE;
				fDtrControl = DTR_CONTROL_DISABLE;
				fOutxCtsFlow = false;
				fOutxDsrFlow = false;
				fOutX = true;
				fInX = true;
			}else if (!sFlowControll.CmpNoCase(wxT("NONE"))){
				fRtsControl = RTS_CONTROL_DISABLE;
				fDtrControl = DTR_CONTROL_DISABLE;
				fOutxCtsFlow = false;
				fOutxDsrFlow = false;
				fOutX = false;
				fInX = false;
			} else return false;

		}
	}
	if(!GetCommState(handle, &dcb)) {
		PurgeComm(handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(handle);	
		return false;
	};

	dcb.BaudRate = lBaud;
	dcb.fRtsControl = fRtsControl;
	dcb.fDtrControl = fDtrControl;
	dcb.fOutxCtsFlow = fOutxCtsFlow;
	dcb.fOutxDsrFlow = fOutxDsrFlow;
	dcb.fOutX = fOutX;
	dcb.fInX = fInX;
	dcb.ByteSize = cByteSize;
	dcb.Parity = cParity;
	dcb.StopBits = cStopBits;
	if(!SetCommState(handle, &dcb)) {
		PurgeComm(handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(handle);	
		return false;
	};

	wxLogDebug( wxString::Format(wxT("COM stage #%d SetCommState OK"), iStage++));
	COMMTIMEOUTS timeouts = {0};

	timeouts.ReadIntervalTimeout = 0; 
	timeouts.ReadTotalTimeoutMultiplier = 0; 
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutConstant = 200; 
	if(!SetCommTimeouts(handle, &timeouts)) {
		PurgeComm(handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(handle);	
		return false;
	};
	wxLogDebug( wxString::Format(wxT("COM stage #%d TimeOUT OK"), iStage++));
	GetCommTimeouts(handle, &timeouts);
	wxLogDebug( wxString::Format(wxT("COM stage #%d GetCommTimeouts OK. RI=%d, WT=%d"), iStage++, timeouts.ReadIntervalTimeout, timeouts.WriteTotalTimeoutConstant ));
	return true;
}
bool DevSerial::attribute(Attr* pAttrStr){

	do{
		std::string sAttr = std::string(pAttrStr->Attr_ID->strptr, pAttrStr->Attr_ID->strlength );
		std::string sVal = std::string(pAttrStr->Attr_VAL->strptr,pAttrStr->Attr_VAL->strlength) ;
		if (sAttr.empty() && sVal.empty())return false;
		std::map<std::string, bool (*) ( DevSerial*,const  wxString &), IcaseCmp>::iterator it (mapAttr.find(sAttr));
		if(it == mapAttr.end()) return false;
		wxString wxsVal = wxString::FromUTF8(pAttrStr->Attr_VAL->strptr,pAttrStr->Attr_VAL->strlength) ;
		if(!it->second(this, wxsVal)) return false;
		pAttrStr = pAttrStr->next;
	}while(pAttrStr);
	
	return true;

};

bool DevSerial::write(const std::string& str){
	//int iState;
	DWORD lBytsWritten;
	bool rc = WriteFile(handle, str.c_str(), str.length(), &lBytsWritten, NULL) ;
	if(!sTerm.IsEmpty()) rc = (rc && WriteFile(handle, sTerm.utf8_str(), sTerm.Len(), &lBytsWritten, NULL));
	return rc;
};  

bool DevSerial::read(std::string*str, int count) {
	if (count == 0) count = 1024;
	count = count<1024?count:1024;
	int iReadCount = 0, iTermLen =  strlen(sOTerm.mb_str()); 
	char buf[1024] = {0};
	char *pcTerm = new char[iTermLen];
	memmove(pcTerm ,sOTerm.mb_str() , iTermLen);	
	DWORD re = 0;
	wxLogDebug( wxT("COM preread re=%d; iReadCount=%d; count=%d; char=%x;"), re, iReadCount,  count,  buf[iReadCount]);
	while (ReadFile( handle, buf + iReadCount++, 1, &re, NULL )){
		wxLogDebug( wxT("COM read re=%d; iReadCount=%d; count=%d; char=%x;"), re, iReadCount,  count,  buf[iReadCount-1]);
		if((re == 0 )|| (count-- == 0)) break;
		if(iReadCount >= iTermLen )if (!memcmp(pcTerm, buf + iReadCount - iTermLen, iTermLen)) break;
		
	}
	//fprintf(stderr, "iReadCount=%d, count=%d\n", iReadCount, count );
	//for(int idx = 0;idx< iReadCount; idx++) if(buf[idx])fprintf(stderr, "%2x",buf[idx]);

	if((iReadCount - iTermLen) > 0)
		*str = std::string(buf, iReadCount - iTermLen);
	return true;
};
inline long DevSerial::canonizeBaud(wxString s){
	unsigned long lBaud, lInRate;
	if (!s.ToULong(&lInRate)) return -1;
	switch (lInRate) { case 110: case 300: case 600: case 1200: case 2400: case 4800: case 9600: case 14400: case 19200: case 38400: case 56000: case 57600: case 115200: case 128000: case 256000: lBaud = lInRate; break;
		default: return -1;
	}
	return lBaud;
}	
static bool DevSerial::setBaud( DevSerial *o,const wxString& sInput){
	//DevSerial *o = dynamic_cast<DevSerial *>(d);
	//if(!o) return false;
	DCB dcb;
	if(!GetCommState(o->handle, &dcb)) {
		PurgeComm(o->handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(o->handle);	
		return false;
	};

	long lBaud;
	if ((lBaud = o->canonizeBaud(sInput))< 0) return false;
	dcb.BaudRate = lBaud;
	if(!SetCommState(o->handle, &dcb)) {
		PurgeComm(o->handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(o->handle);	
		return false;
	};

	return true ;
};


static bool DevSerial::setDataBits( DevSerial *o,const wxString& sInput){
	//DevSerial *o = dynamic_cast<DevSerial *>(d);
	//if(!o) return false;
	long  lSize;
	if (!sInput.ToLong(&lSize)?true:((lSize<5 )|| (lSize>8))) {
		PurgeComm(o->handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(o->handle);	
		return false;
	}
	DCB dcb;
	if(!GetCommState(o->handle, &dcb)) {
		PurgeComm(o->handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(o->handle);	
		return false;
	};


	dcb.ByteSize = (BYTE)lSize;
	if(!SetCommState(o->handle, &dcb)) {
		PurgeComm(o->handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(o->handle);	
		return false;
	};

	return true ;
};

static bool DevSerial::setParity( DevSerial *o,const wxString& sInput){
	//DevSerial *o = dynamic_cast<DevSerial *>(d);
	//if(!o) return false;
	DCB dcb;
	if(!GetCommState(o->handle, &dcb)) {
		PurgeComm(o->handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(o->handle);	
		return false;
	};

	if (!sInput.CmpNoCase(wxT("odd"))) dcb.Parity = ODDPARITY;
	else if (!sInput.CmpNoCase(wxT("even"))) dcb.Parity = EVENPARITY;
	else if (!sInput.CmpNoCase(wxT("space"))) dcb.Parity = SPACEPARITY;
	else if (!sInput.CmpNoCase(wxT("mark"))) dcb.Parity = MARKPARITY;
	else if (!sInput.CmpNoCase(wxT("none"))) dcb.Parity = NOPARITY;
	else {
		PurgeComm(o->handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(o->handle);	
		return false;
	};
	if(!SetCommState(o->handle, &dcb)) {
		PurgeComm(o->handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(o->handle);	
		return false;
	};

	return true ;

};
static bool DevSerial::setStopBits( DevSerial *o,const wxString& sInput){
	//DevSerial *o = dynamic_cast<DevSerial *>(d);
	//if(!o) return false;
	DCB dcb;
	if(!GetCommState(o->handle, &dcb)) {
		PurgeComm(o->handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(o->handle);	
		return false;
	};

	if(sInput == wxT("2")) dcb.StopBits = TWOSTOPBITS;
	else dcb.StopBits = ONESTOPBIT;
	if(!SetCommState(o->handle, &dcb)) {
		PurgeComm(o->handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle(o->handle);	
		return false;
	};

	return true ;

};
static bool DevSerial::setNoTerm(DevSerial *o, const wxString &){
	o->sTerm = wxEmptyString;
	setOTerm(o, wxEmptyString);
	return true;

};
static bool DevSerial::setTerm(DevSerial *o, const wxString &str){
	o->sTerm = str;
	setOTerm(o, str);
	return true;

};

static bool DevSerial::setOTerm(DevSerial *o, const wxString &str){
	o->sOTerm = str;
	return true;

};
static bool DevSerial::setReadTimeout(DevSerial *o, const wxString &str){
	COMMTIMEOUTS timeouts = {0};
	unsigned long ulTimeout;
	if (!str.ToULong(&ulTimeout)) return false;
	if(!GetCommTimeouts(o->handle, &timeouts)) return false;
	timeouts.ReadTotalTimeoutConstant = ulTimeout; 
	return SetCommTimeouts(o->handle, &timeouts);
}
static bool DevSerial::setWriteTimeout(DevSerial *o, const wxString &str){
	COMMTIMEOUTS timeouts = {0};
	unsigned long ulTimeout;
	if (!str.ToULong(&ulTimeout)) return false;
	if(!GetCommTimeouts(o->handle, &timeouts)) return false;
	timeouts.WriteTotalTimeoutConstant = ulTimeout; 
	return SetCommTimeouts(o->handle, &timeouts);
}
std::map<std::string, bool (*) ( DevSerial*, const wxString&), IcaseCmp > DevSerial::mapAttr;
DevSerial::DevSerial(): DevInterface(), handle(INVALID_HANDLE_VALUE){
 sTerm=(wxString(wxT("\x0d\x0a"))); sOTerm = (wxString(wxT("\x0d\x0a")));
	mapAttr[std::string("BAUD_RATE")] =  &setBaud;
	mapAttr[std::string("STOP_BITS")] =  &setStopBits;
	mapAttr[std::string("PARITY")] =  &setParity;
	mapAttr[std::string("DATA_BITS")] =  &setDataBits;
	mapAttr[std::string("TERMINATOR")] =  &setTerm;
	mapAttr[std::string("NO_TERMINATOR")] =  &setNoTerm;
	mapAttr[std::string("OUT_TERMINATOR")] =  &setOTerm;
	mapAttr[std::string("WRITE_TIMEOUT")] =  &setWriteTimeout;
	mapAttr[std::string("READ_TIMEOUT")] =  &setReadTimeout;
	mapAttr[std::string("TIMEOUT")] =  &setReadTimeout;
}

