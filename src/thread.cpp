#include "thread.h"
#include <wx/filedlg.h>
#include <wx/textdlg.h>
#include <wx/regex.h>
#include <wx/filename.h>

extern "C"{
#include <string.h>
#include "defines.h"
#include "rxpack.h"
#include <stdlib.h>
rxfunc(rfChkTerminte);
}

#include <map>
#include <string>
#include <algorithm>
//#include "devdecl.h"
#include "devabstruction.hpp"
//extern wxString sElDir;
std::multimap<wxString,unsigned long> TaskList;
extern unsigned long ulTimeQuant;
wxMutex mtxTaskList;
Vciml vciMailingList;
const wxString getNameByTID(unsigned long TID){
	MyThread* victim = vciMailingList.item(TID);
 	if (!victim)return wxEmptyString;
	return victim->sName;
}
template < typename Type >
struct delete_it_pair: public std::unary_function< std::pair< unsigned long, Type >, void > {
	inline void operator()( std::pair< unsigned long, Type > victim ){delete victim.second;};
};

template < typename Type >
struct delete_it: public std::unary_function< Type , void > {
	inline void operator()(  Type victim ){delete victim;};
};

void MyThread::OnExit(){
	wxCommandEvent event( wxEVT_USER_FIRST, elTHREAD_LIFECYCLE );
::wxLogDebug( wxT("%d: %s sName=%s"), myID, wxT(" event ready"), sName);
	//event.SetClientData(GetId());
	event.SetClientData(static_cast<void*>(new ThreadMsg(myID, sName)));
::wxLogDebug( wxT("%d: %s"), myID, wxT(" client data ready"));
		vciMailingList.erase(myID);
	::wxLogDebug( wxT("%d: %s"), myID, wxT(" vciMailingList.erase(myID);"));
	
	{
		wxMutexLocker ml(mtxTaskList);
		TaskList.erase(itTList);
	}
::wxLogDebug( wxT("%d: %s"),  myID,wxT(" TaskList.erase(itTList);"));
	fGetHBroker()->remove(this);	
	std::for_each(logs->begin(), logs->end(), delete_it_pair<LogDesc *>());

::wxLogDebug( wxT("%d: %s"),  myID,wxT(" logs deleted"));
	delete logs;
	{
		wxMutexLocker ml(mtxMail);
		std::for_each(lstMail.begin(), lstMail.end(), delete_it<wxString *>());
	}
::wxLogDebug( wxT("%d: %s"), myID, wxT(" mail deleted"));
wxPostEvent( m_parent, event );
::wxLogDebug( wxT("%d: %s"), myID, wxT(" event sent "));
	//delete psRunningScript;
}
void MyThread::setID(unsigned long inp) {
	myID = inp;
	wxMutexLocker ml(mtxTaskList);
	itTList = TaskList.insert(std::pair< wxString, unsigned long>(sName, myID));
};
long fRunScript(wxString &, MyThread *, wxString &);

wxThread::ExitCode MyThread::Entry(){

	//fRunScript(wxString &sOutput, MyFrame *m_frame);
	wxString output;


	//long rc = fRunScript(output, this,psRunningScript);
	fRunScript(output, this, sRunningScript);
	//delete psRunningScript;
	/*wxCommandEvent event( wxEVT_USER_FIRST,elTHREAD_MSG );
	//event.SetClientData(GetId());

	event.SetClientData(static_cast<void*>(new ThreadMsg(this, output)));
	wxPostEvent( const_cast<MyFrame*>(m_parent), event );	
	*/
	return 0;
}


