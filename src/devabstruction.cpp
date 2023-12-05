#include <string>
#include "thread.h"
#include "devabstruction.hpp"
#include "md5.h"
#include <wx/filename.h>

ThrDevs::ThrDevs(MyThread *id): vecid<DevDesc *>(0), TID(id) {
	pvLocked = new std::vector<DEVID>;
};

ThrDevs::~ThrDevs(){
	unlockAll();
	delete pvLocked;
};

void ThrDevs::addlock(DEVID ){ 

} 

void ThrDevs::remlock(DEVID ){

};
void ThrDevs::unlockAll(){
};

bool HandlerBroker::connect(MyThread * TID, const wxString& sDevName,const std::string& strDevInit, DEVID *id){

	DevDesc * pNewDev; 
	DevDesc* pDevLocker = getDev(sDevName);
	if (!pDevLocker) return false;
	std::string strUniqueDev = std::string(sDevName.utf8_str())+ pDevLocker->makeUniqueDev(strDevInit);
	{
		wxMutexLocker ml1(mtxDevHandlesTreeEdit);
		std::map<std::string, CountedDevHandle*>::iterator itDevH = mapDevHandlesInUse.find(strUniqueDev);
		if(itDevH != mapDevHandlesInUse.end()){
			pNewDev = itDevH->second->dev;
		}else{
			pNewDev = pDevLocker->clone();
			if (!(pNewDev->connect(strDevInit)))return false;

			wxMutexLocker ml2(mtxBusLockTreeEdit);
			std::string strBusLock= pNewDev->makeBusLock(strDevInit);
			pNewDev->itBusLocker =  mapBusLockers.find(strBusLock);
			if(pNewDev->itBusLocker == mapBusLockers.end())
				pNewDev->itBusLocker = mapBusLockers.insert(std::make_pair(strBusLock, new mtxCont)).first;
			++(*(pNewDev->itBusLocker->second));
			itDevH = mapDevHandlesInUse.insert( std::make_pair( strUniqueDev, new CountedDevHandle(pNewDev)) ).first; 
			itDevH->second->dev->itDevH = itDevH;
		}
		++(*(itDevH->second));
	}

	wxMutexLocker ml0(mtxDevTreeEdit);
	std::map<MyThread *, ThrDevs*>::iterator it=mapThreadDevsInUse.find(TID); 
	if (it == mapThreadDevsInUse.end()) it = mapThreadDevsInUse.insert(std::pair<MyThread * , ThrDevs*>(TID, new ThrDevs(TID)) ).first; 
	*id = it->second->insert(pNewDev);

	return true;
};
void HandlerBroker::disconnect(MyThread * TID, DEVID id){
	DevDesc * pDev;
	{
		wxMutexLocker ml(mtxDevTreeEdit);
		std::map<MyThread *, ThrDevs*>::iterator it=mapThreadDevsInUse.find(TID); 
		if (it == mapThreadDevsInUse.end()){return;}
		DevDesc * pDev = it->second->item(id);
		if(!pDev) return ;
		if (!(it->second->erase(id))) {
			delete it->second;
			mapThreadDevsInUse.erase(TID);
		}
	}
	{
		wxMutexLocker ml(mtxDevHandlesTreeEdit);
		if (!--(*(pDev->itDevH->second))){
			pDev->disconnect();
			if(!--(*(pDev->itBusLocker->second)))
				mapBusLockers.erase(pDev->itBusLocker);				
		//	delete pDev->itDevH->second;
			mapDevHandlesInUse.erase(pDev->itDevH);
			delete pDev;
		}
	}
	return ;
};
void HandlerBroker::remove(MyThread * TID){

	wxMutexLocker ml(mtxDevTreeEdit);
	wxMutexLocker ml2(mtxBusLockTreeEdit);
	std::map<MyThread *, ThrDevs*>::iterator it = mapThreadDevsInUse.find(TID);
	if (it == mapThreadDevsInUse.end()){return ;}
	ThrDevs * pthD = it->second;
	mtxDevHandlesTreeEdit.Lock();
	ThrDevs::iterator ddIt;
	ThrDevs::iterator itLast= pthD->end();
	DevDesc * pDev;
	for(ddIt = pthD->begin(); ddIt != itLast; ++ddIt){
		pDev = ddIt->second;
		if (!--(*(pDev->itDevH->second))){
			pDev->disconnect();
			if(!--(*(pDev->itBusLocker->second)))
				mapBusLockers.erase(pDev->itBusLocker);				
		//	delete pDev->itDevH->second;
			mapDevHandlesInUse.erase(pDev->itDevH);
			delete pDev;
		}
	}
	mtxDevHandlesTreeEdit.Unlock(); 
	delete pthD;
	mapThreadDevsInUse.erase(it); 
	return ;
}
bool HandlerBroker::write(MyThread * TID, DEVID id, const std::string& str){

	DevDesc * pDev= getDev(TID, id);
	if(!pDev ) return false;
	//wxMutexLocker ml(pDev->locker);
	wxMutexLocker ml(*(pDev->itBusLocker->second->pmtx));
	return pDev->write(str);
};
bool HandlerBroker::request(MyThread * TID, DEVID id, const std::string& str, std::string* pstr, int count){

	DevDesc * pDev= getDev(TID, id);
	if(!pDev ) return false;
	wxMutexLocker ml(*(pDev->itBusLocker->second->pmtx));
	//wxMutexLocker ml(pDev->locker);
	bool rc = pDev->write(str);
	rc &= makeRead(TID, pDev,  pstr,count);
	return rc;
};
bool HandlerBroker::read(MyThread * TID, DEVID id, std::string* pstr, int count){

	DevDesc * pDev= getDev(TID, id);
	if(!pDev ) return false;
	wxMutexLocker ml(*(pDev->itBusLocker->second->pmtx));
	//wxMutexLocker mlDev(pDev->locker);
	bool rc = makeRead(TID, pDev,  pstr,count);
	return rc;
};

