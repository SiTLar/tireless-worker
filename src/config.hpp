class HandlersHome;
struct PageData{
	wxPanel* pPanel;
	wxStaticText* pText;
	wxPropertyGrid* pPG;
	wxString sName;
	PageData(const wxString&,wxFlatNotebook* );
	PageData(const wxString&inName):sName(inName){};
	inline bool operator<(const PageData& rhs)const {return sName<rhs.sName;}
};


class HandlerLibPanel : public HandlerLibPanelProto {

	HandlerLibData const* phData;
	inline void chkboxToMap(std::map<wxString,TypeFlag> &, wxCheckListBox *, int);
	public:
	HandlerLibPanel( wxWindow* , int, const HandlerLibData*);
	std::map<wxString,wxString> mapDevInfo, mapLogInfo;
	//void init(wxXmlNode*);
	inline wxString getMD5(){return textMD5->GetValue();};
	inline void applyConfig( std::map<wxString,TypeFlag> &, HandlersHome*);
	void loadConfigIntoChkList(wxCheckListBox * , std::map<wxString,TypeFlag> &, HandlersHome*, int );
	void getSelHandlersMap(std::map<wxString,TypeFlag> &);
	virtual ~HandlerLibPanel();

};

class HandlersHome : public wxPanel {
	wxScrolledWindow *m_scrolledWindow2;
	wxBoxSizer* bSizer1;
	wxBoxSizer* panelSizer;
	wxWindow* pParent;
	wxStaticBoxSizer* sbSizer4;
	int count;

	std::map<wxString, HandlerLibPanel*>mapHPanels;
	std::map<wxString, std::pair<int, wxCheckListBox * > > mapSelDevs;
	std::map<wxString, std::pair<int, wxCheckListBox * > > mapSelLogs;
	public:
	virtual ~HandlersHome(){};
	inline void insertHPanel(HandlerLibPanel*);
	void supplyHandlers(std::map<wxString, std::map<wxString,TypeFlag> >&);
	inline wxWindow * getSW(){return m_scrolledWindow2; };
	void placeHPanels();
	void setSelHandler(wxString ,  std::pair<int, wxCheckListBox * > const&, int );
	inline int getCount(){return count;};
	inline int getCount(int){return count++;};
	inline int getIdByFlag(int flag){
		switch (flag){
			case HandlerLibData::DEVICE:
				return idCHKLISTDEV;
			case  HandlerLibData::LOGGER:
				return idCHKLISTLOG;
		}
		return wxID_ANY;
	};
	void applyConfig();
	wxStaticText *pText;
	HandlersHome(wxFlatNotebook*);

};

class  dlgConfig: public wxDialog 
{

	private:
	protected:
		wxFlatNotebook* m_flatNotebook1; 
		/*wxPanel* m_panelPaths;
		  wxStaticText* m_staticTextPaths;
		  wxPanel* m_panelHandlers;
		  wxStaticText* m_staticTextHandlers;
		  wxPanel* m_panelInternals;
		  wxStaticText* m_staticTextInternals;
		  */
		wxStdDialogButtonSizer* buttonSizer;
		wxButton* buttonSizerOK;
		wxButton* buttonSizerCancel;
		HandlersHome * pHH;

		std::set<PageData> setProps;
		HandlersHome* phhPage;
	public:

		//wxPropertyGridManager* m_PGM;
		dlgConfig( wxWindow* parent, wxXmlNode *oConfig, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,380 ), long style = wxDEFAULT_DIALOG_STYLE );
		~dlgConfig();

		void loadConfig(wxXmlNode* );
		void loadCGrid(wxPropertyGrid*, wxXmlNode* );
		void loadHandlers(wxPropertyGrid*, wxXmlNode* );
		void switchPage( wxFlatNotebookEvent&);
		void supplyHandlers(std::map<wxString, std::map<wxString,TypeFlag> >&);
		void onChkListTog(wxCommandEvent& event);
		void onChkListSel(wxCommandEvent& event);
		void onPropSel(wxPropertyGridEvent&);
		wxXmlNode* getCfg();
};


