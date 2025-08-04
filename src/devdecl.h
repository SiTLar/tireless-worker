class DevDesc;
class cDeviceBroker;
class mtxCont;
class smartMtx;
class MyThread;  
class cDevHandle;
class ThrDevs;
class HandlerLibInterface;
struct Attr {
	RXSTRING * Attr_ID;	
	RXSTRING * Attr_VAL;	
	struct Attr * next;
} ;
void fnGenDevs(HandlerLibInterface*);