inline bool HandlerBroker::makeRead(MyThread * TID, DevDesc*pDev,  std::string* pstr, int iToRead){
	bool rc;
	int iRed = 0;
	if (iToRead == 0) iToRead = 1024;
	int count = iToRead = iToRead<1024?iToRead:1024;

	std::string strError;
	wxStopWatch sw;
	do{
		strError.clear();
		rc = pDev->read(pstr, &strError, count);
		if(!rc || !strError.empty()){
			wxLogError(wxString::FromUTF8(strError.c_str()));
			return false;
		}
		iRed = pstr->length();
		/*if ((pstr->find(pDev->getTerm()) != std::string::npos)
		||	sw.Time() > pDev->readTimeout()) break;
		*/
		std::string term = pDev->getTerm();
		if ((pstr->find(pDev->getTerm()) != std::string::npos)) break;
		if(sw.Time() > pDev->readTimeout()) break;
		if (TID->TestDestroy()) return false;
		TID->Sleep(100);	
	}while(iToRead);
	return true;
	
}

void HandlerBroker::unlock(MyThread * , DEVID ){
	return ;
};
void HandlerBroker::lock(MyThread * , DEVID ){
	return ;
};

bool HandlerBroker::attribute(MyThread * TID, DEVID id, Attr *pA){
	DevDesc * pDev = getDev(TID, id);
	if(!pDev) return false;
	wxMutexLocker ml(*(pDev->itBusLocker->second->pmtx));
	//wxMutexLocker ml(pDev->locker);
	return pDev->attribute(pA);
};

DevDesc * HandlerBroker::getDev(MyThread * TID, DEVID id){
	wxMutexLocker ml(mtxDevTreeEdit);
	std::map<MyThread *, ThrDevs*>::iterator it=mapThreadDevsInUse.find(TID); 
	if (it == mapThreadDevsInUse.end()) return 0; 
	return it->second->item(id);
};


CountedDevHandle::~CountedDevHandle(){ /*delete dev; */};
bool CountedDevHandle::operator--(){
	if(--count) return true;
	else {
		//delete dev;
		return false;
	}
};


