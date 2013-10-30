//#include "devdecl.h"
#include <wx/dynlib.h>
#include "thread.h"
#include "devabstruction.hpp"
#include "tty.hpp"
#include <map>
#include <wx/string.h>
std::map<wxString,DevDesc*> *pDevList;
std::map<wxString,Logger*> *pLogList;
Vciml *pvciMailingList;
extern "C"{
//void __cdecl dynLoad(stDL *input){
void dynLoad(stDL *input){
	pDevList = input->pDevList;
	pLogList = input->pLogList;
	pvciMailingList = input->pvciMailingList;
	fnGenDevs();
	return;
}
static struct es{
	void load(stDL *input){};
	//char desc[] = "geneic devices";
} sExpsym;
extern  es *expsym = &sExpsym;
}
void fnGenDevs(){
//Device interfaces
	(*pDevList)[wxString(wxT("SERIAL"))] = new DevTTY();
}