extern "C" 
APIRET APIENTRY rfDevConnect( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	ULONG rc = 10;
	//char errmsg[] = "ERROR";
	//char cOut[20] = {0};
	//RXSTRING ret = {0,0};
	//	wxMessageBox(wxT("rxConn0") );
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	//	wxMessageBox(wxT("rxConn1") );
	if ( my_checkparam( NULL, (char *)name, argc, 2, 2 ) )
		return -1; 
	//	wxMessageBox(wxT("rxConn2") );
	if(!(fGetHBroker()->connect(glData.pthr, wxString::FromUTF8(argv[0].strptr, argv[0].strlength), std::string(argv[1].strptr, argv[1].strlength), &rc)))	
		return -1; 
	//	wxMessageBox(wxT("rxConn3") );
	return RxReturnNumber(NULL, retstr, rc);

}
extern "C" 
APIRET APIENTRY rfDevLock( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	//ULONG rc = 0;
	ULONG devid = 0;
	//ULONG count = 1024;
	//char errmsg[] = "ERROR";
	wxString inbuf;
	//char cOut[20] = {0};
	//RXSTRING ret = {0,0};
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	if (!wxString::FromUTF8(argv[0].strptr, argv[0].strlength).ToULong(&devid)) return -1; ;
	
	fGetHBroker()->lock(glData.pthr, devid);
	return RxReturnNumber(NULL, retstr, 0);

}
extern "C" 
APIRET APIENTRY rfDevUnlock( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	ULONG rc = 0;
	ULONG devid = 0;
	//ULONG count = 1024;
	//char errmsg[] = "ERROR";
	wxString inbuf;
	//char cOut[20] = {0};
	//RXSTRING ret = {0,0};
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	if (!wxString::FromUTF8(argv[0].strptr, argv[0].strlength).ToULong(&devid)) return -1; ;
	
	fGetHBroker()->unlock(glData.pthr, devid);
	return RxReturnNumber(NULL, retstr, rc);

}



extern "C" 
APIRET APIENTRY rfDevAttrib( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	//ULONG rc = 0;
	Attr *pAttrList; 
	Attr *pAttrLast;
	ULONG devid = 0;
	//char errmsg[] = "ERROR";
	wxString wrbuf;
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if ( my_checkparam( NULL, (char *)name, argc, 3, 21 ) ) return -1;
	if (!wxString::FromUTF8(argv[0].strptr, argv[0].strlength).ToULong(&devid)) return -1; ;
	pAttrLast = pAttrList = new Attr;

	for(unsigned int count = 1;;) {
		pAttrLast->Attr_VAL = 0;
		pAttrLast->Attr_ID = 0;
		pAttrLast->next = 0; 
		pAttrLast->Attr_ID = &argv[count++];
		if (count == argc) break;
		pAttrLast->Attr_VAL= &argv[count++];
		if (count == argc) break;
		pAttrLast->next = new Attr;
	}
	if(!fGetHBroker()->attribute(glData.pthr, devid, pAttrList )) return -1;
	//wxMessageBox(inbuf);
	//wxMessageBox(wxString::Format(wxT("buflen=%d"), strlen(inbuf.mb_str(wxConvUTF8))));
	//char* buf = (char *)malloc(strlen(inbuf.mb_str(wxConvUTF8))+1);
	//strcpy(buf, inbuf.mb_str(wxConvUTF8));
	//return RxReturnStringAndFree( NULL, retstr, buf, 1); 
	return RxReturnNumber(NULL, retstr, 1);

}
extern "C" 
APIRET APIENTRY rfDevRead( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	//ULONG rc = 0;
	ULONG devid = 0;
	ULONG count = 1024;
	//char errmsg[] = "ERROR";
	std::string strInbuf;
	//char cOut[20] = {0};
	//RXSTRING ret = {0,0};
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if ( my_checkparam( NULL, (char *)name, argc, 1, 2 ) ) return -1;
	if (!wxString::FromUTF8(argv[0].strptr, argv[0].strlength).ToULong(&devid)) return -1; 
	if (argc == 2)
		wxString::FromUTF8(argv[1].strptr, argv[1].strlength).ToULong(&count);
//		wxMessageBox(wxString::Format(wxT("count=%d"), count));
	
	if(!fGetHBroker()->read(glData.pthr, devid, &strInbuf, count )) strInbuf= "ERROR";
	wxLogDebug( wxT("DevRead: error waiting for a semaphore"));
	wxLogDebug( wxT("DevRead: reader thread killed"));
	//wxMessageBox(inbuf);
	//wxMessageBox(wxString::Format(wxT("buflen=%d"), strlen(inbuf.mb_str(wxConvUTF8))));
	
	if ( strInbuf.length() > RXAUTOBUFLEN ){
		char* ret = (char *)RexxAllocateMemory( strInbuf.length() + 1 );
		if (!ret) return -1;
		retstr->strptr = (RXSTRING_STRPTR_TYPE)ret;
	}
	memcpy( retstr->strptr, strInbuf.data(), strInbuf.length() );
	retstr->strlength = strInbuf.length();
	return 0;
//	return RxReturnStringAndFree( NULL, retstr, buf, 1); 
	//return RxReturnNumber(NULL, retstr, rc);

}

