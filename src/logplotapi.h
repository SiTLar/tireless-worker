struct LPCreatePlot{
	char cCommand;
	unsigned long ulTID;
	unsigned long ulLID;
};
struct LPAttach{
	char cCommand;
	unsigned long ulTID;
	unsigned long ulLID;
	unsigned long ulTIDatt;
	unsigned long ulLIDatt;
};
struct LPAddPoint{
	char cCommand;
	unsigned long ulTID;
	unsigned long ulLID;
	double x;
	double y;
};
struct LPString{
	char cCommand;
	unsigned long ulTID;
	unsigned long ulLID;
	int length;
	char str[255];
};
