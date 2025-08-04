#include <wx/wx.h>
#include <wx/string.h>
#include <map>
#include <string>
#include "interface.h"
#include "devfile.hpp"
#include "devtcpip.hpp"
#include "devgpib.hpp"

#include <sstream>
#include <ctime>
#include "logfile.hpp"
#include "logplot.hpp"
#include "tty.hpp"
extern "C"{
void dynLoad(HandlerLibInterface* inp ){
	fnGenDevs( inp );
	inp->strDescr = "A basic library with basic handler implementations";
	return;
}
}

void fnGenDevs(HandlerLibInterface* inp ){
	//Device interfaces
	inp->mapDevs[std::string("FILE")] = new DevFile();
	inp->mapDevs[std::string("SERIAL")] = new DevTTY();
	inp->mapDevsInfo[std::string("FILE")] = std::string("Basic file IO");
	inp->mapDevsInfo[std::string("SERIAL")] = std::string("RS235 interface handler");
	inp->mapDevs[std::string("TCPIP")] = new DevTCPIP();
	inp->mapDevsInfo[std::string("TCPIP")] = std::string("Raw TCP/IP");
	inp->mapDevs[std::string("IEEE488")] = new DevGPIB();
	inp->mapDevsInfo[std::string("IEEE488")] = std::string("GPIB interface");

	//Logging interfaces
	inp->mapLogs[std::string("FILE")] = new LogFile();
	inp->mapLogs[std::string("PLOT")] = new LogPlot();
	inp->mapLogsInfo[std::string("FILE")] = std::string("CVS-like logging");
	inp->mapLogsInfo[std::string("PLOT")] = std::string("Graphical output based on wxMathPlot");
}

bool splitString(const std::string& strInput, char token, std::vector<std::string>& out){
	int start = 0,  stop = 0;
	stop = strInput.find(token, start);
	if( std::string::npos == stop) return false;
	do{
		stop = strInput.find(token, start);
		if(stop != start){
			std::string strPart(strInput.substr(start, stop - start ));
			if (!strPart.empty()) out.push_back(strPart);
		}
		start = stop+1;
	}while ( std::string::npos != stop );
	return true;

}