extern "C" 
APIRET APIENTRY rfDevWrite( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	//ULONG rc = 0;
	ULONG devid = 0;
	//char errmsg[] = "ERROR";
	//wxString wrbuf;
	//char cOut[20] = {0};
	//RXSTRING ret = {0,0};
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if ( my_checkparam( NULL, (char *)name, argc, 2, 2 ) ) return -1;
	if (!wxString::FromUTF8(argv[0].strptr, argv[0].strlength).ToULong(&devid)) return -1; ;
	std::string wrbuf(argv[1].strptr, argv[1].strlength);
	//	wxMessageBox(wxString::Format(wxT("DevId=%d"), devid));
	if(!fGetHBroker()->write(glData.pthr, devid, wrbuf )) return -1;
	//wxMessageBox(inbuf);
	//wxMessageBox(wxString::Format(wxT("buflen=%d"), strlen(inbuf.mb_str(wxConvUTF8))));
	//char* buf = (char *)malloc(strlen(inbuf.mb_str(wxConvUTF8))+1);
	//strcpy(buf, inbuf.mb_str(wxConvUTF8));
	//return RxReturnStringAndFree( NULL, retstr, buf, 1); 
	return RxReturnNumber(NULL, retstr, 1);
}
extern "C" 
APIRET APIENTRY rfDevRequest( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	//ULONG rc = 0;
	ULONG devid = 0;
	ULONG count = 1024;
	//char errmsg[] = "ERROR";
	std::string strInbuf;
	//char cOut[20] = {0};
	//RXSTRING ret = {0,0};
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if ( my_checkparam( NULL, (char *)name, argc, 1, 3 ) ) return -1;
	if (!wxString::FromUTF8(argv[0].strptr, argv[0].strlength).ToULong(&devid)) return -1; ;
	std::string wrbuf(argv[1].strptr, argv[1].strlength);
	if (argc == 3)
		wxString::FromUTF8(argv[2].strptr, argv[2].strlength).ToULong(&count);
//		wxMessageBox(wxString::Format(wxT("count=%d"), count));
	
	if(!fGetHBroker()->request(glData.pthr, devid, wrbuf, &strInbuf, count ))  strInbuf= "ERROR";
	//wxMessageBox(inbuf);
	//wxMessageBox(wxString::Format(wxT("buflen=%d"), strlen(inbuf.mb_str(wxConvUTF8))));
	if ( strInbuf.length() > RXAUTOBUFLEN ){
		char* ret = (char *)RexxAllocateMemory( strInbuf.length() + 1 );
		if (!ret) return -1;
		retstr->strptr = (RXSTRING_STRPTR_TYPE)ret;
	}
	memcpy( retstr->strptr, strInbuf.data(), strInbuf.length() );
	retstr->strlength = strInbuf.length();
	return 0;
	//return RxReturnNumber(NULL, retstr, rc);

}

