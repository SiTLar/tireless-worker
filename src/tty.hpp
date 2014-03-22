extern "C"{
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
//#include <rexxsaa.h>
}
//#include <wx/string.h>
//#include <wx/thread.h>
#include <wx/regex.h>
#include <string>

#include <map>

struct IcaseCmp : public std::binary_function <std::string,std::string,bool> {
	bool operator() (const std::string& x, const std::string& y) const{
		
		return (strcasecmp(x.c_str(), y.c_str())<0);
	
	};
};

class DevTTY: public DevInterface{

	friend void fnGenDevs(HandlerLibInterface* );
	protected:
	DevTTY();
	//DevTTY(const DevTTY& ) :DevDesc(), sTerm(wxT("\x0d\x0a")){ };
	static std::map<std::string, bool (*) ( DevTTY*, const std::string&),IcaseCmp> mapAttr;
	int handle;
	std::string sTerm;
	static bool setBaud ( DevTTY*, const std::string&);
	static bool setStopBits ( DevTTY*, const std::string&);
	static bool setParity ( DevTTY*, const std::string&);
	static bool setDataBits ( DevTTY*, const std::string&);
	static bool setTimeout ( DevTTY*, const std::string&);
	static bool setTerm ( DevTTY*, const std::string&);
	long canonizeBaud(const wxString& ); 

	public:
	virtual DevInterface* clone(){return new DevTTY(*this);};
	virtual std::string makeBusLock(const std::string& strInit) const;
	virtual std::string makeUniqueDev(const std::string& strInit) const ;
	virtual void disconnect();

	virtual bool connect(const std::string& strInit) ; 
	virtual bool attribute(Attr* pAttrStr); 
	virtual bool write(const std::string& str);  
	virtual bool read(std::string*str, int count) ;

};
