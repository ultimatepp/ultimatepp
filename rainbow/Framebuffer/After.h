class ViewDraw : public SystemDraw {
	ImageBuffer ib;
	
	Vector<Rect> dummy_invalid;

public:
	ViewDraw(Ctrl *ctrl) : SystemDraw(ib, dummy_invalid) { _DBG_ }
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

#ifdef PLATFORM_POSIX
void FBInit(const String& fbdevice);

#define GUI_APP_MAIN \
void GuiMainFn_();\
\
int main(int argc, const char **argv, const char **envptr) { \
	UPP::AppInit__(argc, argv, envptr); \
	FBInit("/dev/fb0"); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::UsrLog("---------- About to delete this log of LinuxFB..."); \
	UPP::DeleteUsrLog(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()
#endif

void  SetDesktop(Ctrl& desktop);
Ctrl *GetDesktop();