HandlerBroker* fGetHBroker(){
	static HandlerBroker DevBr;
	return &DevBr;
}
DevCont::~DevCont(){ 
	if (sem->TryWait() ==  wxSEMA_BUSY ) {
		delete dev;
		delete sem;
	}
};

LogDesc::LogDesc(Logger *inpL, HandlerLibData*inpHLD): HandlerDesc(inpHLD), l(inpL->clone()){};
DevDesc::DevDesc(DevInterface *inpD, HandlerLibData*inpHLD):
	HandlerDesc(inpHLD),d(inpD->clone())//, semFreeMutexes(new wxSemaphore),
	//locker( *(new wxMutex)) 
	{
	};
DevDesc::~DevDesc(){ 
	wxLogError("Deleting d");
	delete d;
	/*
	if (semFreeMutexes->TryWait() ==  wxSEMA_BUSY ){
		delete &locker;
		delete semFreeMutexes;
	}
	*/
};

bool DevDesc::connect(const std::string&inp ) {

//	wxMutexLocker ml(locker);
	return d->connect(inp);

};
struct  AHGenDevCont: public std::unary_function<std::pair<wxString const, DevDesc *>&, void>{
	std::map<wxString, DevCont>&outDevs;
	void doinsert(std::pair<wxString const, DevDesc *>&);
	AHGenDevCont(std::map<wxString, DevCont> &_outDevs): outDevs(_outDevs){ }
	inline void operator()(std::pair<wxString const, DevDesc *>&inp){
		outDevs.insert(std::make_pair(inp.first, DevCont(inp.second)));
	}

};
void AHGenDevCont::doinsert (std::pair<wxString const, DevDesc *>&inp){

	outDevs.insert(std::make_pair(inp.first, DevCont(inp.second)));
}
void HandlerBroker::setAvailHandlers(std::map<wxString, DevDesc*>&inpDevs, std::map<wxString, LogDesc*>&inpLogs){
	clear();
	wxMutexLocker ml(mtxAH);
	mapAvailLogs = inpLogs;
	std::for_each(inpDevs.begin(), inpDevs.end(), AHGenDevCont(mapAvailDevs));
};

struct  AHDevChk: public std::unary_function<std::pair<wxString const, DevDesc *>&, void>{
	std::map<wxString, DevCont>&devConts;
	bool &bOK;
	AHDevChk(std::map<wxString, DevCont> _devConts, bool &_bOK): devConts(_devConts), bOK(_bOK){ 
		_bOK = true;
	};

	inline void operator()(std::pair<wxString const, DevDesc *>&inp){
		std::map<wxString, DevCont>::iterator it(devConts.find(inp.first));
		if (it == devConts.end()) return;
		if (!it->second.strLock.empty()) if (inp.second->makeBusLock(it->second.strInit) != it->second.strLock) bOK = false;
	};

};
bool HandlerBroker::checkLock(std::map<wxString, DevDesc*>&inpDevs){
	bool bOK;
	wxMutexLocker ml(mtxAH);
	std::for_each(inpDevs.begin(), inpDevs.end(),AHDevChk(mapAvailDevs,bOK));
	return bOK; 
}

void smartMtx::init(const std::string& strKey ){
	wxMutexLocker ml(locker);
	fInitiaized = true;
	itMtx = mtxTree.find(strKey);
	if(itMtx == mtxTree.end()) 
		itMtx = mtxTree.insert(std::pair<std::string, mtxCont* >(strKey, new mtxCont)).first;
	pmtx = itMtx->second->pmtx;
	itMtx->second->inc();
};
void smartMtx::lock(){
	wxMutexLocker ml(locker);
	if (fInitiaized) pmtx->Lock();
}
void smartMtx::unlock(){
	wxMutexLocker ml(locker);
	if (fInitiaized) pmtx->Unlock();
}


