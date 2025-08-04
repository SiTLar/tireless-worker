extern "C"{
//#include "ni4882.h"
#include <gpib/ib.h>
}
//extern unsigned long __stdcall (*_ibonl)(int ud, int v);
class DevGPIB: public DevInterface{
	int handle = -1;
	static std::map<std::string, void(*)(int, long)>mapAttr;
	friend void fnGenDevs(HandlerLibInterface*);

	protected:
	DevGPIB();

	public:
	virtual ~DevGPIB();
	//DevGPIB(const DevGPIB& ){ };
	virtual DevInterface* clone(){return new DevGPIB(*this);};
	virtual void disconnect() { ibonl(handle,0);};

	virtual std::string makeUniqueDev(const std::string& strInit) const ;
	virtual std::string makeBusLock(const std::string& strInit) const ;
	virtual bool connect(const std::string& strInit) ;
	
	virtual bool attribute(Attr* pAttrStr);

	virtual bool write(const std::string& str);
	virtual bool read(std::string*,std::string* ,int ) ;

};