extern "C" 
APIRET APIENTRY rfDevDisconnect( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//char errmsg[] = "ERROR";
	//SHORT sCBRetFlag;
	//ULONG rc = 0;
	ULONG devid = 0;
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	if (!wxString::FromUTF8(argv[0].strptr, argv[0].strlength).ToULong(&devid)) return -1; ;
	fGetHBroker()->disconnect(glData.pthr, devid );
	return RxReturnNumber(NULL, retstr, 0);

}
extern "C" 
APIRET APIENTRY rfChkTerminte( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
//	SHORT sCBRetFlag;
	ULONG rc = 0;
	double dSleeptime;
	unsigned long iSleeptime, ulLTimeQuant = ulTimeQuant;
//	char cOut[120] = {0};
//	RXSTRING ret = {0,0};
	//	wxMessageBox(wxString::FromUTF8(argv[0].strptr, argv[0].strlength)+wxT(".kj.kjl;.kjl;.jn"));
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return 1;
	//return 0;
	if(!wxString::FromUTF8(argv[0].strptr, argv[0].strlength).ToDouble(&dSleeptime)) return 1;
	iSleeptime = static_cast<unsigned long>(dSleeptime *1000);
	for (;iSleeptime > ulLTimeQuant; iSleeptime -= ulLTimeQuant){
		glData.pthr->Sleep(ulLTimeQuant);
		if ( rc = glData.pthr->TestDestroy()) break;
			
	}
	if(!rc){
		glData.pthr->Sleep(iSleeptime);
		rc = glData.pthr->TestDestroy();
	}
	//if (rc ) RexxCallBack("bye_bye", 0, NULL, &sCBRetFlag, &ret);
	 // if( rc & RX_CB_BADN) sprintf(cOut, "%s is not good enough ", argv[0].strptr );
	 // else sprintf(cOut, "%s of us", argv[0].strptr );
	//  return RxReturnString( NULL, retstr, cOut ); 
	
	return rc?-1:RxReturnNumber(NULL, retstr, rc);

}

extern "C" 
APIRET APIENTRY rfLogStart( RFH_ARG0_TYPE , RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	LogDesc * plog = 0;
	//SHORT sCBRetFlag;
	//ULONG rc = 0;
	ULONG logid = 0;
	//char errmsg[] = "ERROR";
	wxString sLogName(wxT("FILE")), sLogInv, sParam(wxEmptyString);
	//char cOut[20] = {0};
	//RXSTRING ret = {0,0};
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	/*if (argc == 0) plog = LogList.find(wxT("FILE"))->second;
	else{*/
	if (argc != 0){
		sLogName = wxString::FromUTF8(argv[0].strptr, argv[0].strlength);
		if (sLogName == wxEmptyString) return -1;
		if (argc > 1 )sParam = wxString::FromUTF8(argv[1].strptr, argv[1].strlength);
		
	}
	LogDesc * ld = fGetHBroker()->getLog(sLogName);
	if (!ld) return -1;
	
	plog = ld->clone();

	logid = glData.pthr->logs->insert(plog);
	if (!plog->start(glData.pthr->getID(), logid,  std::string(glData.pthr->sName.mb_str()),std::string(sParam.mb_str()), glData.pthr->m_parent ))return -1;

	return RxReturnNumber(NULL, retstr, logid);

}

extern "C" 
APIRET APIENTRY rfLogWrite( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	//ULONG rc = 0;
	ULONG logid = 0;
	//char errmsg[] = "ERROR";
	std::string wrbuf;
	//char cOut[20] = {0};
	//RXSTRING ret = {0,0};
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if ( my_checkparam( NULL, (char *)name, argc, 2, 21 ) ) return -1;
	if (!wxString::FromUTF8(argv[0].strptr, argv[0].strlength).ToULong(&logid)) return -1; ;

	LogDesc * plog = glData.pthr->logs->item(logid);

	if(!plog)return -1;
	//	wxMessageBox(wxString::Format(wxT("DevId=%d"), logid));
	unsigned int count = 0;
	while(++count!=argc){
		wrbuf = std::string(argv[count].strptr, argv[count].strlength);
		if(plog->write(wrbuf) < 0) return -1;
	}
	//wxMessageBox(inbuf);
	//wxMessageBox(wxString::Format(wxT("buflen=%d"), strlen(inbuf.mb_str(wxConvUTF8))));
	//char* buf = (char *)malloc(strlen(inbuf.mb_str(wxConvUTF8))+1);
	//strcpy(buf, inbuf.mb_str(wxConvUTF8));
	//return RxReturnStringAndFree( NULL, retstr, buf, 1); 
	return RxReturnNumber(NULL, retstr, 1);

}

