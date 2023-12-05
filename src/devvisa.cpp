/*
extern "C" {
#define INCL_RXFUNC
#define INCL_RXSYSEXIT
#include "rexxsaa.h" 
#include "defines.h"
#include "rxpack.h"
}
*/
#include <cstdio>
#include <map>
#include <string>
#include <cstring>
#include "interface.h"
#include "devvisa.hpp"
DevVisa::~DevVisa(){disconnect();};
bool DevVisa::connect(const std::string& strInit) {
	size_t uiInitPos = strInit.find(':');
	if (uiInitPos == std::string::npos) return false;
	size_t uiHandleLength = strInit.length();
	char *buf = new char[uiHandleLength+1];
	strInit.copy(buf,uiHandleLength ,0 );
	buf[uiHandleLength ] = '\0';
	int res = viOpen(defaultRM, buf, VI_NULL, VI_TMO_IMMEDIATE, &handle);
	/*
	#ifdef DEBUG
		Format(wxT("viOpen:%x"), res);
	#endif
	*/
	delete[] buf;

	return ( res ==  VI_SUCCESS )?true:false;
};

std::string DevVisa::makeUniqueDev(const std::string& strInit) const {

	return strInit;
}
std::string DevVisa::makeBusLock(const std::string& strInit) const {
	size_t uiInitPos = strInit.find("::");
	if (uiInitPos == std::string::npos) return std::string("BAD_INIT");

	return strInit.substr(0, uiInitPos );
}
bool DevVisa::attribute(Attr* pAttrStr){
	unsigned long ulAttrVal;

	do{
		std::string strAttrN(pAttrStr->Attr_ID->strptr, pAttrStr->Attr_ID->strlength );
		std::map<std::string, unsigned long>::iterator itAttrName;

		if(strAttrN == std::string("SET_TERM")){
			std::string newTerm(pAttrStr->Attr_VAL->strptr,pAttrStr->Attr_VAL->strlength);
			if (newTerm.empty()) return false;
			sTerm = newTerm;
			return true;
		}
		if(strAttrN == std::string("SET_TIMEOUT")){
			if (!std::sscanf(std::string(pAttrStr->Attr_VAL->strptr,pAttrStr->Attr_VAL->strlength).c_str(),"%lu", &ulAttrVal))
				return false;
			lReadTimeout = ulAttrVal;
			return true;
		}

		itAttrName = mapVisaAttr.find(strAttrN);
		if(itAttrName == mapVisaAttr.end()) return false;

		if (!std::sscanf(std::string(pAttrStr->Attr_VAL->strptr,pAttrStr->Attr_VAL->strlength).c_str(),"%lu", &ulAttrVal))
			return false;
		if (int iVisaRet = viSetAttribute(handle, itAttrName->second, ulAttrVal) != VI_SUCCESS){
			return false ;
		}
		pAttrStr = pAttrStr->next;
	}while(pAttrStr);
	return true;

};

bool DevVisa::write(const std::string& str){
	ViUInt32 retCount;
	unsigned char *buf = new unsigned char[str.length()];
	memmove(buf, str.c_str(), str.length());

	bool rc = viWrite(handle, buf, str.length(), &retCount) ==  VI_SUCCESS ?true:false;
	delete[] buf;
	return rc;

};  

bool DevVisa::read(std::string*str, std::string*err, int count) {

	if (count == 0) count = 1024;
	count = count<1024?count:1024;
	ViUInt32 retCount;
	unsigned char buf[1024] = {0};
	int iVisaRet = viRead(handle, buf, count, &retCount) ;
	if ( iVisaRet <  VI_SUCCESS){
		return false;
	}

	*str = std::string(reinterpret_cast<char *>(buf), retCount);
	return true;
};


