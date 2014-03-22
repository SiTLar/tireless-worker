extern "C"{
#include "visa.h"
}
class DevVisa: public DevInterface{
	ViSession handle;
	static std::map<std::string, unsigned long>mapVisaAttr;
	static ViSession defaultRM;
	friend void fnGenDevs(HandlerLibInterface*);

	protected:
	DevVisa();

	public:
	virtual ~DevVisa();
	//DevVisa(const DevVisa& ){ };
	virtual DevInterface* clone(){return new DevVisa(*this);};
	virtual void disconnect() { viClose(handle);};

	virtual std::string makeUniqueDev(const std::string& strInit) const ;
	virtual std::string makeBusLock(const std::string& strInit) const ;
	virtual bool connect(const std::string& strInit) ;
	
	virtual bool attribute(Attr* pAttrStr);

	virtual bool write(const std::string& str);
	virtual bool read(std::string*str, int count) ;

};