extern "C" 
APIRET APIENTRY rfLogCommand( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	bool rc = true;
	ULONG logid = 0;
	//char errmsg[] = "ERROR";
	std::string sCmd,sVal;
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if ( my_checkparam( NULL, (char *)name, argc, 2, 21 ) ) return -1;
	if (!wxString::FromUTF8(argv[0].strptr, argv[0].strlength).ToULong(&logid)) return -1; 
	LogDesc * plog = glData.pthr->logs->item(logid);
	if(!plog)return -1;

	for(unsigned int count = 1;;) {
		sCmd = std::string(argv[count].strptr, argv[count].strlength);
		if (++count == argc) {
			rc = plog->command(sCmd, std::string() );
			break;
		}
		sVal = std::string(argv[count].strptr, argv[count].strlength);

		rc = plog->command(sCmd, sVal);
		if (++count == argc) break;
	}

	if(!rc) return -1;
	//wxMessageBox(inbuf);
	//wxMessageBox(wxString::Format(wxT("buflen=%d"), strlen(inbuf.mb_str(wxConvUTF8))));
	//char* buf = (char *)malloc(strlen(inbuf.mb_str(wxConvUTF8))+1);
	//strcpy(buf, inbuf.mb_str(wxConvUTF8));
	//return RxReturnStringAndFree( NULL, retstr, buf, 1); 
	return RxReturnNumber(NULL, retstr, rc);

}

extern "C" 
APIRET APIENTRY rfNewTask( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	//ULONG rc = 0;
	//char errmsg[] = "ERROR";
	//char cOut[20] = {0};
	//RXSTRING ret = {0,0};
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	wxFileName fnCScript(glData.pthr->sFullName);
	::wxLogDebug((glData.pthr->sFullName + wxT(" is starting ") + (fnCScript.GetPath()+wxFileName::GetPathSeparator() + wxString::FromUTF8(argv[0].strptr, argv[0].strlength))).c_str());;
	NewTaskReq * newtask = new NewTaskReq (glData.pthr->getID(), new wxString(fnCScript.GetPath()+wxFileName::GetPathSeparator() + wxString::FromUTF8(argv[0].strptr, argv[0].strlength)));
	wxCommandEvent event( wxEVT_USER_FIRST, NEW_TASK );
	event.SetClientData(newtask);
	; 
	wxPostEvent(glData.pthr->m_parent , event );
	::wxLogDebug((glData.pthr->sFullName + wxT(" event sent")).c_str());
	if (newtask->ready.WaitTimeout(5000) != wxSEMA_NO_ERROR ) return RxReturnNumber(NULL, retstr, -1);
	delete newtask;
	::wxLogDebug((glData.pthr->sFullName + wxT(" all OK")).c_str());
	return RxReturnNumber(NULL, retstr, newtask->TID);
}
/*
   struct cmp_name: public std::unary_function< vecid<MyThread *>::cont_type , bool > {
   wxString& host;
   cmp_name(wxString& input): host(input){};
   inline bool operator()( vecid<MyThread *>::cont_type victim ){return host==victim.second->sName;};
   };
   */
