#include <map>
#include <string>
#include "logger.h"

/*
#include "thread.h"
extern Vciml * pvciMailingList;
Logger *Logger::getLog(unsigned long  TID, unsigned long id){
	MyThread* victim = pvciMailingList->item(TID);
 	if (!victim) return 0;
	return victim->logs->item(id);
}; 


static std::string const Logger::getName(unsigned long TID){
	MyThread* victim = pvciMailingList->item(TID);
 	if (!victim)return std::string();
	return victim->sName;
	
};*/
bool Logger::start(unsigned long  TID, unsigned long id, const std::string & in_name, const std::string & sParam, void*_parent) {
	//pCB = _pCB;
	myTID = TID;
	myId = id;
	sName = in_name;
	fParent = _parent;
	return init(sParam); 
};
int Logger::command(const std::string& sCmd,const  std::string& sVal){
	std::map<std::string, int (*) ( Logger*,const  std::string &)>::iterator it (mapCommands.find(sCmd));
	if(it == mapCommands.end()) return 0;
	return it->second(this, sVal);
};
