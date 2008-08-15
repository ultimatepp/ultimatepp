#include <Core/Core.h>

using namespace Upp;

#include <AddOn/AddOn.h>
#include <AddOnHost/IAddOns.h>

class DoPlugIn : public IDoPlugin
{
public:
	virtual String GetName(void) {return "My First Do Plugin";}
	virtual String Do(void) {return "Do";}
	virtual int GetMajorRevision() {return 0;}
	virtual int GetMinorRevision() {return 1;}

};

class ComputePlugIn : public IComputePlugin
{
public:
	virtual String GetName(void) {return "My First Compute Plugin";}
	virtual String Compute(void) {return "Compute";}
	virtual int GetMajorRevision() {return 0;}
	virtual int GetMinorRevision() {return 1;}
};

#if 1
ADDONEXPORT DoPlugIn* Start() {
	return &Single<DoPlugIn>();
}
#else
ADDONEXPORT ComputePlugIn* Start() {
	return &Single<ComputePlugIn>();
}
#endif
