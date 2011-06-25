class ViewDraw : public SystemDraw {
	ImageBuffer ib;

public:
	ViewDraw(Ctrl *ctrl) : SystemDraw(ib) { _DBG_ }
	~ViewDraw() {}
};

class DHCtrl : Ctrl {};

void InitFB();


#ifdef PLATFORM_WIN32
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
	UPP::FBInit(hInstance); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::UsrLog("---------- About to delete this log of WinFB..."); \
	UPP::DeleteUsrLog(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()
#endif