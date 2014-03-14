#include <wx/wx.h>
#include <wx/string.h>

struct IcaseCmp : public std::binary_function <wxString,wxString,bool> {
	bool operator() (const wxString& x, const wxString& y) const
	{return (x.CmpNoCase(y)<0);};
};

class DevSerial: public DevInterface{

	friend void fnGenDevs( HandlerLibInterface*);
	protected:
	DevSerial();
	static std::map<wxString, bool (*) ( DevSerial*, const wxString&),IcaseCmp> mapAttr;
	HANDLE handle;
	COMMTIMEOUTS initTimeouts;
	wxString sTerm;
	wxString sOTerm;
	static bool setBaud ( DevSerial*, const wxString&);
	static bool setStopBits ( DevSerial*, const wxString&);
	static bool setParity ( DevSerial*, const wxString&);
	static bool setDataBits ( DevSerial*, const wxString&);
	static bool setTerm ( DevSerial*, const wxString&);
	static bool setOTerm ( DevSerial*, const wxString&);
	static bool setNoTerm ( DevSerial*, const wxString&);
	static bool setWriteTimeout( DevSerial*, const wxString&);
	static bool setReadTimeout( DevSerial*, const wxString&);
	long canonizeBaud(wxString ); 

	public:
	virtual DevInterface* clone(){return new DevSerial(*this);};
	virtual void disconnect() {
		if (handle != INVALID_HANDLE_VALUE){
			PurgeComm(handle,  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
			SetCommTimeouts(handle, &initTimeouts);
			CloseHandle(handle);	
		}
	};

	virtual bool connect(const std::string& strInit) ; 
	virtual bool attribute(Attr* pAttrStr); 
	virtual bool write(const std::string& str);  
	virtual bool read(std::string*str, int count) ;
	virtual std::string makeLock(const std::string& strInit) const;

};
