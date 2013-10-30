
#include <wx/wx.h>
#include <wx/string.h>

extern "C" {
#define INCL_RXFUNC
#define INCL_RXSYSEXIT
#include "rexxsaa.h" 
#include "defines.h"
#include "rxpack.h"
}
#include <map>
#include <string>
#include "interface.h"
#include "logplot.hpp"
//#include "devlcard.hpp"
#include "com.hpp"


extern "C"{
#include <stdio.h>
#include <string.h>
//#include "rexxsaa.h"
}
std::map<std::string,DevInterface*> *pDevList;
std::map<std::string,Logger*> *pLogList;
//Vciml *pvciMailingList;
extern "C"{
//void dynLoad(stDL *input){
void dynLoad(std::map<std::string,DevInterface*> *inpDev, std::map<std::string,Logger*> *inpLog ){
	pDevList = inpDev;
	pLogList = inpLog;
	fnGenDevs();
	return;
}
}
void fnGenDevs(void){
		(*pDevList)[std::string("SERIAL")] = new DevSerial();
		(*pDevList)[std::string("NULL")] = new DevSerial();
		//(*pDevList)[std::string("LCARD")] = new DevLCard();
		(*pLogList)[std::string("PLOT")] = new LogPlot();

}

