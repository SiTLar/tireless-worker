#include "interface.h"
#include <iterator>
#include <map>
#include <vector>
#include <algorithm>
#include "handlerlib.hpp"
typedef	std::map<std::string, mtxCont*> MBUSMUTEXES;


class DevCont{
	wxSemaphore*sem;
public:
	DevCont(const DevCont &inp): sem(inp.sem), dev(inp.dev), strInit(inp.strInit),strLock(inp.strLock) {sem->Post();};
	DevCont(DevDesc* _dev): dev(_dev), sem(new wxSemaphore) { };
	DevDesc* dev;
	~DevCont(); 
	
	std::string strInit;
	std::string strLock;
};
struct ExecReq {
	wxString sCommand;
	wxSemaphore ready;
	wxProcess * pPr;

};
class mtxCont{
	int counter;
	public:
	wxMutex *pmtx;
	mtxCont(): pmtx(new wxMutex){
		counter = 0;

	};
	~mtxCont(){
		delete pmtx;	
	}; 
	int inc(){ return ++counter;};
	int dec(){ return --counter;};
	int operator++(){ return ++counter;};
	int operator--(){ return --counter;};
};

class CountedDevHandle{

	int count;
	public:

	DevDesc * const dev;
	CountedDevHandle(DevDesc* newDev): dev(newDev){
		count = 0;
	};
	~CountedDevHandle();

	DevDesc* operator++(){
		++count;
		return  dev;
	};
	bool operator--();


};

//class smartMtx: public MutexInterface{

class smartMtx{
	wxMutex *pmtx;
	MBUSMUTEXES::iterator itMtx;
	smartMtx(smartMtx&);
	MBUSMUTEXES &mtxTree;
	wxMutex &locker;
	bool fInitiaized;
	public:
	smartMtx(MBUSMUTEXES & mT, wxMutex &l):mtxTree(mT), locker(l), pmtx(0),fInitiaized(false) {};
	virtual void init(const std::string& strKey );
	virtual void lock();
	virtual void unlock();


	virtual ~smartMtx();
	wxMutex * getmtx(){
		return pmtx;
	};

	wxMutex * operator->(){
		return pmtx;
	};
	wxMutex & operator*(){
		return *pmtx;
	};
};

class DevLocker{
	wxMutex *pmtx;
	MyThread * idOwner;

	public:
	DevLocker() :pmtx(0), idOwner(0){};
	~DevLocker() {
		if(pmtx) delete pmtx;
	};
	void Lock(MyThread * const); 

	void Unlock(MyThread * const);

};


class HandlerDesc{
	protected:
		HandlerLibData* const pHLD;
	public:
		HandlerDesc(HandlerLibData* inp): pHLD(inp){pHLD->inc();};
		virtual ~HandlerDesc(){ pHLD->dec(); };
};

class DevDesc: public HandlerDesc{
	//unsigned long ulHandle;
	friend struct HGenStuff;
	wxSemaphore *semFreeMutexes;
	DevInterface * const  d;
	DevDesc(DevInterface * ,HandlerLibData*);
	protected:

	DevDesc(DevDesc&P):HandlerDesc(P.pHLD),  d(P.d->clone()), locker(P.locker), semFreeMutexes(P.semFreeMutexes),
		itDevH(P.itDevH), id(P.id) {
		semFreeMutexes->Post();
	};
	//static MBUSMUTEXES pmtxTree;

	public:
	virtual DevDesc * clone(){return new DevDesc(*this);}; 
	wxMutex &locker;
	//DevLocker access;
	std::map<std::string, CountedDevHandle*>::iterator itDevH ;
	unsigned long id;
	virtual ~DevDesc();
	//DevDesc * clone() = 0;
	inline bool attribute(Attr* inp) { return d->attribute(inp);};
	inline bool connect(const std::string&);
	inline void disconnect() { 
		wxMutexLocker ml(locker);
		return d->disconnect();
	};
	inline std::string makeLock(const std::string& inp) {return d->makeLock(inp);};
	inline bool write(const std::string&inp) { return d->write(inp);};
	inline bool read(std::string* inp, int num) { return d->read(inp, num);};
	inline bool operator<(/*DevDesc* lhs,*/ DevDesc* rhs){
		return this->id < rhs->id;	
	};

};

