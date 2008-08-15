#ifndef _AddOnHost_IAddOns_h_
#define _AddOnHost_IAddOns_h_

#include <AddOn/AddOn.h>

class IDoPlugin : public IAddOn
{
public:
	virtual String Do() = 0;
};

class IComputePlugin : public IAddOn
{
public:
	virtual String Compute() = 0;
};

#endif
