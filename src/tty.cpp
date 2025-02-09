extern "C"{
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
/*#include <libexplain/tcgetattr.h>
 */
#include <sys/ioctl.h>
}

#include <sstream>
#include "interface.h"
#include "tty.hpp"
std::string DevTTY::makeUniqueDev(const std::string& strInit) const {
	return makeBusLock(strInit);
}
std::string DevTTY::makeBusLock(const std::string& strInit) const{

	wxString sInit(strInit.c_str(), wxConvUTF8);
	wxRegEx reOptions(wxT("(\\w+)(?:::)?([0-9]+)?(?:::)?(\\w{3})?(?:::)?(SW|HW|NONE)?"),wxRE_ADVANCED + wxRE_ICASE  );
	wxASSERT_MSG(reOptions.IsValid(), wxT("RegEx failour"));
	if (!reOptions.Matches(sInit)) return std::string("BAD_INIT");
	return std::string (reOptions.GetMatch(sInit, 1).utf8_str());

};
bool DevTTY::connect(const std::string& strInit) {
	struct termios t = {0};
	unsigned long lBaud = B9600, lMode = CS8;
	wxString sInit(strInit.c_str(), wxConvUTF8);
	wxRegEx reOptions(wxT("(\\w+)(?:::)?([0-9]+)?(?:::)?(\\w{3})?(?:::)?(SW|HW|NONE)?"),wxRE_ADVANCED + wxRE_ICASE  );
	wxASSERT_MSG(reOptions.IsValid(), wxT("RegEx failour"));
	if (!reOptions.Matches(sInit)) return false;
	handle = open(wxString(wxT("/dev/")+reOptions.GetMatch(sInit, 1)).mb_str(wxConvUTF8), O_RDWR | O_NOCTTY|O_NDELAY| O_NONBLOCK );
	if (handle < 0 )return false;
	long flags = fcntl(handle, F_GETFL) & ~O_NONBLOCK ;
	fcntl(handle, F_SETFL, flags);
	if (!reOptions.GetMatch(sInit, 2).IsEmpty()){
		lBaud = canonizeBaud(wxString(reOptions.GetMatch(sInit, 2))); 
		if (lBaud < 0) return false;
	}
	if (!reOptions.GetMatch(sInit, 3).IsEmpty()){
		wxString sMode(reOptions.GetMatch(sInit, 3));
		long lSize;
		if (!(sMode.Mid(0,1).ToLong(&lSize))) return false;
		switch (lSize){case 8: lMode = CS8 ; break; case 7: lMode = CS7 ; break; case 6: lMode = CS6 ; break; case 5: lMode =CS5 ; break; default: return false;}
		//fprintf(stderr,"size:%lx\n", lMode);

		if (!(sMode.Mid(1,1).CmpNoCase(wxT("o")))){
			lMode |= PARENB;
			lMode |= PARODD;
			t.c_iflag |= INPCK;
		}else if (!(sMode.Mid(1,1).CmpNoCase(wxT("e")))){
			lMode |= PARENB;
			lMode &= ~PARODD;
			t.c_iflag |= INPCK;
		}else if (!(sMode.Mid(1,1).CmpNoCase(wxT("n")))){
			lMode &= ~PARENB;
			lMode &= ~PARODD;
			t.c_iflag &= ~INPCK;
		}else return false;
		if(sMode.Mid(2,1) == wxT("2")) lMode |= CSTOPB;
		else lMode &= ~CSTOPB;
	}
	/*if (tcgetattr(handle, &t) < 0 ){
		char message[3000];
		//explain_message_tcgetattr(message, sizeof(message),handle,&t );
		//wxASSERT_MSG(false, wxString(message , wxConvUTF8));
		return false;
	}
*/
		lMode &= ~(CRTSCTS);
	if (!reOptions.GetMatch(sInit, 4).IsEmpty()){
		wxString sMode(reOptions.GetMatch(sInit, 4));
		fprintf(stderr,"HW/SW:%s", sMode.utf8_str().data() );
		lMode &= ~(CRTSCTS);
		t.c_iflag &=  ~(IXON | IXOFF | IXANY);
		if(!sMode.CmpNoCase(wxT("hw")))
			lMode |= CRTSCTS;
		else if (!sMode.CmpNoCase(wxT("sw")))
			t.c_iflag |= IXANY;
	}
	t.c_cflag |=  lMode | CREAD |CLOCAL;
	t.c_lflag &= ~(ICANON|ECHO|ECHOE|ISIG);
	t.c_cc[VMIN] = 0;
	t.c_cc[VTIME] =0;
	t.c_oflag &= ~(OPOST|ONLCR);
	cfsetspeed(&t, lBaud);
	return tcsetattr(handle, TCSANOW, &t )<0 ? false:true ;
	//return setCBaud(this, lBaud);
};