class LogDesc: public HandlerDesc{
	//unsigned long ulHandle;
	Logger * const  l;
	friend struct HGenStuff;
	protected:

	LogDesc(LogDesc&P):HandlerDesc(P.pHLD), l(P.l->clone()), id(P.id){};
	LogDesc(Logger *inpL, HandlerLibData*inpHLD);
	//static MBUSMUTEXES pmtxTree;

	public:
	LogDesc * clone(){return new LogDesc(*this);}; 
	unsigned long id;
	virtual ~LogDesc(){ 
		delete l;
	};
	inline bool start(unsigned long inp0, unsigned long inp1, const std::string &inp2, const std::string & inp3, void * inp4){return l->start(inp0,inp1,inp2,inp3, inp4);};
	inline bool init( const std::string & inp ){return l->init(inp);};
	//virtual bool init() = 0;
	inline bool OK(){return l->OK();};
	inline int write(const std::string&inp){return l->write(inp);};
	inline int command(const std::string& inp0,const  std::string&inp1 ){return l->command(inp0,inp1);};

};


class ThrDevs:public vecid<DevDesc*>{

	MyThread * const TID;
	std::vector<DEVID> *pvLocked;
	public:
	unsigned long counter;
	bool bFullCycle;

	ThrDevs(MyThread *);
	~ThrDevs(); 
	void addlock(DEVID ); 
	void remlock(DEVID ); 
	void unlockAll(); 

};
class HandlerBroker{

	friend  HandlerBroker* fGetHBroker();
	std::map<MyThread *, ThrDevs*> mapThreadDevsInUse;
	std::map<std::string, CountedDevHandle*> mapDevHandlesInUse;
	wxMutex  mtxDevHandlesTreeEdit;
	wxMutex  mtxDevTreeEdit;

	wxMutex mtxAH;
	std::map<wxString, DevCont>mapAvailDevs;
	std::map<wxString, LogDesc*>mapAvailLogs;
	~HandlerBroker(){};
	public:
	DevDesc * getDev(MyThread * , DEVID);
	bool connect(MyThread * , const wxString&, const std::string&, DEVID*);
	void lock(MyThread * , DEVID);
	void unlock(MyThread * , DEVID);
	void disconnect(MyThread * , DEVID);
	void remove(MyThread * );
	bool attribute(MyThread * , DEVID, Attr*);
	bool write(MyThread * , DEVID, const std::string&);
	bool read(MyThread * , DEVID, std::string*, int );
	bool request(MyThread *, DEVID , const std::string&,   std::string*, int );
	void sortMail();
	void clear();
	void setAvailHandlers(std::map<wxString, DevDesc*>&, std::map<wxString, LogDesc*>&);
	inline LogDesc*getLog(wxString const&inp){ 
		wxMutexLocker ml(mtxAH);
		std::map<wxString, LogDesc*>::iterator it(mapAvailLogs.find(inp));
		if (it == mapAvailLogs.end()) return NULL;
		return it->second;
	};
	inline DevDesc*getDev(wxString const&inp){ 
		wxMutexLocker ml(mtxAH);
		std::map<wxString, DevCont>::iterator it(mapAvailDevs.find(inp));
		if (it == mapAvailDevs.end()) return NULL;
		return it->second.dev;
	};
	inline void setLock(wxString const&sName, std::string const& init, std::string const& lock){ 
		wxMutexLocker ml(mtxAH);
		std::map<wxString, DevCont>::iterator it(mapAvailDevs.find(sName));
		if (it == mapAvailDevs.end()) return;
		it->second.strInit = init;
		it->second.strLock = lock;
	};

	void getDevList(wxString &sOut);
	void getLogList(wxString &sOut);
	bool checkLock(std::map<wxString, DevDesc*>&);

};
HandlerBroker* fGetHBroker();


