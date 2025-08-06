/*
#include <wx/wx.h>
#include <wx/string.h>
*/

#include <map>
#include <string>
#include "interface.h"
#include "devlcard.hpp"
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
	inp->mapDevs[std::string("LCARD")] = new DevLCard();
	inp->mapDevsInfo[std::string("LCARD")] = std::string("Experimental LCARD interface");

	//Logging interfaces
}