void  DevTTY::disconnect() { close(handle);};
bool DevTTY::attribute(Attr* pAttrStr){

	do{
		std::string sAttr = std::string(pAttrStr->Attr_ID->strptr, pAttrStr->Attr_ID->strlength );
		std::string sVal = std::string(pAttrStr->Attr_VAL->strptr,pAttrStr->Attr_VAL->strlength) ;
		if (sAttr.empty() && sVal.empty())return false;
		std::map<std::string, bool (*) ( DevTTY*,const  std::string &)>::iterator it (mapAttr.find(sAttr));
		if(it == mapAttr.end()) return false;
		if(!it->second(this, sVal)) return false;
		pAttrStr = pAttrStr->next;
	}while(pAttrStr);
	
	return true;

};

bool DevTTY::write(const std::string& str){
	//int iState;
	/*
	ioctl(handle, TIOCMGET , &iState);
	iState |= TIOCM_DTR;
	ioctl(handle, TIOCMSET , &iState);
	*/
	bool rc = ::write(handle, str.c_str(), str.length()) < 0 ?false:true;
	rc = (rc && !(::write(handle, sTerm.c_str(), sTerm.length()) < 0) );
	/*
	ioctl(handle, TIOCMGET , &iState);
	iState &= ~TIOCM_DTR;
	ioctl(handle, TIOCMSET , &iState);
	*/
	return rc;
};  

