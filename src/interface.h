#ifndef __REXXSAA_H_INCLUDED 
#include "rexxsaa.h"
#endif

#include <map>
#include <string>
#include "devdecl.h"
#include "logger.h"

#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>


class DevInterface {
	std::string ToHex(const std::string& s, bool upper_case /* = true */) {
		std::ostringstream ret;

		for (std::string::size_type i = 0; i < s.length(); ++i)
			ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << (int)s[i];

		return ret.str();
	}
	public:
	std::string *strDescr;
	std::string sTerm;
	long lReadTimeout = 500;
	inline long readTimeout(){return lReadTimeout;};
	static bool setTerm ( DevInterface*o, const std::string& str){
		o->sTerm = str;
		std::cerr<<"New Term"<<o->ToHex(o->sTerm, false)<< "\n";

		return true;
	};
	static bool setTimeout ( DevInterface*o, const std::string&str) {
		double dTime;
		if (sscanf(str.c_str(), "%lf", &dTime) != 1) return false;
		o->lReadTimeout = dTime*1000;
		return true;
	};
	DevInterface():sTerm("\n") {};
	virtual ~DevInterface(){};
	virtual DevInterface* clone() = 0;
	virtual bool connect(const std::string& ) = 0;
	virtual std::string makeBusLock(const std::string& ) const = 0;
	virtual std::string makeUniqueDev(const std::string& ) const = 0;
	virtual void disconnect() = 0;
	virtual bool attribute(Attr*) = 0;
	virtual bool write(const std::string&) = 0;  
	virtual bool read(std::string*, std::string*, int) = 0;
};
struct HandlerLibInterface{
	std::map<std::string,std::string> mapDevsInfo;
	std::map<std::string,std::string> mapLogsInfo;
	std::map<std::string,DevInterface*> mapDevs;
	std::map<std::string,Logger *> mapLogs;
	std::string strDescr;
	virtual ~HandlerLibInterface(){};
};

