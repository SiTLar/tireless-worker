#include <stdio.h>
class LogFile: public Logger{

	FILE * handle;
	char *sDlm;
	const static char *sDefDlm ;
	char *sNewEntry;
	static const char *sDefNewEntry ;
	bool fIsNewEntry;
//	static std::map<std::string, int (*) ( LogFile*, const std::string&)> mapCommands;
	LogFile();
	friend void fnGenDevs(HandlerLibInterface*);
	struct std::tm dtStart;
	protected:
	public:
	virtual bool init( const std::string&) ;
	virtual bool OK(){return (handle != 0);};
	virtual Logger * clone() {return new LogFile(*this);};
	virtual ~LogFile();
	void endwrite();
	void startwrite();
	virtual int write(const std::string& sData);
	static int newentry(Logger *l,const std::string&);
	static int setdefdlm(Logger *l,const std::string& );
	static int setdlm(Logger *l,const std::string &str);
	static int setdefnewentry(Logger *l,const std::string &); 
	static int setnewentry(Logger *l,const std::string &str); 

};

//std::map<std::string, int (*) ( LogFile*, const std::string&)> LogFile::mapCommands;
	

