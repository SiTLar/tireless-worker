#include <wx/dynlib.h>
extern "C"{
#include <stdio.h>
#include <string.h>
//#include "rexxsaa.h"
#include "Lusbapi.h"
}
//#include <map>

class DevLCard: public DevInterface{
	wxDynamicLibrary dllLCardUSB;
	

	friend void fnGenDevs(HandlerLibInterface*);
	static std::map<wxString, unsigned long> mapFileAttr;
	protected:
	// указатель на интерфейс модуля
	ILE440 *pModule;
	MODULE_DESCRIPTION_E440 ModuleDescription;
	ADC_PARS_E440 ap;
	int nMeanWindow;

	DevLCard();

	public:
	DevLCard(const DevLCard& ) :DevInterface(){ };
	virtual ~DevLCard(){
	//	disconnect();
	  };
	virtual DevInterface* clone(){return new DevLCard(*this);};
	virtual void disconnect() {
		if(!pModule->ReleaseLInstance()) {wxLogError(wxString::Format(wxT(" ReleaseLInstance() --> Bad\n"))); }
		pModule = NULL;
		
	};

	std::string makeUniqueDev(const std::string& strInit) const {

		return strInit;
	}
	std::string makeBusLock(const std::string& strInit) const {
		size_t uiInitPos = strInit.find("::");
		if (uiInitPos == std::string::npos) return std::string("BAD_INIT");

		return strInit.substr(0, uiInitPos );
	}
	virtual bool connect(const std::string& ) {
		nMeanWindow = 10;
		if(!dllLCardUSB.Load(wxT("Lusbapi"))) 
			{wxLogError(wxString::Format(wxT("Can't load Lusbapi dll"))); return false;}; 
		LPVOID WINAPI (*CreateLInstance)(PCHAR const ) = reinterpret_cast<LPVOID WINAPI (*)(PCHAR const )>(dllLCardUSB.GetSymbol(wxT("CreateLInstance")));
		pModule = static_cast<ILE440 *>(CreateLInstance("e440"));
		if(!pModule) {wxLogError(wxString::Format(wxT(" Module Interface --> Bad\n"))); return false;};
		int i;
		// попробуем обнаружить модуль E14-440 в первых MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI виртуальных слотах
		for(i = 0x0; i < MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI; i++) if(pModule->OpenLDevice(i)) break;
		// что-нибудь обнаружили?
		if(i == MAX_VIRTUAL_SLOTS_QUANTITY_LUSBAPI) {wxLogError(wxString::Format(wxT(" Can't find any module E14-440 in first 127 virtual slots!\n"))); return false;};

		if(!pModule->LOAD_MODULE()) {wxLogError(wxString::Format(wxT(" LOAD_MODULE() --> Bad\n"))); return false;};
		if(!pModule->TEST_MODULE()) {wxLogError(wxString::Format(wxT(" TEST_MODULE() --> Bad\n"))); return false;};
		if(!pModule->GET_MODULE_DESCRIPTION(&ModuleDescription)) {wxLogError(wxString::Format(wxT(" GET_MODULE_DESCRIPTION() --> Bad\n"))); return false;};
		if(!pModule->GET_ADC_PARS(&ap)) {wxLogError(wxString::Format(wxT(" GET_ADC_PARS() --> Bad\n"))); return false;};
		ap.IsCorrectionEnabled = true;					// разрешим корректировку данных на уровне драйвера DSP
		ap.InputMode = NO_SYNC_E440;						// обычный сбор данных безо всякой синхронизации ввода
		ap.ChannelsQuantity = 15; 		// четыре активных канала
		// формируем управляющую таблицу
		for(i = 0x0; i < ap.ChannelsQuantity; i++)
			ap.ControlTable[i] = (WORD)(i | (ADC_INPUT_RANGE_2500mV_E440 << 0x6));
		ap.AdcRate = 100.0;									// частота работы АЦП в кГц
		ap.InterKadrDelay = 0.0;							// межкадровая задержка в мс
		ap.AdcFifoBaseAddress = 0x0;			  			// базовый адрес FIFO буфера АЦП в DSP модуля
		ap.AdcFifoLength = MAX_ADC_FIFO_SIZE_E440;	// длина FIFO буфера АЦП в DSP модуля
		// будем использовать фирменные калибровочные коэффициенты, которые храняться в ППЗУ модуля
		for(i = 0x0; i < ADC_CALIBR_COEFS_QUANTITY_E440; i++) {
			ap.AdcOffsetCoefs[i] =  ModuleDescription.Adc.OffsetCalibration[i];
			ap.AdcScaleCoefs[i] =  ModuleDescription.Adc.ScaleCalibration[i];
		}
		// передадим требуемые параметры работы АЦП в модуль
		if(!pModule->SET_ADC_PARS(&ap)) {wxLogError(wxString::Format(wxT(" SET_ADC_PARS() --> Bad\n"))); return false;};
		return true;
	};

