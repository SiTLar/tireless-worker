#include <wx/wx.h>
extern "C"{
#include <stdio.h>
#include <string.h>
//#include "rexxsaa.h"
}

#include <map>
#include <string>
#include <sstream>
#include <wx/msgdlg.h>
#include <wx/socket.h>
#include "interface.h"
#include "logplot.hpp"
#include "tools.hpp"
//#include "execreq.h"

bool LogPlot::init(const std::string &str) {
	//unsigned long ulLocalId;
	wxString sColour, sLabel;
	//, sParam(str.c_str(), wxConvUTF8);
	unsigned long attachT = myTID, attachI = myId;
	bool rc = false;
	std::vector< std::string> vParam ;
	char pcLabel[256] = {0}, pcColour[256] = {0};
	dtStart.SetToCurrent();
	wxIPV4address client;
	client.AnyAddress();
	client.Service(0);
	pUdpSocket = new wxDatagramSocket(client, wxSOCKET_BLOCK );

	/*
	   wxLogMessage(sParam);
	   wxString sRegEx(wxT(":([[:alnum:]_ ]*)?(?:::)?([[:alnum:]_]*)?(?:::)?([0-9]*)?(?:::)?([0-9]+)?"));
	   wxLogMessage(sRegEx);
	   wxRegEx reParam(sRegEx , wxRE_ADVANCED + wxRE_ICASE);
	   if (reParam.IsValid()){
	   wxMessageBox(wxT("Bad RegEx"));
	   return false;
	   }
	   */
	if (splitString(str, ':', vParam )){
		switch(vParam.size()){
			case 4:
				//reParam.GetMatch(sParam, 4).ToULong(&attachT);
				wxString(vParam[3].c_str(), wxConvUTF8).ToULong(&attachT);
				//wxLogMessage(wxT("attachT=%d"), attachT);
			case 3:
				wxString(vParam[2].c_str(), wxConvUTF8).ToULong(&attachI);
				//wxLogMessage(wxT("attachI=%d"), attachI);
				//	if (reParam.GetMatch(sParam, 3).ToULong(&attachI))
				{
					LPAttach data;
					data.cCommand = 'A';
					data.ulTID = myTID;
					data.ulLID = myId;
					data.ulTIDatt = attachT;
					data.ulLIDatt = attachI;
					rc |=send((wxChar*)&data, sizeof(data));
				}

			case 2:
				//wxLogMessage(wxT("Colour=%s"), vParam[1].c_str() );
				//	sColour = reParam.GetMatch(sParam, 2);
				if(vParam[1].length()){
					LPString data;
					data.cCommand = 'O';
					data.ulTID = myTID;
					data.ulLID = myId;
					data.length = vParam[1].length()< 255?vParam[1].length():255; 
					memmove(data.str, vParam[1].c_str(), data.length);
					rc |=send((wxChar*)&data, sizeof(data));
				}

			case 1:
				//wxLogMessage(wxT("Label=%s"), vParam[0].c_str() );
				//sLabel = reParam.GetMatch(sParam, 1);
				if(vParam[0].length()){
					LPString data;
					data.cCommand = 'L';
					data.ulTID = myTID;
					data.ulLID = myId;
					//data.length = sLabel.length() > 256?sLabel.length():255;
					//memmove(data.str, sLabel.c_str(), data.length);
					data.length = vParam[0].length()< 255?vParam[0].length():255; 
					memmove(data.str, vParam[0].c_str(), data.length);
					rc |= send((wxChar*)&data, sizeof(data));
				}
				return rc;
			default: 
				break;
		}
	}

	LPCreatePlot data;
	data.cCommand = 'C';
	data.ulTID = myTID;
	data.ulLID = myId;
	return send((wxChar*)&data, sizeof(data));
};
int LogPlot::write(const std::string& strData ){
	double x, y;
	/*wxString sData(strData.c_str(), wxConvUTF8);
	if (!(sData.BeforeFirst(':').ToDouble(&x) &
				sData.AfterFirst(':').ToDouble(&y))) return false;
*/
	if (sscanf(strData.c_str(), "%le:%le", &x, &y) != 2)return false;
	LPAddPoint data;
	data.cCommand = 'W';
	data.ulTID = myTID;
	data.ulLID = myId;
	data.x = x;
	data.y = y;
	return send((wxChar*)&data, sizeof(LPAddPoint))?1:-1;
};

LogPlot::~LogPlot(){
	if(pUdpSocket) pUdpSocket->Destroy(); 
	;
};
bool LogPlot::send(wxChar*data, int size){

	pUdpSocket->SendTo(server,data, size);
	bool rc = pUdpSocket->Error();
	return !rc;

}
int LogPlot::setLabel(Logger *l,const   std::string&str){
	LogPlot*o = dynamic_cast<LogPlot*>(l);
	LPString data;
	data.cCommand = 'L';
	data.ulTID = o->myTID;
	data.ulLID = o->myId;
	data.length = str.length() < 255?str.length():255;
	memmove(data.str, str.c_str(), data.length);
	return o->send((wxChar*)&data, sizeof(data));
}
int LogPlot::setTitle(Logger *l,const   std::string&str){
	LogPlot*o = dynamic_cast<LogPlot*>(l);
	LPString data;
	data.cCommand = 'T';
	data.ulTID = o->myTID;
	data.ulLID = o->myId;
	data.length = str.length() < 255?str.length():255;
	memmove(data.str, str.c_str(), data.length);
	return o->send((wxChar*)&data, sizeof(data));
}
int LogPlot::setColour(Logger *l,const   std::string&str){
	LogPlot*o = dynamic_cast<LogPlot*>(l);
	LPString data;
	data.cCommand = 'O';
	data.ulTID = o->myTID;
	data.ulLID = o->myId;
	data.length = str.length() < 255?str.length():255;
	memmove(data.str, str.c_str(), data.length);
	return o->send((wxChar*)&data, sizeof(data));
}

int LogPlot::attach(Logger *l,const   std::string&str){
	LogPlot*o = dynamic_cast<LogPlot*>(l);
	LPAttach data;
	unsigned long ul0, ul1;
	data.cCommand = 'A';
	data.ulTID = o->myTID;
	data.ulLID = o->myId;
	wxString sData(str.c_str(), wxConvUTF8);
	int rc = 0;
	if (sData.BeforeFirst(':').ToULong(&ul0)) ++rc;
	if (sData.AfterFirst(':').ToULong(&ul1)) ++rc;
	switch (rc){
		case 2:
			data.ulTIDatt = ul0;
			data.ulLIDatt = ul1;
			break;
		case 1:
			data.ulTIDatt = o->myTID;
			data.ulLIDatt = ul0;
			break;
		default:
			return false;
	}
	return o->send((wxChar*)&data, sizeof(data));
}

LogPlot::LogPlot(): Logger() , pUdpSocket (NULL){
	server.LocalHost();
	server.Service(48000);           
	mapCommands[std::string("SET_LABEL")] =  &setLabel; 
	mapCommands[std::string("SET_TITLE")] =  &setTitle; 
	mapCommands[std::string("SET_COLOUR")] =  &setColour; 
	mapCommands[std::string("ATTACH")] =  &attach; 

};

