extern "C"{
#include <stdio.h>
#include <string.h>
//#include "rexxsaa.h"
}
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include "interface.h"
#include "logfile.hpp"

bool LogFile::init( const std::string&) {
	char pcResult[1+5+3+3+3+3+3+9+1] = {0};
	

      	time_t rawtime;

	std::time ( &rawtime );
	memmove(&dtStart, std::localtime ( &rawtime ), sizeof(struct std::tm));
	sprintf(pcResult, "-%04d-%02d-%02d-%02d-%02d-%02d-%08ld", 
			1900 + dtStart.tm_year,
			dtStart.tm_mon,
			dtStart.tm_mday,
			dtStart.tm_hour,
			dtStart.tm_min,
			dtStart.tm_sec,
			myTID );
	std::string sFname = sName+std::string(pcResult);

	/*
	   std::stringstream ssFmt;

	   ssFmt <<std::setfill ('0') << std::setw (2) <<
	   1900 + dtStart.tm_year<<"-"<<
	   dtStart.tm_mon<<"-"<<
	   dtStart.tm_mday<<"-"<<
	   dtStart.tm_hour<<"-"<<
	   dtStart.tm_min<<"-"<<
	   dtStart.tm_sec<<  std::setw (8) << myTID;
	   std::string sFname  = sName+ssFmt.str();

*/
	int namelen = strlen(sFname.c_str());
	char* buf = new char[namelen+1];
	//wcscpy(buf, sFname.mb_str(wxConvUTF8));
	//wxMessageBox(sFname);
	buf[namelen] = 0;
	strcpy(buf, sFname.c_str());

	handle = fopen(buf, "a+");	
	delete[] buf;
	return (handle != 0);
};
LogFile::~LogFile(){
	if (sDlm) delete[] sDlm;
	if (sNewEntry) delete[] sNewEntry;
	if(handle)fclose(handle);
};
void LogFile::endwrite(){
	fflush(handle);
};
void LogFile::startwrite(){
	if (fIsNewEntry) {
		fIsNewEntry = false;
		return;
	}
	const char* sNE;
	if(!sDlm) sNE = sDefDlm;
	else sNE = sDlm;
	fputs(sNE, handle);
};
int LogFile::write(const std::string& sData){
	startwrite();
	int ret = fputs(sData.c_str(), handle ); 
	endwrite();
	return ret ;
};
/*
   int LogFile::command(const std::string& sCmd,const  std::string& sVal){
   std::map<std::string, int (*) ( LogFile*,const  std::string &)>::iterator it (mapCommands.find(sCmd));
   if(it == mapCommands.end()) return false;
   return it->second(this, sVal);
   };
   */
int LogFile::newentry(Logger *l,const   std::string&){
	LogFile *o = dynamic_cast<LogFile*>(l);
	if(!o) return false;
	const char* sNE;
	if(!o->sNewEntry) sNE = o->sDefNewEntry;
	else sNE = o->sNewEntry;
	fputs(sNE, o->handle);
	o->fIsNewEntry = true;
	return true;
};

int LogFile::setdefdlm(Logger *l,const std::string& ){
	LogFile *o = dynamic_cast<LogFile*>(l);
	if(!o) return false;
	if (o->sDlm) delete[] o->sDlm;
	o->sDlm = 0;
	return true;
};
int LogFile::setdlm(Logger *l,const std::string &str){
	LogFile *o = dynamic_cast<LogFile*>(l);
	if(!o) return false;
	if (o->sDlm) delete[] o->sDlm;
	o->sDlm = new char[str.length()+1];
	memmove(o->sDlm, str.c_str(), str.length());
	o->sDlm[str.length()+1] = 0;
	return true;

};
int LogFile::setdefnewentry(Logger *l,const std::string &){ 
	LogFile *o = dynamic_cast<LogFile*>(l);
	if(!o) return false;
	if (o->sNewEntry) delete[] o->sNewEntry;
	o->sNewEntry = 0;
	return true;
};


int LogFile::setnewentry(Logger *l,const std::string &str){ 
	LogFile *o = dynamic_cast<LogFile*>(l);
	if(!o) return false;
	if ( o->sNewEntry) delete[]  o->sNewEntry;
	o->sNewEntry = new char[str.length()+1];
	memmove( o->sNewEntry, str.c_str(), str.length());
	o->sNewEntry[str.length()+1] = 0;
	return true;
};
const char* LogFile::sDefDlm =",";
const char *LogFile::sDefNewEntry ="\n";

LogFile::LogFile():Logger(), sDlm(0), sNewEntry(0), fIsNewEntry(true), handle(NULL){ 

	mapCommands[std::string("NEW_ENTRY")] =  &newentry; 
	mapCommands[std::string("SET_DLM")] =  &setdlm; 
	mapCommands[std::string("SET_NEWENTRY")] =  &setnewentry; 
	mapCommands[std::string("SET_DEFAULT_DLM")] =  &setdefdlm; 
	mapCommands[std::string("SET_DEFAULT_NEWENTRY")] =  &setdefnewentry; 
};
