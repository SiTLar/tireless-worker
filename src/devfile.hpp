#include <cstdio>
#include <cstring>
#include <string>

class DevFile: public DevInterface{
	std::FILE * handle;

	static std::map<std::string, unsigned long> mapFileAttr;
	friend void fnGenDevs( HandlerLibInterface*);
	protected:
	DevFile();

	//DevFile(const DevFile& ) {};
	public:
	/*virtual ~DevFile(){
		disconnect();
		fDHunplug();
	};
	*/
	virtual DevInterface* clone(){return new DevFile();};
	virtual ~DevFile(){
		if (handle)disconnect();
	};
	virtual void disconnect(){
		//MutexLocker ml(pMutex);
		std::fclose(handle);	
		handle = NULL;
	};

	virtual std::string makeLock(const std::string& strInit) const {
		return strInit;
	};
//	virtual bool connect(const std::string& strInit) {
//		std::string strFname = strInit.substr(6);
	virtual bool connect(const std::string& strFname ) {
		handle = std::fopen(strFname.c_str(), "a");	
		handle = std::freopen(strFname.c_str() , "r+", handle);	
		std::fseek(handle, 0, SEEK_END);
		return (handle  !=  0 )?true:false;
	};

	virtual bool attribute(Attr* pAttrStr){

		do{
			std::string strAttrN(pAttrStr->Attr_ID->strptr, pAttrStr->Attr_ID->strlength );
			std::map<std::string, unsigned long>::iterator itAttrName;
			itAttrName = mapFileAttr.find(strAttrN);
			if(itAttrName == mapFileAttr.end()) return false;

			unsigned long ulAttrVal;
			if (!std::sscanf(std::string(pAttrStr->Attr_VAL->strptr,pAttrStr->Attr_VAL->strlength).c_str(),"%lu", &ulAttrVal))
				return false;
			if (std::fseek(handle, ulAttrVal, itAttrName->second) != 0) return false ;
			pAttrStr = pAttrStr->next;
		}while(pAttrStr);
		return true;

	};

	virtual bool write(const std::string& str){
		 bool rc = std::fputs(str.c_str(), handle ) !=  EOF?true:false;
		 std::fflush(handle);
		 return rc;
	};  

	virtual bool read(std::string*str, int count) {
		if (count == 0) count = 1024;
		count = count<1024?count:1024;

		char buf[1024] = {0};
		int retCount = std::fread( buf, 1, count, handle);

		*str = std::string(buf, retCount);

		return true;
	};
	
};

std::map<std::string, unsigned long> DevFile::mapFileAttr;
DevFile::DevFile():DevInterface(){
	mapFileAttr[std::string("SEEK_SET")] = SEEK_SET;
	mapFileAttr[std::string("SEEK_CUR")] = SEEK_CUR ;
	mapFileAttr[std::string("SEEK_END")] = SEEK_END;
}
