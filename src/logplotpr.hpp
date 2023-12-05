#include "fxyseq.h"
class PlotFrame: public wxFrame {
	public:
		PlotFrame(SharedP<PlotFrame*>&);
		enum{
			REDRAW,
			ADDPLOT
		};
		~PlotFrame();

		void OnAbout( wxCommandEvent &event );
		void OnQuit( wxCommandEvent &event );
		void OnPrintPreview( wxCommandEvent &event);
		void OnPrint( wxCommandEvent &event );
		void OnFit( wxCommandEvent &event );
		void OnAlignXAxis( wxCommandEvent &event );
		void OnAlignYAxis( wxCommandEvent &event );
		void OnToggleGrid( wxCommandEvent &event );
		void OnToggleScrollbars(wxCommandEvent& event);
		void OnToggleInfoLayer(wxCommandEvent& event);
		void OnSaveScreenshot(wxCommandEvent& event);
		void OnToggleLissajoux(wxCommandEvent& event);
		void OnToggleSine(wxCommandEvent& event);
		void OnToggleCosine(wxCommandEvent& event);
		void OnBlackTheme(wxCommandEvent& event);
		void OnClose(wxCloseEvent & ) ;

		void redraw();
		void addPlot(FXYSeq * ); 
		wxMutex mtx;

		wxEvtHandler *ehFitHook;
		void FitOff(wxCommandEvent &event); 
		void FitOn(wxCommandEvent &event); 

	private:
		int axesPos[2];
		bool ticks;
		SharedP<PlotFrame*>myTail;
		mpInfoCoords *nfo; // mpInfoLayer* nfo;
		//DECLARE_DYNAMIC_CLASS(PlotFrame);
		mpWindow        *m_plot;
		bool m_bFit;
		wxTextCtrl      *m_log;
		DECLARE_EVENT_TABLE();
};

struct GraphHead{
	const unsigned long TID;
	const unsigned long LID;
	GraphHead(unsigned long _TID, unsigned long _LID ) : TID(_TID), LID(_LID){};
	inline bool operator<(const GraphHead& rhs)const {
		if(TID == rhs.TID) return LID<rhs.LID;
		else return TID<rhs.TID;
	};

};
class GraphHandler{
	SharedP<PlotFrame*> ppFrame;
	SharedP<FXYSeq *> ppFxys;
	wxString sLabel;
	wxColour colour;
	void init();
	public:
	GraphHandler();
	GraphHandler(GraphHandler &inp, int);
	//~GraphHandler(); 

	wxString m_sTitle;
	void addData(double, double) ;
	void setLabel(int , char * );
	void setTitle(int , char * );
	void setColour(int , char * );
	void attach(GraphHandler&);

};
struct AddPlotCB{
	wxSemaphore ready;
	FXYSeq*tail;
};
struct RedrawCB{
	double myX, myY;
	RedrawCB(double x, double y, FXYSeq*plot): myX(x), myY(y), tail(plot){};
	void addData(){tail->addData(myX, myY);};
	FXYSeq*tail;
};

class LPApp : public wxApp {
	public:
		virtual bool OnInit();
		virtual int OnExit();
		void OnSockInput(wxSocketEvent& );
		PlotFrame *GetFrame() { return frame; };

	protected:
		PlotFrame      *frame;
		wxDatagramSocket  *pSockServer;

};
union UnionMessages{
	char cCommand;
	LPAddPoint W;
	LPCreatePlot C;
	LPString S;
	LPAttach A;
};

