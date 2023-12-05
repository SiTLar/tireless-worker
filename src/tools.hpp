#ifndef _TW_TOOLS_INCLUDED
#define _TW_TOOLS_INCLUDED

#include <string>
#include <map>
#include <vector>
bool splitString(const std::string& strInput, char token, std::vector<std::string>& out);
struct IcaseCmp : public std::binary_function <std::string,std::string,bool> {
	bool operator() (const std::string& x, const std::string& y) const{
		
		return (strcasecmp(x.c_str(), y.c_str())<0);
	
	};
};

#endif
