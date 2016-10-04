#include "support.h"

void  MyProgress::SetMax(int max) {

	ts = max;
	return; 
}

void  MyProgress::SetLev(int lev) {

	pp = lev;
	Set(pp, ts);
	Refresh();
	return; 
}

bool	MyProgress::PartialProgress(int par, int tot) {

	double k = (double)ts / (double)tot;
	int p = fceil(par / k);
	Set(pp+p, ts);	
	Refresh();
	
	return false;
}
