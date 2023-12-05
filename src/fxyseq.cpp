#include <wx/wx.h>
#include <wx/string.h> 
#include "mathplot.h"
#include "fxyseq.h"
//#include "logplotpr.hpp"
IMPLEMENT_DYNAMIC_CLASS(FXYSeq, mpFXY)

FXYSeq::FXYSeq(SharedP<FXYSeq*>& _myTail,wxString name, int flags ) : 
mpFXY(name,flags),myTail(_myTail), m_minX(-1), 
	m_maxX(1), m_minY(-1), m_maxY(1) {
		m_type = mpLAYER_PLOT;
		Rewind();
}
FXYSeq::FXYSeq(wxString name, int flags ) : 
mpFXY(name,flags),myTail(NULL), m_minX(-1), 
	m_maxX(1), m_minY(-1), m_maxY(1) {
		m_type = mpLAYER_PLOT;
		Rewind();
}

inline void FXYSeq::Rewind() {
	itIDX = data.begin() ;
}

bool FXYSeq::GetNextXY(double & x, double & y) {
	if (itIDX == data.end()) return false;
	else {
		FXY_POINT cPoint = *itIDX++;
		x = cPoint.first;
		y = cPoint.second;

		return itIDX != data.end();
	}
}

void FXYSeq::Clear(){
	data.clear();
}
FXYSeq::~FXYSeq(){
	if(*myTail) *myTail = NULL;
	//wxMessageBox(wxT("KUKU"));
}

void FXYSeq::addData( double x, double y){
	if (data.empty()){
		m_minX = x;
		m_maxX = x;
		m_minY = y;
		m_maxY = y;
	} else {
		m_minX = x<m_minX?x:m_minX;
		m_maxX = x>m_maxX?x:m_maxX;
		m_minY = y<m_minY?y:m_minY;
		m_maxY = y>m_maxY?y:m_maxY;
	}
	// Copy the data:
	data.push_back(FXY_POINT(x,y)); 
	// Update internal variables for the bounding box.

}
