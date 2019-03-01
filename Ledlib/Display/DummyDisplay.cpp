#include "DummyDisplay.h"
#include "../Config.h"

namespace Ledlib {

DummyDisplay::DummyDisplay(){}

bool DummyDisplay::Init(){
	if(++initCounter > 1) return false;
	// read config file
	int cols = Config::GetInt("mat_cols");
	int rows = Config::GetInt("mat_rows");
	int chain = Config::GetInt("mat_chain");
	int parallel = Config::GetInt("mat_parallel");
	int orientation = Config::GetInt("mat_orientation");
	int d1 = rows*parallel;
	int d2 = cols*chain;
	int dmax = (d1 > d2) ? d1 : d2;
	int dmin = (d1 < d2) ? d1 : d2;
	if(orientation == 0){
		SetDimensions(dmax, dmin);
	} else {
		SetDimensions(dmin, dmax);
	}
	return true;
}

}
