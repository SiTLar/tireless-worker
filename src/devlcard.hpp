extern "C"{
#include <stdio.h>
#include <string.h>
//#include "rexxsaa.h"
#include "Lusbapi.h"
}
//#include <wx/thread.h>
//#include <map>

class DevLCard: public DevDesc{
	wxDynamicLibrary dllLCardUSB;
	

	friend void fnGenDevs();
	static std::map<wxString, unsigned long> mapFileAttr;
	protected:
	// указатель на интерфейс модуля
	ILE440 *pModule;
	MODULE_DESCRIPTION_E440 ModuleDescription;
	ADC_PARS_E440 ap;
	

	DevLCard();

	public:
	DevLCard(const DevLCard& ) :DevDesc(){ };
	virtual ~DevLCard(){
	//	disconnect();
	  };
	virtual DevDesc* clone(){return new DevLCard(*this);};
	virtual void disconnect() {
		wxMutexLocker ml(*pMutex);
		if(!pModule->ReleaseLInstance()) {wxLogError(wxString::Format(wxT(" ReleaseLInstance() --> Bad\n"))); }
		pModule = NULL;
		
	};

	virtual bool connect(const wxString& strInit) {
		wxString sFname = strInit.Mid(6); 
		pMutex.init(sFname);
		wxMutexLocker ml(*pMutex);
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
		ap.ChannelsQuantity = 4; 		// четыре активных канала
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

	virtual bool write(const wxString& str){
		/* 
		   bool rc = fputs(str.char_str(), handle ) !=  EOF?true:false;
		   fflush(handle);
		   */
		return true;
	};  

	virtual bool read(wxString*str, int count) {
		short * buf = new short[ap.ChannelsQuantity];
		bool ret = false;
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
		str->Clear();
		if(pModule->ADC_KADR(buf)){
			for(int idx = 0; idx < ap.ChannelsQuantity; ++idx)
				//*str << wxString::Format(wxT("%d;"), buf[idx]/* /8192*ADC_INPUT_RANGES_E140[(ap.ControlTable[idx] & 0xC0) >> 6 ]*/);
				*str << wxString::Format(wxT("%e;"), static_cast<double>(buf[idx]) /8000*ADC_INPUT_RANGES_E140[(ap.ControlTable[idx] & 0xC0) >> 6 ]);
			ret = true;
		}



		delete[] buf;
		return ret;
	};

};

std::map<wxString, unsigned long> DevLCard::mapFileAttr;
DevLCard::DevLCard():DevDesc(){
	mapFileAttr[wxString(wxT("SEEK_SET"))] = SEEK_SET;
	mapFileAttr[wxString(wxT("SEEK_CUR"))] = SEEK_CUR ;
	mapFileAttr[wxString(wxT("SEEK_END"))] = SEEK_END;
}