extern "C" 
APIRET APIENTRY rfGetTID( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	std::multimap<wxString,unsigned long>::const_iterator it;
	std::pair<std::multimap<wxString,unsigned long>::const_iterator, std::multimap<wxString,unsigned long>::const_iterator> band;
	wxString sRet(wxT(""));
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	wxString victim = wxString::FromUTF8(argv[0].strptr, argv[0].strlength);
	//wxMessageBox(victim);
	{
		wxMutexLocker ml(mtxTaskList);
		band = TaskList.equal_range(victim);
		for( it = band.first; it != band.second; ++it)
			sRet<<wxString::Format(wxT("%ld "),it->second);
	}
	char* buf = (char *)malloc(strlen(sRet.mb_str(wxConvUTF8))+1);
	strcpy(buf, sRet.mb_str(wxConvUTF8));
	return RxReturnStringAndFree( NULL, retstr, buf, 1); 
}
extern "C" 
APIRET APIENTRY rfSendMSG( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	//ULONG rc = 0;
	unsigned long TID = 0;
	//char errmsg[] = "ERROR";

	//char cOut[20] = {0};
	//RXSTRING ret = {0,0};
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if ( my_checkparam( NULL, (char *)name, argc, 2, 2 ) ) return -1;
	if (!wxString::FromUTF8(argv[0].strptr, argv[0].strlength).ToULong(&TID)) return -1; 
	//if (sscanf(argv[0].strptr, "%ld", &TID) == EOF) return -1;	
	if (!vciMailingList[TID]) return RxReturnNumber(NULL, retstr, 1);
	wxString* psMsg = new wxString(wxString::Format(wxT("%ld;"), glData.pthr->getID())<<wxString::FromUTF8(argv[1].strptr, argv[1].strlength));
	{
		wxMutexLocker ml(vciMailingList[TID]->mtxMail);
		vciMailingList[TID]->semMail.Post();
		vciMailingList[TID]->lstMail.push_back(psMsg);
	}
	return RxReturnNumber(NULL, retstr, 1);
}
APIRET APIENTRY rfPeekMSG( RFH_ARG0_TYPE , RFH_ARG1_TYPE , RFH_ARG2_TYPE , RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	//SHORT sCBRetFlag;
	//ULONG rc = 0;
	//char errmsg[] = "ERROR";

	//char cOut[20] = {0};
	//RXSTRING ret = {0,0};
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	if(!glData.pthr->lstMail.empty()){
		glData.pthr->semMail.Wait();
		wxMutexLocker ml(glData.pthr->mtxMail);
		wxString *psMsg = *(glData.pthr->lstMail.begin());
		glData.pthr->lstMail.pop_front();
		char* buf = (char *)malloc(strlen(psMsg->mb_str(wxConvUTF8))+1);
		strcpy(buf, psMsg->mb_str(wxConvUTF8));
		return RxReturnStringAndFree( NULL, retstr, buf, 1); 
	}

	return RxReturnPointer( NULL, retstr, 0); 
}
APIRET APIENTRY rfGetMSG( RFH_ARG0_TYPE , RFH_ARG1_TYPE , RFH_ARG2_TYPE , RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	USHORT   query_flag;          /* flag for handler query     */
	SHORT sCBRetFlag;
	ULONG rc = 0;
	//char errmsg[] = "ERROR";

	//char cOut[20] = {0};
	RXSTRING ret = {0,0};
	unsigned long ulLTimeQuant = ulTimeQuant;
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	while (glData.pthr->semMail.WaitTimeout(ulLTimeQuant) != wxSEMA_NO_ERROR){
		if (rc = glData.pthr->TestDestroy()) return -1;
			//return RxReturnNumber(NULL, retstr, 0);
	}
	wxMutexLocker ml(glData.pthr->mtxMail);
	wxString *psMsg = *(glData.pthr->lstMail.begin());
	glData.pthr->lstMail.pop_front();
	char* buf = (char *)malloc(strlen(psMsg->mb_str(wxConvUTF8))+1);
	strcpy(buf, psMsg->mb_str(wxConvUTF8));
	delete psMsg;
	return RxReturnStringAndFree( NULL, retstr, buf, 1); 
}


APIRET APIENTRY rfGetDevices( RFH_ARG0_TYPE , RFH_ARG1_TYPE , RFH_ARG2_TYPE , RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	
	wxString sOut;
	fGetHBroker()->getDevList(sOut);
	char* buf = (char *)malloc(strlen(sOut.mb_str(wxConvUTF8))+1);
	strcpy(buf, sOut.mb_str(wxConvUTF8));
	return RxReturnStringAndFree( NULL, retstr, buf, 1); 
}
APIRET APIENTRY rfGetLoggers( RFH_ARG0_TYPE , RFH_ARG1_TYPE , RFH_ARG2_TYPE , RFH_ARG3_TYPE , RFH_ARG4_TYPE retstr ){
	
	wxString sOut;
	fGetHBroker()->getLogList(sOut);
	char* buf = (char *)malloc(strlen(sOut.mb_str(wxConvUTF8))+1);
	strcpy(buf, sOut.mb_str(wxConvUTF8));
	return RxReturnStringAndFree( NULL, retstr, buf, 1); 
}

