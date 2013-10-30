#ifndef __REXXSAA_H_INCLUDED 
#include "rexxsaa.h"
#endif

#include <map>
#include <string>
#include "devdecl.h"
#include "logger.h"

class DevInterface {
	public:
	std::string *strDescr;
	DevInterface() {};
	virtual ~DevInterface(){};
	virtual DevInterface* clone() = 0;
	virtual bool connect(const std::string& ) = 0;
	virtual std::string makeLock(const std::string& ) const = 0;
	virtual void disconnect() = 0;
	virtual bool attribute(Attr*) = 0;
	virtual bool write(const std::string&) = 0;  
	virtual bool read(std::string*, int) = 0;
};
struct HandlerLibInterface{
	std::map<std::string,std::string> mapDevsInfo;
	std::map<std::string,std::string> mapLogsInfo;
	std::map<std::string,DevInterface*> mapDevs;
	std::map<std::string,Logger *> mapLogs;
	std::string strDescr;
	virtual ~HandlerLibInterface(){};
};

