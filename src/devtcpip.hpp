#include <wx/socket.h>
#include "tools.hpp"
class DevTCPIP: public DevInterface{
	int handle;
	static std::map<std::string, bool (*) ( DevInterface*, const std::string&), IcaseCmp > mapAttr;
	friend void fnGenDevs(HandlerLibInterface*);
	wxIPV4address oAddr;
	wxSocketClient *poSocket;
	protected:
//	static bool setTerm ( DevInterface*, const std::string&);
//	static bool setTimeout ( DevInterface*, const std::string&);
	std::string logError(wxSocketError);
	DevTCPIP();

	public:
	virtual ~DevTCPIP();
	//DevGPIB(const DevGPIB& ){ };
	virtual DevInterface* clone(){return new DevTCPIP(*this);};
	virtual void disconnect() {
		if(poSocket){
			poSocket->Close();
			poSocket->Destroy();
		}
	};

	virtual std::string makeUniqueDev(const std::string& ) const ;
	virtual std::string makeBusLock(const std::string& ) const ;
	virtual bool connect(const std::string& ) ;
	
	virtual bool attribute(Attr* );

	virtual bool write(const std::string&);
	virtual bool read(std::string*, std::string*, int ) ;

};
