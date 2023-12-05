#include "mycontrolls.hpp"
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
