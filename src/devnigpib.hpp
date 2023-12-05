extern "C"{
#include "ni4882.h"
}
class DevNIGPIB: public DevInterface{
	int handle;
	//static std::map<std::string, unsigned long>mapVisaAttr;
	friend void fnGenDevs(HandlerLibInterface*);

	protected:
	DevNIGPIB();

	public:
	virtual ~DevNIGPIB();
	//DevNIGPIB(const DevNIGPIB& ){ };
	virtual DevInterface* clone(){return new DevNIGPIB(*this);};
	virtual void disconnect() { ibonl(handle,0);};

	virtual std::string makeUniqueDev(const std::string& strInit) const ;
	virtual std::string makeBusLock(const std::string& strInit) const ;
	virtual bool connect(const std::string& strInit) ;
	
	virtual bool attribute(Attr* pAttrStr);

	virtual bool write(const std::string& str);
	virtual bool read(std::string*str, int count) ;

};
