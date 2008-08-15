#ifndef _agg_upp_bind_AggCtrl_h_
#define _agg_upp_bind_AggCtrl_h_

#include "agg_upp_bind.h"

class AggCtrl : public ParentCtrl {
	ImageBuffer uibuf;
public:
	
	typedef AggCtrl CLASSNAME;
	AggCtrl();
	~AggCtrl() {;}
};

#endif