smartMtx::~smartMtx(){
	wxMutexLocker ml(locker);
	if(fInitiaized)
		if (!(itMtx->second->dec())){ 
			mtxTree.erase(itMtx);
			delete pmtx;
		}
};
/*
   struct  DevGenerator: public std::unary_function<std::pair<wxString, DevInterface*>, void>{
   std::map<wxString,DevDesc*> &dest;
   HandlerLibData*HLD;
   DevGenerator(std::map<wxString,DevDesc*> &inpDest, HandlerLibData*_HLD): dest(inpDest), HLD(_HLD){};
   inline void operator()( std::pair<const std::string, DevInterface*>  &input){
   dest[wxString(input.first.c_str(), wxConvUTF8)] = new DevDesc(input.second, HLD);
   }

   };


   struct  LogGenerator: public std::unary_function<std::pair<const std::string, Logger*>, void>{
   std::map<wxString,LogDesc*> &dest;
   HandlerLibData*HLD;
   LogGenerator(std::map<wxString,LogDesc*> &inpDest, HandlerLibData*_HLD): dest(inpDest), HLD(_HLD){};
   inline void operator()( std::pair<const std::string, Logger*>  &input){
   dest[wxString(input.first.c_str(), wxConvUTF8)] = new LogDesc(input.second, HLD);
   }

   };

 */
template<typename T, typename U>
struct DeleteMappedObject: public std::unary_function<std::pair<const T, U *>, void>{
	inline void operator()( std::pair<const T, U*>&input){
		delete input.second;
	}

};
void HandlerBroker::clear(){
	wxMutexLocker ml(mtxAH);
	//std::for_each(mapAvailDevs.begin(), mapAvailDevs.end(), DeleteMappedObject<wxString, DevCont>() );
	std::for_each(mapAvailLogs.begin(), mapAvailLogs.end(), DeleteMappedObject<wxString, LogDesc>() );	
	mapAvailDevs.clear();
	mapAvailLogs.clear();
};
template< typename T >
struct  GenHList: public std::unary_function<const std::pair<const wxString,T>&, void>{
	wxString &sOut;
	GenHList(wxString &_sOut): sOut(_sOut){};
	inline void operator()(const std::pair<const wxString,T>&paInp){
		sOut += paInp.first + wxT(";");
	}
};
void HandlerBroker::getDevList(wxString &sOut){
	wxMutexLocker ml(mtxAH);
	std::for_each(mapAvailDevs.begin(), mapAvailDevs.end(),GenHList<DevCont>(sOut) );
}
void HandlerBroker::getLogList(wxString &sOut){
	wxMutexLocker ml(mtxAH);
	std::for_each(mapAvailLogs.begin(), mapAvailLogs.end(),GenHList<LogDesc*>(sOut) );
}
HandlerLibData::HandlerLibData(wxString const& _md5, wxString const& _sName,wxString const & _sPath, 
		HandlerLibInterface& _HLI, wxDllType _dllHandle ):
	dllHandle (_dllHandle), sName(_sName),  md5(_md5), sPath(_sPath),
	pMtxCount(new wxMutex), HLI(_HLI), pInstances(new int)

{
	*pInstances = 1;
	sDescr = wxString(HLI.strDescr.c_str(), wxConvUTF8);
};

void HandlerLibData::inc(){
	wxMutexLocker ml(*pMtxCount);
	(*pInstances)++; 
};
void HandlerLibData::dec(){
	wxMutexLocker ml(*pMtxCount);

	if (!--(*pInstances)){
		std::for_each(HLI.mapDevs.begin(), HLI.mapDevs.end(), DeleteMappedObject<std::string, DevInterface>() );
		HLI.mapDevs.clear();
		std::for_each(HLI.mapLogs.begin(), HLI.mapLogs.end(), DeleteMappedObject<std::string, Logger>() );
		HLI.mapLogs.clear();
		wxDynamicLibrary::Unload(dllHandle);
	}
};
void DevLocker::Lock(MyThread * const idLocker){
	if(!pmtx) pmtx = new wxMutex;
	if (idOwner == idLocker) return;
	pmtx->Lock();
	idOwner = idLocker;
};

void DevLocker::Unlock(MyThread * const  idLocker){
	if(!pmtx) pmtx = new wxMutex;
	if (idOwner != idLocker) return;
	pmtx->Unlock();
	idOwner = 0;
};
