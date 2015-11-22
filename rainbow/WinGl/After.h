#ifndef _WinGl_After_h_
#define _WinGl_After_h_

class ViewDraw : public SystemDraw {
public:
	ViewDraw(Ctrl *ctrl) {}
	~ViewDraw() {}
};

class DHCtrl : Ctrl {};

int AppMain(HINSTANCE hInstance, LPSTR lpCmdLine);

#define GUI_APP_MAIN \
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) \
{ return UPP::AppMain(hInstance, lpCmdLine); } \
\
void GuiMainFn_()

#endif
