/*
#include <wx/wx.h>
#include <wx/string.h>
*/

#include <map>
#include <string>
#include "interface.h"
#include "devvisa.hpp"

#include <sstream>
#include <ctime>
extern "C"{
void dynLoad(HandlerLibInterface* inp ){
	fnGenDevs( inp );
	inp->strDescr = "NI VISA bindings";
	return;
}
}

void fnGenDevs(HandlerLibInterface* inp ){
	//Device interfaces
	inp->mapDevs[std::string("VISA")] = new DevVisa();
	inp->mapDevsInfo[std::string("VISA")] = std::string("VISA interface handler");

	//Logging interfaces
}