std::map<std::string, unsigned long> DevVisa::mapVisaAttr;
ViSession DevVisa::defaultRM;
DevVisa::DevVisa():DevInterface(){
	viOpenDefaultRM(&defaultRM);
	mapVisaAttr[std::string("VI_ATTR_RSRC_CLASS")] = 0xBFFF0001UL;
	mapVisaAttr[std::string("VI_ATTR_RSRC_NAME")] = 0xBFFF0002UL;
	mapVisaAttr[std::string("VI_ATTR_RSRC_IMPL_VERSION")] = 0x3FFF0003UL;
	mapVisaAttr[std::string("VI_ATTR_RSRC_LOCK_STATE")] = 0x3FFF0004UL;
	mapVisaAttr[std::string("VI_ATTR_MAX_QUEUE_LENGTH")] = 0x3FFF0005UL;
	mapVisaAttr[std::string("VI_ATTR_USER_DATA_32")] = 0x3FFF0007UL;
	mapVisaAttr[std::string("VI_ATTR_FDC_CHNL")] = 0x3FFF000DUL;
	mapVisaAttr[std::string("VI_ATTR_FDC_MODE")] = 0x3FFF000FUL;
	mapVisaAttr[std::string("VI_ATTR_FDC_GEN_SIGNAL_EN")] = 0x3FFF0011UL;
	mapVisaAttr[std::string("VI_ATTR_FDC_USE_PAIR")] = 0x3FFF0013UL;
	mapVisaAttr[std::string("VI_ATTR_SEND_END_EN")] = 0x3FFF0016UL;
	mapVisaAttr[std::string("VI_ATTR_TERMCHAR")] = 0x3FFF0018UL;
	mapVisaAttr[std::string("VI_ATTR_TMO_VALUE")] = 0x3FFF001AUL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_READDR_EN")] = 0x3FFF001BUL;
	mapVisaAttr[std::string("VI_ATTR_IO_PROT")] = 0x3FFF001CUL;
	mapVisaAttr[std::string("VI_ATTR_DMA_ALLOW_EN")] = 0x3FFF001EUL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_BAUD")] = 0x3FFF0021UL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_DATA_BITS")] = 0x3FFF0022UL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_PARITY")] = 0x3FFF0023UL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_STOP_BITS")] = 0x3FFF0024UL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_FLOW_CNTRL")] = 0x3FFF0025UL;
	mapVisaAttr[std::string("VI_ATTR_RD_BUF_OPER_MODE")] = 0x3FFF002AUL;
	mapVisaAttr[std::string("VI_ATTR_RD_BUF_SIZE")] = 0x3FFF002BUL;
	mapVisaAttr[std::string("VI_ATTR_WR_BUF_OPER_MODE")] = 0x3FFF002DUL;
	mapVisaAttr[std::string("VI_ATTR_WR_BUF_SIZE")] = 0x3FFF002EUL;
	mapVisaAttr[std::string("VI_ATTR_SUPPRESS_END_EN")] = 0x3FFF0036UL;
	mapVisaAttr[std::string("VI_ATTR_TERMCHAR_EN")] = 0x3FFF0038UL;
	mapVisaAttr[std::string("VI_ATTR_DEST_ACCESS_PRIV")] = 0x3FFF0039UL;
	mapVisaAttr[std::string("VI_ATTR_DEST_BYTE_ORDER")] = 0x3FFF003AUL;
	mapVisaAttr[std::string("VI_ATTR_SRC_ACCESS_PRIV")] = 0x3FFF003CUL;
	mapVisaAttr[std::string("VI_ATTR_SRC_BYTE_ORDER")] = 0x3FFF003DUL;
	mapVisaAttr[std::string("VI_ATTR_SRC_INCREMENT")] = 0x3FFF0040UL;
	mapVisaAttr[std::string("VI_ATTR_DEST_INCREMENT")] = 0x3FFF0041UL;
	mapVisaAttr[std::string("VI_ATTR_WIN_ACCESS_PRIV")] = 0x3FFF0045UL;
	mapVisaAttr[std::string("VI_ATTR_WIN_BYTE_ORDER")] = 0x3FFF0047UL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_ATN_STATE")] = 0x3FFF0057UL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_ADDR_STATE")] = 0x3FFF005CUL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_CIC_STATE")] = 0x3FFF005EUL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_NDAC_STATE")] = 0x3FFF0062UL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_SRQ_STATE")] = 0x3FFF0067UL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_SYS_CNTRL_STATE")] = 0x3FFF0068UL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_HS488_CBL_LEN")] = 0x3FFF0069UL;
	mapVisaAttr[std::string("VI_ATTR_CMDR_LA")] = 0x3FFF006BUL;
	mapVisaAttr[std::string("VI_ATTR_VXI_DEV_CLASS")] = 0x3FFF006CUL;
	mapVisaAttr[std::string("VI_ATTR_MAINFRAME_LA")] = 0x3FFF0070UL;
	mapVisaAttr[std::string("VI_ATTR_MANF_NAME")] = 0xBFFF0072UL;
	mapVisaAttr[std::string("VI_ATTR_MODEL_NAME")] = 0xBFFF0077UL;
	mapVisaAttr[std::string("VI_ATTR_VXI_VME_INTR_STATUS")] = 0x3FFF008BUL;
	mapVisaAttr[std::string("VI_ATTR_VXI_TRIG_STATUS")] = 0x3FFF008DUL;
	mapVisaAttr[std::string("VI_ATTR_VXI_VME_SYSFAIL_STATE")] = 0x3FFF0094UL;
	mapVisaAttr[std::string("VI_ATTR_WIN_BASE_ADDR_32")] = 0x3FFF0098UL;
	mapVisaAttr[std::string("VI_ATTR_WIN_SIZE_32")] = 0x3FFF009AUL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_AVAIL_NUM")] = 0x3FFF00ACUL;
	mapVisaAttr[std::string("VI_ATTR_MEM_BASE_32")] = 0x3FFF00ADUL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_CTS_STATE")] = 0x3FFF00AEUL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_DCD_STATE")] = 0x3FFF00AFUL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_DSR_STATE")] = 0x3FFF00B1UL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_DTR_STATE")] = 0x3FFF00B2UL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_END_IN")] = 0x3FFF00B3UL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_END_OUT")] = 0x3FFF00B4UL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_REPLACE_CHAR")] = 0x3FFF00BEUL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_RI_STATE")] = 0x3FFF00BFUL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_RTS_STATE")] = 0x3FFF00C0UL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_XON_CHAR")] = 0x3FFF00C1UL;
	mapVisaAttr[std::string("VI_ATTR_ASRL_XOFF_CHAR")] = 0x3FFF00C2UL;
	mapVisaAttr[std::string("VI_ATTR_WIN_ACCESS")] = 0x3FFF00C3UL;
	mapVisaAttr[std::string("VI_ATTR_RM_SESSION")] = 0x3FFF00C4UL;
	mapVisaAttr[std::string("VI_ATTR_VXI_LA")] = 0x3FFF00D5UL;
	mapVisaAttr[std::string("VI_ATTR_MANF_ID")] = 0x3FFF00D9UL;
	mapVisaAttr[std::string("VI_ATTR_MEM_SIZE_32")] = 0x3FFF00DDUL;
	mapVisaAttr[std::string("VI_ATTR_MEM_SPACE")] = 0x3FFF00DEUL;
	mapVisaAttr[std::string("VI_ATTR_MODEL_CODE")] = 0x3FFF00DFUL;
	mapVisaAttr[std::string("VI_ATTR_SLOT")] = 0x3FFF00E8UL;
	mapVisaAttr[std::string("VI_ATTR_INTF_INST_NAME")] = 0xBFFF00E9UL;
	mapVisaAttr[std::string("VI_ATTR_IMMEDIATE_SERV")] = 0x3FFF0100UL;
	mapVisaAttr[std::string("VI_ATTR_INTF_PARENT_NUM")] = 0x3FFF0101UL;
	mapVisaAttr[std::string("VI_ATTR_RSRC_SPEC_VERSION")] = 0x3FFF0170UL;
	mapVisaAttr[std::string("VI_ATTR_INTF_TYPE")] = 0x3FFF0171UL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_PRIMARY_ADDR")] = 0x3FFF0172UL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_SECONDARY_ADDR")] = 0x3FFF0173UL;
	mapVisaAttr[std::string("VI_ATTR_RSRC_MANF_NAME")] = 0xBFFF0174UL;
	mapVisaAttr[std::string("VI_ATTR_RSRC_MANF_ID")] = 0x3FFF0175UL;
	mapVisaAttr[std::string("VI_ATTR_INTF_NUM")] = 0x3FFF0176UL;
	mapVisaAttr[std::string("VI_ATTR_TRIG_ID")] = 0x3FFF0177UL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_REN_STATE")] = 0x3FFF0181UL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_UNADDR_EN")] = 0x3FFF0184UL;
	mapVisaAttr[std::string("VI_ATTR_DEV_STATUS_BYTE")] = 0x3FFF0189UL;
	mapVisaAttr[std::string("VI_ATTR_FILE_APPEND_EN")] = 0x3FFF0192UL;
	mapVisaAttr[std::string("VI_ATTR_VXI_TRIG_SUPPORT")] = 0x3FFF0194UL;
	mapVisaAttr[std::string("VI_ATTR_TCPIP_ADDR")] = 0xBFFF0195UL;
	mapVisaAttr[std::string("VI_ATTR_TCPIP_HOSTNAME")] = 0xBFFF0196UL;
	mapVisaAttr[std::string("VI_ATTR_TCPIP_PORT")] = 0x3FFF0197UL;
	mapVisaAttr[std::string("VI_ATTR_TCPIP_DEVICE_NAME")] = 0xBFFF0199UL;
	mapVisaAttr[std::string("VI_ATTR_TCPIP_NODELAY")] = 0x3FFF019AUL;
	mapVisaAttr[std::string("VI_ATTR_TCPIP_KEEPALIVE")] = 0x3FFF019BUL;
	mapVisaAttr[std::string("VI_ATTR_4882_COMPLIANT")] = 0x3FFF019FUL;
	mapVisaAttr[std::string("VI_ATTR_USB_SERIAL_NUM")] = 0xBFFF01A0UL;
	mapVisaAttr[std::string("VI_ATTR_USB_INTFC_NUM")] = 0x3FFF01A1UL;
	mapVisaAttr[std::string("VI_ATTR_USB_PROTOCOL")] = 0x3FFF01A7UL;
	mapVisaAttr[std::string("VI_ATTR_USB_MAX_INTR_SIZE")] = 0x3FFF01AFUL;
	mapVisaAttr[std::string("VI_ATTR_PXI_DEV_NUM")] = 0x3FFF0201UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_FUNC_NUM")] = 0x3FFF0202UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_BUS_NUM")] = 0x3FFF0205UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_CHASSIS")] = 0x3FFF0206UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_SLOTPATH")] = 0xBFFF0207UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_SLOT_LBUS_LEFT")] = 0x3FFF0208UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_SLOT_LBUS_RIGHT")] = 0x3FFF0209UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_TRIG_BUS")] = 0x3FFF020AUL;
	mapVisaAttr[std::string("VI_ATTR_PXI_STAR_TRIG_BUS")] = 0x3FFF020BUL;
	mapVisaAttr[std::string("VI_ATTR_PXI_STAR_TRIG_LINE")] = 0x3FFF020CUL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_TYPE_BAR0")] = 0x3FFF0211UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_TYPE_BAR1")] = 0x3FFF0212UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_TYPE_BAR2")] = 0x3FFF0213UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_TYPE_BAR3")] = 0x3FFF0214UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_TYPE_BAR4")] = 0x3FFF0215UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_TYPE_BAR5")] = 0x3FFF0216UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_BASE_BAR0")] = 0x3FFF0221UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_BASE_BAR1")] = 0x3FFF0222UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_BASE_BAR2")] = 0x3FFF0223UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_BASE_BAR3")] = 0x3FFF0224UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_BASE_BAR4")] = 0x3FFF0225UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_BASE_BAR5")] = 0x3FFF0226UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_SIZE_BAR0")] = 0x3FFF0231UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_SIZE_BAR1")] = 0x3FFF0232UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_SIZE_BAR2")] = 0x3FFF0233UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_SIZE_BAR3")] = 0x3FFF0234UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_SIZE_BAR4")] = 0x3FFF0235UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MEM_SIZE_BAR5")] = 0x3FFF0236UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_IS_EXPRESS")] = 0x3FFF0240UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_SLOT_LWIDTH")] = 0x3FFF0241UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_MAX_LWIDTH")] = 0x3FFF0242UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_ACTUAL_LWIDTH")] = 0x3FFF0243UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_DSTAR_BUS")] = 0x3FFF0244UL;
	mapVisaAttr[std::string("VI_ATTR_PXI_DSTAR_SET")] = 0x3FFF0245UL;
	mapVisaAttr[std::string("VI_ATTR_JOB_ID")] = 0x3FFF4006UL;
	mapVisaAttr[std::string("VI_ATTR_EVENT_TYPE")] = 0x3FFF4010UL;
	mapVisaAttr[std::string("VI_ATTR_SIGP_STATUS_ID")] = 0x3FFF4011UL;
	mapVisaAttr[std::string("VI_ATTR_RECV_TRIG_ID")] = 0x3FFF4012UL;
	mapVisaAttr[std::string("VI_ATTR_INTR_STATUS_ID")] = 0x3FFF4023UL;
	mapVisaAttr[std::string("VI_ATTR_STATUS")] = 0x3FFF4025UL;
	mapVisaAttr[std::string("VI_ATTR_RET_COUNT_32")] = 0x3FFF4026UL;
	mapVisaAttr[std::string("VI_ATTR_BUFFER")] = 0x3FFF4027UL;
	mapVisaAttr[std::string("VI_ATTR_RECV_INTR_LEVEL")] = 0x3FFF4041UL;
	mapVisaAttr[std::string("VI_ATTR_OPER_NAME")] = 0xBFFF4042UL;
	mapVisaAttr[std::string("VI_ATTR_GPIB_RECV_CIC_STATE")] = 0x3FFF4193UL;
	mapVisaAttr[std::string("VI_ATTR_RECV_TCPIP_ADDR")] = 0xBFFF4198UL;
	mapVisaAttr[std::string("VI_ATTR_USB_RECV_INTR_SIZE")] = 0x3FFF41B0UL;
	mapVisaAttr[std::string("VI_ATTR_USB_RECV_INTR_DATA")] = 0xBFFF41B1UL;
};