bool DevTTY::read(std::string*str, std::string*err,int count) {
	if (count == 0) count = 1024;
	count = count<1024?count:1024;
	int iReadCount = 0;//, iTermLen =  sTerm.length(); 
	char buf[1024] = {0};
	//char *pcTerm = new char[iTermLen];
	/*
	ioctl(handle, TIOCMGET , &iState);
	iState |= TIOCM_DSR;
	ioctl(handle, TIOCMSET , &iState);
	*/
	//memmove(pcTerm ,sTerm.c_str() , iTermLen);	
	int re;
	while (((re = ::read( handle, buf + iReadCount, 1 )) > 0) && (count-- != 0)) 
		iReadCount += re;
	
	//	if(iReadCount >= iTermLen )if (!memcmp(pcTerm, buf + iReadCount - iTermLen, iTermLen))break;
	//fprintf(stderr, "iReadCount=%d, count=%d\n", iReadCount, count );
	for(int idx = 0;idx< iReadCount; idx++) fprintf(stderr, "%02x|",buf[idx]);
	if (re < 0) *err = std::string(strerror(errno));
	*str = std::string(buf, iReadCount);
	/*
	ioctl(handle, TIOCMGET , &iState);
	iState &= ~TIOCM_DSR;
	ioctl(handle, TIOCMSET , &iState);
	*/
	return true;
};
inline long DevTTY::canonizeBaud(const wxString& s){
	unsigned long lBaud, lInRate;
	if (!s.ToULong(&lInRate)) return -1;
	switch (lInRate) { case 38400: lBaud = B38400; break; case 19200: lBaud  = B19200; break; case 9600: lBaud  = B9600; break; case 4800: lBaud  = B4800; break; case 2400: lBaud  = B2400; break; case 1800: lBaud  = B1800; break; case 1200: lBaud  = B1200; break; case 600: lBaud  = B600; break; case 300: lBaud  = B300; break; case 200: lBaud  = B200; break; case 150: lBaud  = B150; break; case 134: lBaud  = B134; break; case 110: lBaud  = B110; break; case 75: lBaud  = B75; break; case 50: lBaud  = B50; break; case 57600: lBaud = B57600; break; case 115200: lBaud = B115200; break; case 230400: lBaud = B230400; break;default: return -1;  }
	return lBaud;
}	
 bool DevTTY::setBaud( DevTTY*o,const std::string& strInput){
	//DevTTY *o = dynamic_cast<DevTTY *>(d);
	//if(!o) return false;
	long lBaud;
	wxString sInput(strInput.c_str(), wxConvUTF8);
	if ((lBaud = o->canonizeBaud(sInput))< 0) return false;
	return DevTTY::setCBaud(o, lBaud);
}
 bool DevTTY::setCBaud( DevTTY *o,long lBaud){
	struct termios t;
	if (tcgetattr(o->handle,&t ) < 0){
		/*
		char message[3000];
		explain_message_tcgetattr(message, sizeof(message),o->handle,&t );
		wxASSERT_MSG(false, wxString(message , wxConvUTF8));
		*/
		return false;
	}
	cfsetspeed(&t, lBaud);
	return tcsetattr(o->handle, TCSANOW, &t )<0 ? false:true ;
};
 bool DevTTY::setDTR( DevTTY *o,const std::string& strInput){
 	int flag = TIOCM_DTR;
	if (!strcasecmp(strInput.c_str(), "0")
	|| !strcasecmp(strInput.c_str(), "NONE")
	|| !strcasecmp(strInput.c_str(), "FALSE")
	|| !strcasecmp(strInput.c_str(), "DOWN")
	|| !strcasecmp(strInput.c_str(), "LOW"))
		ioctl(o->handle, TIOCMBIC, &flag);	
	else if ( !strcasecmp(strInput.c_str(), "HIGH")
	|| !strcasecmp(strInput.c_str(), "UP")
	|| !strcasecmp(strInput.c_str(), "TRUE")
	|| !strcasecmp(strInput.c_str(), "1")
	|| !strcasecmp(strInput.c_str(), "RISED"))
		ioctl(o->handle, TIOCMBIS, &flag);	
	
	else return false;
	return true;
};
 bool DevTTY::setRTS( DevTTY *o,const std::string& strInput){
 	int flag = TIOCM_RTS;
	if (!strcasecmp(strInput.c_str(), "0")
	|| !strcasecmp(strInput.c_str(), "NONE")
	|| !strcasecmp(strInput.c_str(), "FALSE")
	|| !strcasecmp(strInput.c_str(), "DOWN")
	|| !strcasecmp(strInput.c_str(), "LOW"))
		ioctl(o->handle, TIOCMBIC, &flag);	
	else if ( !strcasecmp(strInput.c_str(), "HIGH")
	|| !strcasecmp(strInput.c_str(), "UP")
	|| !strcasecmp(strInput.c_str(), "TRUE")
	|| !strcasecmp(strInput.c_str(), "1")
	|| !strcasecmp(strInput.c_str(), "RISED"))
		ioctl(o->handle, TIOCMBIS, &flag);	
	
	else return false;
	return true;
};

 bool DevTTY::setDataBits( DevTTY *o,const std::string& strInput){
	//DevTTY *o = dynamic_cast<DevTTY *>(d);
	//if(!o) return false;
	struct termios t;
	long lSizeMask, lSize;
	std::stringstream input(strInput);
	input >> lSize;
	if (!input.good()) return false;
	switch (lSize){case 8: lSizeMask = CS8 ; break; case 7: lSizeMask = CS7 ; break; case 6: lSizeMask = CS6 ; break; case 5: lSizeMask =CS5 ; break; default: return false;}
	if (tcgetattr(o->handle,&t ) < 0) return false;
	t.c_cflag &= ~CSIZE;
	t.c_cflag |= lSizeMask;
	return tcsetattr(o->handle, TCSANOW, &t )<0 ? false:true ;
};

 bool DevTTY::setParity( DevTTY *o,const std::string& strInput){
	//DevTTY *o = dynamic_cast<DevTTY *>(d);
	//if(!o) return false;
	struct termios t;
	if (tcgetattr(o->handle,&t ) < 0) return false;
	if (!strcasecmp(strInput.c_str(), "odd")){
		t.c_cflag |= PARENB;
		t.c_cflag |= PARODD;
	}else if (!strcasecmp(strInput.c_str(), "even")){
		t.c_cflag |= PARENB;
		t.c_cflag &= ~PARODD;
	}else if (!strcasecmp(strInput.c_str(), "none")){
		t.c_cflag &= ~PARENB;
		t.c_cflag &= ~PARODD;
	}else return false;
	return tcsetattr(o->handle, TCSANOW, &t )<0 ? false:true ;
};
 bool DevTTY::setStopBits( DevTTY*o,const std::string& sInput){
	//DevTTY *o = dynamic_cast<DevTTY *>(d);
	//if(!o) return false;
	struct termios t;
	if (tcgetattr(o->handle,&t ) < 0) return false;
	if(sInput == "2") t.c_cflag |= CSTOPB;
	else t.c_cflag &= ~CSTOPB;
	return tcsetattr(o->handle, TCSANOW, &t )<0 ? false:true ;
};
/*
 bool DevTTY::setTimeout( DevTTY *o,const std::string& sInput){
	//DevTTY *o = dynamic_cast<DevTTY *>(d);
	//if(!o) return false;
	long lTimeout;
	struct termios t;
	std::stringstream input(sInput);
	input >> lTimeout;
	if (!input.good()) return false;
	if (tcgetattr(o->handle,&t ) < 0) return false;
	t.c_lflag &= ~(ICANON|ECHO);
	t.c_cc[VMIN] = 0;
	t.c_cc[VTIME] = lTimeout;
	return tcsetattr(o->handle, TCSANOW, &t )<0 ? false:true ;
};
*/
std::map<std::string, bool (*) ( DevTTY*, const std::string&), IcaseCmp > DevTTY::mapAttr;
DevTTY::DevTTY(): DevInterface(){
	mapAttr[std::string("BAUD_RATE")] =  &setBaud;
	mapAttr[std::string("SET_DTR")] =  &setDTR;
	mapAttr[std::string("SET_RTS")] =  &setRTS;
	mapAttr[std::string("STOP_BITS")] =  &setStopBits;
	mapAttr[std::string("PARITY")] =  &setParity;
	mapAttr[std::string("DATA_BITS")] =  &setDataBits;
	mapAttr[std::string("TIMEOUT")] =  reinterpret_cast<bool(*)(DevTTY*, const std::string&)>(&setTimeout);
	mapAttr[std::string("TERMINATOR")] = reinterpret_cast<bool(*)(DevTTY*, const std::string&)>(&setTerm);
}