extern "C" LONG APIENTRY reSayExit(
		LONG ExitNumber,    /* code defining the exit function    */
		LONG Subfunction,   /* code defining the exit subfunction */
		PUCHAR ParmBlock)    /* function dependent control block   */
{
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;
	wxFrame *m_frame;
	//UCHAR global_workarea;    /* application data anchor    */
	USHORT           query_flag;         /* flag for handler query     */
	RXSIOSAY_PARM* psParam;
	if(ExitNumber != RXSIO) return RXEXIT_NOT_HANDLED;
	RexxQueryExit("sayHandler", NULL, &query_flag, glData.user_info);
	//wxMessageBox(wxString::Format(wxT("%d--%d"), ExitNumber, Subfunction));
	//wxMutexGuiEnter();
	// if (Subfunction == RXSIOSAY){
	psParam = reinterpret_cast<RXSIOSAY_PARM*>(ParmBlock);
	wxString *psSayed = new wxString;
	wxString sSayed = wxString::FromUTF8(psParam->rxsio_string.strptr, psParam->rxsio_string.strlength);
	if (sSayed.Matches(wxT("*ChkDeath*"))) {
		glData.pthr->fTerminated = true;
		//*psSayed <<wxT(" ") << sSayed; 
		wxCommandEvent event( wxEVT_USER_FIRST, elTHREAD_MSG );
		event.SetClientData(new ThreadMsg(glData.pthr->getID(),psSayed, ThreadMsg::TERMINATED));

		m_frame = glData.pthr->m_parent; 
		wxPostEvent( m_frame, event );
		return RXEXIT_HANDLED;               // successfully handled  

	}

	//psSayed->Printf(wxT("TID#%ld::Created "), user_info[0]->GetCurrentId());
	//*psSayed << glData.pthr->GetCreation().FormatISOTime();
	if (glData.pthr->fTerminated) return RXEXIT_HANDLED;
	*psSayed <<wxT(" ") << sSayed; 
	wxCommandEvent event( wxEVT_USER_FIRST, elTHREAD_MSG );
	event.SetClientData(new ThreadMsg(glData.pthr->getID(),psSayed, (Subfunction == RXSIOTRC?ThreadMsg::ERROR_OUT:ThreadMsg::TEXT_OUT)));

	m_frame = glData.pthr->m_parent; 
	wxPostEvent( m_frame, event );
	return RXEXIT_HANDLED;               // successfully handled  
	//}else return RXEXIT_NOT_HANDLED;
}

RXFDATA rfMainArray[] = {
	RXFDATA("ChkDeath", &rfChkTerminte),
	RXFDATA("DevConnect", &rfDevConnect),
	RXFDATA("DevRead", &rfDevRead),
	RXFDATA("DevWrite", &rfDevWrite),
	RXFDATA("DevRequest", &rfDevRequest),
	RXFDATA("DevAttrib", &rfDevAttrib),
	RXFDATA("DevDisconnect", &rfDevDisconnect),
	RXFDATA("DevUnlock", &rfDevUnlock),
	RXFDATA("DevLock", &rfDevLock),
	RXFDATA("LogStart", &rfLogStart),
	RXFDATA("LogWrite", &rfLogWrite),
	RXFDATA("LogCommand", &rfLogCommand),
	RXFDATA("NewTask", &rfNewTask),
	RXFDATA("GetTID", rfGetTID),
	RXFDATA("GetMSG", rfGetMSG),
	RXFDATA("SendMSG", rfSendMSG),
	RXFDATA("PeekMSG", rfPeekMSG),
	RXFDATA("GetDevices", rfGetDevices),
	RXFDATA("GetLoggers", rfGetLoggers),
	RXFDATA(0, 0)

};


