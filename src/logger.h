//#ifndef __LOGGER_INCL
//#define __LOGGER_INCL
class MyThread;
class Logger{

	protected:
		unsigned long myTID;
		unsigned long myId;

		std::map<std::string, int (*) ( Logger*, const std::string&)> &mapCommands;
		Logger(): mapCommands(*(new std::map<std::string, int (*) ( Logger*, const std::string&)>)){};
		Logger( Logger &p): mapCommands(p.mapCommands){};
		/*
		 Logger *getLog(unsigned long TID, unsigned long id);
		static const std::string getName(unsigned long);
		*/
		std::string sName;

	public:
		virtual ~Logger(){};
		void * fParent; 
		//Logger(MyThread * myid): TID(myid){};
		virtual bool start(unsigned long , unsigned long , const std::string &, const std::string &, void* );
		virtual bool init( const std::string & ) = 0;
		//virtual bool init() = 0;
		virtual Logger * clone( ) = 0;
		virtual bool OK() = 0;
		virtual int write(const std::string&) = 0;
		virtual int command(const std::string& ,const  std::string& );
		std::string *strDescr;
};
struct Callback{
	virtual int exec(std::string, void*pPr);
	virtual std::string getLogPath();
	virtual std::string getHandlerPath();
};


//#endif
