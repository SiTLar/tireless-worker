#include <wx/wx.h>

struct FrameGen{
	wxSemaphore ready;
	virtual wxFrame *FG() = 0;
	virtual ~FrameGen(){};
} ;
