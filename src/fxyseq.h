#include <utility>
#include <deque>
#include <wx/object.h>
#include "mathplot.h"
template <typename T>
class SharedP{
	T * data;
	unsigned int *count;
	public:
	SharedP():data(new T), count(new unsigned int){
		*count = 1;
	}
	SharedP(T*_):data(_), count(new unsigned int){
		*count = 1;
	}
	SharedP(const SharedP& _): data(_.data), count(_.count){
		++count;
	};
	~SharedP(){
		if(!--count){
			delete count;
			delete data;
		}
	}
	SharedP& operator=( T*r) {
		if(!--count){
			delete count;
			delete data;
		}
		data = r;
		count = new unsigned int;
		*count = 1;
		return *this;

	}
	SharedP& operator=( SharedP&r) {
		if(!--count){
			delete count;
			delete data;
		}
		data = r.data;
		count = r.count;
		return *this;
	}

	T& operator*(){ return *data;};
	T* operator->(){ return data;}
};
typedef std::pair<double, double> FXY_POINT;

class FXYSeq : public mpFXY {
	public:
		/** @param name  Label
		  @param flags Label alignment, pass one of #mpALIGN_NE, #mpALIGN_NW, #mpALIGN_SW, #mpALIGN_SE.
		  */
		FXYSeq(SharedP<FXYSeq*>&_myTail, wxString name = wxEmptyString, int flags = mpALIGN_NE);
		FXYSeq(wxString name = wxEmptyString, int flags = mpALIGN_NE);

		/** Changes the internal data: the set of points to draw.
		 * @sa Clear
		 */
		void addData( double x, double y);

		/** Clears all the data, leaving the layer empty.
		 * @sa SetData
		 */
		void Clear();
		~FXYSeq();

	protected:
		SharedP<FXYSeq*> myTail;
		/** The internal copy of the set of data to draw.
		*/
		std::deque<FXY_POINT> data;
		/** The internal counter for the "GetNextXY" interface
		*/
		std::deque<FXY_POINT>::iterator itIDX;

		/** Loaded at SetData
		*/
		double m_minX,m_maxX,m_minY,m_maxY;

		/** Rewind value enumeration with mpFXY::GetNextXY.
		  Overridden in this implementation.
		  */
		void Rewind();

		/** Get locus value for next N.
		  Overridden in this implementation.
		  @param x Returns X value
		  @param y Returns Y value
		  */
		bool GetNextXY(double & x, double & y);

		/** Returns the actual minimum X data (loaded in SetData).
		*/
		double GetMinX() { return m_minX - (m_maxX - m_minX)/10; }

		/** Returns the actual minimum Y data (loaded in SetData).
		*/

		double GetMinY() { return m_minY - (m_maxY - m_minY)/10; }
		/** Returns the actual maximum X data (loaded in SetData).
		*/
		double GetMaxX() { return m_maxX + (m_maxX - m_minX)/10; }

		/** Returns the actual maximum Y data (loaded in SetData).
		*/
		double GetMaxY() { return m_maxY + (m_maxY - m_minY)/10; }

		int     m_flags; //!< Holds label alignment

		DECLARE_DYNAMIC_CLASS(FXYSeq)
};
