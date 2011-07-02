#ifndef _WinGl_After_h_
#define _WinGl_After_h_

class ViewDraw : public SystemDraw {
	ImageBuffer ib;
	
	Vector<Rect> dummy_invalid;

public:
	ViewDraw(Ctrl *ctrl) {}
	~ViewDraw() {}
};

void InitGl();

Vector<WString>& coreCmdLine__();
Vector<WString> SplitCmdLine__(const char *cmd);

void FBInit(HINSTANCE hInstance);

#define GUI_APP_MAIN \
void GuiMainFn_();\
\
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) \
{ \
	UPP::coreCmdLine__() = UPP::SplitCmdLine__(UPP::FromSystemCharset(lpCmdLine)); \
	UPP::AppInitEnvironment__(); \
	UPP::GlInit(hInstance); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::UsrLog("---------- About to delete this log of WinGL..."); \
	UPP::DeleteUsrLog(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()


void  SetDesktop(Ctrl& desktop);
Ctrl *GetDesktop();

#endif
