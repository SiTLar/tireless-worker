#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <wx/msgdlg.h>
#include <wx/gdicmn.h>
#include <wx/grid.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
#include <vector>
class myGoodListCtrl: public wxListCtrl{
	int iLast;
	int iSelected;
	double dColAR;
	public:
		myGoodListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxLC_ICON, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxListCtrlNameStr):
			wxListCtrl(parent, id, pos, size, style, validator, name ){
				iLast = 0;
				dColAR = 1;
				wxListItem col0;
				wxListItem col1;
				col0.SetId(1);
				col0.SetText( wxT("Name") );
				InsertColumn(1, col0);

				col1.SetId(0);
				col1.SetText( wxT("TID") );
				InsertColumn(0, col1);
				int w = (this->GetSize()).GetWidth();
				SetColumnWidth(0, w/2*.9);
				SetColumnWidth(1, w/2*.9);
			};

		void DeleteI(unsigned long item){
			DeleteItem(item);
			iLast--;
			
		}
		void onSized();	
		long GetSelected(){
			long item = -1;
			item = GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			return item;
		};

		void AddItem(const wxString& sName, unsigned long pThr, int pos = -1 ){
			int idx = pos; 
			if ((idx < 1) || (idx > iLast) ) idx = InsertItem(iLast, sName );
			else InsertItem(iLast, sName );
			
			SetItem(idx, 1, sName);
			SetItem(idx, 0, wxString::Format(wxT("%ld"),pThr) );
			//SetItemPtrData(idx, reinterpret_cast<wxUIntPtr>(pThr));
			SetItemPtrData(idx, pThr);
			/*wxListItem info;
			info.SetId(idx);
			GetItem(info);*/
			SetColumnWidth(1, -1);
			dColAR = GetColumnWidth(1)/64;
			onSized();
			++iLast;
		}

};

class MyConfigListCtrl:public wxListCtrl{

	public:
		
		MyConfigListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxLC_ICON, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxListCtrlNameStr):
			wxListCtrl(parent, id, pos, size, style, validator, name ){
				wxListItem col0;
				wxListItem col1;
				col0.SetId(1);
				col0.SetText( wxT("Parameter") );
				InsertColumn(1, col0);

				col1.SetId(0);
				col1.SetText( wxT("Value") );
				InsertColumn(0, col1);
				int count =  7;
				while(--count){
					int i = GetItemCount();

					InsertItem(i, wxT("Lable"));

					SetItem(i, 1, wxString::Format(wxT("C:\\%ld"),i));
					SetItem(i, 0, wxString::Format(wxT("Path #%ld"),i) );						
				}
			};
};
/*
class cfgGrid: public wxGrid{
	int rows;
	wxGridCellAttr gaOddRows;
	std::vector<wxString> vHelp;
	std::vector<wxString> vNames;
	wxStaticText * hlpText;
	public:
	cfgGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, const wxString& name = wxPanelNameStr): rows(0), hlpText(NULL), wxGrid(parent, id, pos,size, style, name ){ gaOddRows.SetBackgroundColour(*wxLIGHT_GREY); };
	void setHelp(int iRow, const wxString & str){ if(iRow < rows) vHelp[iRow] = str; };
	int AppendRow(const wxString &sName){
		if (AppendRows(1)){
			vNames.push_back(sName);
			vHelp.push_back(wxEmptyString);
			if (rows % 2) SetRowAttr(rows, &gaOddRows );
		}
		return rows++;	
	};
	void setHlpTC(wxStaticText * inp){hlpText = inp;}
	void writeHelp(int row){if (hlpText && (row < rows)) hlpText->SetLabel(vHelp[row]);};
};
*/


