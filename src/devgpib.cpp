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
#include <wx/dynlib.h>
#include "interface.h"
#include "devgpib.hpp"
int __stdcall (*_ibdev) (int boardID, int pad, int sad, int tmo, int eot, int eos);
unsigned long __stdcall  (*_ibwrt)(int ud, const void * buf, size_t cnt);
unsigned long __stdcall  (*_ibrd)(int ud, void * buf, size_t cnt);
unsigned long __stdcall (*_ibonl)(int ud, int v);
unsigned long __stdcall (*_ThreadIbsta) (void);
unsigned long __stdcall (*_ThreadIberr) (void);
unsigned long __stdcall (*_ThreadIbcnt) (void);
DevGPIB::~DevGPIB(){disconnect();};
bool DevGPIB::connect(const std::string& strInit) {
	int boardno;
	int pad;
	int sad;
	int timeout;
	int eot;
	int eos;
	long rc;
	do{
		rc = std::sscanf(strInit.c_str(), "%d::%d::%d::%d::%d::%d", boardno,  pad,  sad,  timeout,  eot,  eos);
		if (rc == EOF) break;
		handle = _ibdev( boardno,  pad,  sad,  timeout,  eot,  eos);
		if ( (_ThreadIbsta()&ERR) &&( EDVR == _ThreadIberr()))
			rc = EOF;
	}while(false);
	return ( rc !=  EOF )?true:false;
};

std::string DevGPIB::makeUniqueDev(const std::string& strInit) const {

	return strInit;
}
std::string DevGPIB::makeBusLock(const std::string& strInit) const {
	size_t uiInitPos = strInit.find("::");
	if (uiInitPos == std::string::npos) return std::string("BAD_INIT");

	return strInit.substr(0, uiInitPos );
}
bool DevGPIB::attribute(Attr* pAttrStr){

	do{
		std::string strAttrN(pAttrStr->Attr_ID->strptr, pAttrStr->Attr_ID->strlength );
		std::map<std::string,void(*)(int,long) >::iterator itAttrName;
		itAttrName = mapAttr.find(strAttrN);
		if(itAttrName == mapAttr.end()) return false;

		unsigned long ulAttrVal;
		if (!std::sscanf(std::string(pAttrStr->Attr_VAL->strptr,pAttrStr->Attr_VAL->strlength).c_str(),"%lu", &ulAttrVal))
			return false;
		(itAttrName->second)(handle,ulAttrVal); 
		if(_ThreadIbsta()&ERR)
			return false ;
		
		pAttrStr = pAttrStr->next;
	}while(pAttrStr);
	return true;

};

bool DevGPIB::write(const std::string& str){
	unsigned char *buf = new unsigned char[str.length()];
	memmove(buf, str.c_str(), str.length());

	_ibwrt(handle, buf, str.length());
	delete[] buf;
	return _ThreadIbsta()&ERR?false:true;

};  

bool DevGPIB::read(std::string*str, int count) {

	if (count == 0) count = 1024;
	count = count<1024?count:1024;
	unsigned char buf[1024] = {0};
	_ibrd(handle, buf, count) ;
	if ( _ThreadIbsta()&ERR)
		return false;

	*str = std::string(reinterpret_cast<char *>(buf), _ThreadIbcnt());
	return true;
};


void f0(int handle,long param){
//_ibeos(handle, param);

}; 
std::map<std::string, void(*)(int,long)> DevGPIB::mapAttr;
DevGPIB::DevGPIB():DevInterface(){
	
	wxDynamicLibrary dllDev;
	dllDev.Load(wxT("ni4882.dll"));
	_ibdev = reinterpret_cast<int __stdcall(*)(int,int,int,int,int,int)>(dllDev.GetSymbol(wxT("ibdev")));
	_ibwrt = reinterpret_cast<unsigned long __stdcall(*) (int , const void * , size_t)>(dllDev.GetSymbol(wxT("ibwrt")));
	_ibrd = reinterpret_cast<unsigned long  __stdcall(*)(int , void * , size_t)>(dllDev.GetSymbol(wxT("ibrd")));
	_ibonl = reinterpret_cast<unsigned long  __stdcall(*)(int , int)>(dllDev.GetSymbol(wxT("ibrd")));
	_ThreadIbsta = reinterpret_cast<unsigned long __stdcall(*)(void)>(dllDev.GetSymbol(wxT("ThreadIbsta")));
	_ThreadIberr = reinterpret_cast<unsigned long __stdcall(*)(void)>(dllDev.GetSymbol(wxT("ThreadIberr")));
	_ThreadIbcnt = reinterpret_cast<unsigned long __stdcall(*)(void)>(dllDev.GetSymbol(wxT("ThreadIbcnt")));
	mapAttr[std::string("ibeos")] =&f0; ;
//	std::unary_fanction<long, void>{ void operator()(long param){ibeos(handle, param); }; };
	//mapAttr[std::string("")] = void(long param){(handle, param)};
};

