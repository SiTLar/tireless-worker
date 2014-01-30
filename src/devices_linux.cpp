#include <wx/wx.h>
#include <wx/string.h>
#include <map>
#include <string>
#include "interface.h"
#include "devfile.hpp"

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

	//Logging interfaces
	inp->mapLogs[std::string("FILE")] = new LogFile();
	inp->mapLogs[std::string("PLOT")] = new LogPlot();
	inp->mapLogsInfo[std::string("FILE")] = std::string("CVS-like logging");
	inp->mapLogsInfo[std::string("PLOT")] = std::string("Graphical output based on wxMathPlot");
}

