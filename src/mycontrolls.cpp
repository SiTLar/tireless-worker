#include "mycontrolls.hpp"

myGoodListCtrl::myGoodListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name ):
	wxListCtrl(parent, id, pos, size, style, validator, name ){
		iLast = 0;
		dColAR = 1;
		wxListItem col0;
		wxListItem col1;
		col0.SetId(0);
		col0.SetText( wxT("TID") );
		InsertColumn(0, col0);
		col1.SetId(1);
		col1.SetText( wxT("Name") );
		InsertColumn(1, col1);

		//int w = (this->GetSize()).GetWidth();
		SetColumnWidth(0, 32);
		//SetColumnWidth(1, -1);
}


void myGoodListCtrl::DeleteI(unsigned long item){
	DeleteItem(item);
	iLast--;
	
}
//	void onSized();	
long myGoodListCtrl::GetSelected(){
	long item = -1;
	item = GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	return item;
};

void myGoodListCtrl::AddItem(const wxString& sName, unsigned long pThr, int pos = -1 ){
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
	//onSized();
	++iLast;
}
/*
void myGoodListCtrl::onSized(){
	//int w = (this->GetSize()).GetWidth();
	int w = GetClientSize().GetWidth();
	int h = GetClientSize().GetHeight();

	//SetColumnWidth(1, w/2*.9*dColAR);
	//SetColumnWidth(1, (w-64)*.9);
	wxFrame * p = dynamic_cast<wxFrame*>(GetParent());
	//if (p) p->GetStatusBar()->SetStatusText(wxString::Format(wxT("%ldx%ld"),w-64,h));
	SetColumnWidth(0, 64);
	if (h > GetBestSize().GetHeight() ) SetColumnWidth(1,  w - 64);
}
*/