extern "C" void fLoadRexxMath();
long fRunScript(wxString &sOutput, MyThread *m_thread, wxString &sScript ){
	RXSTRING rxargv[1];                   /* program argument string    */
	RXSTRING retstr;                      /* program return value       */
	RXSTRING Instore[2];
	ULONG	    rc;                          /* converted return code      */
	SHORT	rc_short;
	LONG	return_code;
	CHAR      return_buffer[250];          /* returned buffer            */
	CHAR	macro_argument[] = "arg";
	wxString sGoodScript;
	RXSYSEXIT pExits[2];
	union {
		unsigned char user_info[8];
		MyThread * pthr;
	} glData;

	//PUCHAR user_info[2];
	/*wxString filename = wxFileSelector(wxT("Choose a script file"));
	  if ( filename.empty() ) filename = wxT("1.cmd"); */


	/* build the argument string */
	MAKERXSTRING(rxargv[0], macro_argument,
			strlen(macro_argument));
	/* set up default return      */
	MAKERXSTRING(retstr, return_buffer, sizeof(return_buffer));
	char tmpbuf[]="sayHandler";
	pExits[0].sysexit_name = tmpbuf;
	pExits[0].sysexit_code = RXSIO;
	pExits[1].sysexit_code = RXENDLST;

	long lScriptLength = strlen(sScript.utf8_str());
	Instore[0].strlength = lScriptLength ;
	Instore[0].strptr = static_cast<char *>(RexxAllocateMemory(lScriptLength ));
	memmove(Instore[0].strptr, sScript.utf8_str(), lScriptLength );
	//wxMessageBox(wxString::FromUTF8(Instore[0].strptr,Instore[0].strlength));
	Instore[1].strptr = NULL;
	Instore[1].strlength = 0;
	glData.pthr = m_thread;
	rc = RexxRegisterExitExe("sayHandler", /* register exit handler */
			&reSayExit,            /* located at this address*/
			glData.user_info);     /* save global pointer    */
	//text.Printf(wxT("RexxRegisterExitExe(...)=%d"), rc);
	//wxMessageBox(wxString::Format(wxT("%d--"), rc));
	//wxMessageBox(m_thread->sName);
	RXFDATA *funcs = rfMainArray;
	do RexxRegisterFunctionExe(funcs->first, funcs->second); 
	while((++funcs)->first);
	fLoadRexxMath();
	return_code = RexxStart(1,	// one argument
			rxargv,		// argument array
			m_thread->sName.mb_str(wxConvUTF8),	// REXX procedure name
			Instore,	// script is preloaded
			"TirelessWorker",	// default address name
			//RXSUBROUTINE,   // calling as a subcommand
			RXCOMMAND|RXRESTRICTED,
			pExits,		// no exits used
			&rc_short,		// converted return code
			&retstr);	// returned result

	::wxLogDebug((glData.pthr->sFullName + wxT(" RexxStart done")).c_str());
	//m_thread->Sleep(10000);
	Instore[0].strlength = 8;
	//wxMessageBox(wxString::FromUTF8(Instore[1].strptr,Instore[1].strlength));
	RexxFreeMemory(Instore[0].strptr);
	RexxFreeMemory(Instore[1].strptr);
	RexxDeregisterExit("sayHandler", NULL);
	funcs = rfMainArray;
	::wxLogDebug((glData.pthr->sFullName + wxT(" rexx memory freed")).c_str());
	do RexxDeregisterFunction(funcs->first); 
	while((++funcs)->first);
	::wxLogDebug((glData.pthr->sFullName + wxT(" rexx functions dereged")).c_str());
	sOutput << wxString::FromUTF8(retstr.strptr,retstr.strlength);
	return rc;
}
/*
   bool LogDesc::start(unsigned long TID, unsigned long id, const std::string & sParam) {
   myTID = TID;
   myId = id; 
//fParent = vciMailingList[myTID]->m_parent; 
return init(sParam); 
};
*/