	virtual bool attribute(Attr* pAttrStr){
		/*
		   do{
		   wxString strAttrN = wxString::From8BitData(pAttrStr->Attr_ID->strptr, pAttrStr->Attr_ID->strlength );
		//unsigned long ulAttrID;
		std::map<wxString, unsigned long>::iterator itAttrName;
		itAttrName = mapFileAttr.find(strAttrN);
		if(itAttrName == mapFileAttr.end()) return false;

		unsigned long ulAttrVal;
		if (!wxString::From8BitData(pAttrStr->Attr_VAL->strptr,pAttrStr->Attr_VAL->strlength).ToULong(&ulAttrVal))
		return false;
		if (fseek(handle, ulAttrVal, itAttrName->second) != 0) return false ;
		pAttrStr = pAttrStr->next;
		}while(pAttrStr);
		*/
		return true;

	};

	virtual bool write(const std::string& str){
		/* 
		   bool rc = fputs(str.char_str(), handle ) !=  EOF?true:false;
		   fflush(handle);
		   */
		return true;
	};  

	virtual bool read(std::string*str, int count) {
	
		double * arrDOut = new double[ap.ChannelsQuantity];
		double * arrDOutMean = new double[ap.ChannelsQuantity];
		
		memset(arrDOutMean, 0, sizeof(double)*ap.ChannelsQuantity);
		for(int idx = 0; idx < nMeanWindow; idx++){
			if(!read_once(arrDOut, str)){
				delete[] arrDOut;
				delete[] arrDOutMean;
				return false;
			}

			for (int i =0; i< ap.ChannelsQuantity; i++)
				arrDOutMean[i] += arrDOut[i]/nMeanWindow;
		}
		wxString output;
		for (int i =0; i< ap.ChannelsQuantity; i++)
			output << wxString::Format(wxT("%e;"), arrDOutMean[i]);
		*str = std::string(output.mb_str());
		delete[] arrDOut;
		delete[] arrDOutMean;
		return true;
	}
	
	
	bool read_once(double *arrDOut , std::string *str){
		short * buf = new short[ap.ChannelsQuantity];
		bool ret = false;
		double val;
		/*
		   if (count == 0) count = 1024;
		   count = count<1024?count:1024;

		   unsigned char buf[1024] = {0};
		//fseek(handle, 0, SEEK_SET);
		int retCount = fread( buf, 1, count, handle);

		//wxMessageBox(wxString::Format(wxT("retCount=%d, ferror=%d"), retCount, ferror(handle)));
		//if (!retCount) return false;
		 *str = wxString::From8BitData(reinterpret_cast<char *>(buf), retC<wx/dynlib.h>ount);
		 */
		//str->clear();
		int iAmp = 0;
		int fOK = 1;
		int * arr_amp = new int[ap.ChannelsQuantity];
		do{
			if(pModule->ADC_KADR(buf)){
				fOK = 1;		
				for(int idx = 0; idx < ap.ChannelsQuantity; ++idx){
					iAmp =   (ap.ControlTable[idx] & 0xC0) >> 6;
					if ((abs(buf[idx])<1800) && (iAmp <3) ) {
						arr_amp[idx] = iAmp + 1; 
						fOK = 0;
					}else if ((abs(buf[idx])>7500) && iAmp){
						arr_amp[idx] = iAmp - 1; 
						fOK = 0;
						
					}else arr_amp[idx] = iAmp; 
					if(!fOK) continue;
					val = buf[idx];
					arrDOut[idx] = val/8000*ADC_INPUT_RANGES_E140[iAmp];

					//*str << wxString::Format(wxT("%d;"), buf[idx]/* /8192*ADC_INPUT_RANGES_E140[(ap.ControlTable[idx] & 0xC0) >> 6 ]*/);
					 ///8000*ADC_INPUT_RANGES_E140[(ap.ControlTable[idx] & 0xC0) >> 6 ]);
					//*str << wxString::Format(wxT("%e;"), static_cast<double>(buf[idx]) /8000*ADC_INPUT_RANGES_E140[(ap.ControlTable[idx] & 0xC0) >> 6 ]);
				}
				if(!fOK) {
					pModule->STOP_ADC();

					for(int i = 0x0; i < ap.ChannelsQuantity; i++)
						ap.ControlTable[i] = (WORD)(i | (arr_amp[i] << 0x6));
					if(!pModule->SET_ADC_PARS(&ap)) {*str = std::string(wxString::Format(wxT(" SET_ADC_PARS() --> Bad")).mb_str()); return false;};
					pModule->START_ADC();
				}
				ret = true;
			}else {*str = std::string(wxString::Format(wxT(" ADC_KADR() --> Bad")).mb_str()); return false;}
		} while (!fOK);
		


		delete[] arr_amp;
		delete[] buf;
		return ret;
	};

};
std::map<wxString, unsigned long> DevLCard::mapFileAttr;
DevLCard::DevLCard():DevInterface(){
	mapFileAttr[wxString(wxT("SEEK_SET"))] = SEEK_SET;
	mapFileAttr[wxString(wxT("SEEK_CUR"))] = SEEK_CUR ;
	mapFileAttr[wxString(wxT("SEEK_END"))] = SEEK_END;
}
