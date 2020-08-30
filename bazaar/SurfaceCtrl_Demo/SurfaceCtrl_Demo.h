#ifndef _GLCanvas_demo_GLCanvas_demo_h
#define _GLCanvas_demo_GLCanvas_demo_h

#include <CtrlLib/CtrlLib.h>
#include <SurfaceCtrl/SurfaceCtrl.h>
namespace Upp{

#define LAYOUTFILE <SurfaceCtrl_Demo/SurfaceCtrl_Demo.lay>
#include <CtrlCore/lay.h>

class SurfaceCtrl_Demo : public WithSurfaceCtrl_DemoLayout<TopWindow> {
private:
	
	int TreeIDSelected = 0;
	
	void RetrieveCameraInformation();
	void UpdatePosition();
	void UpdateRotation();
	void UpdateFocus();
	void RetrieveObjectInformation();
	void UpdateObjectInformation();
public:
	typedef SurfaceCtrl_Demo CLASSNAME;
	SurfaceCtrl_Demo();
	
	virtual bool Key(dword key, int count);
};
}
#endif
