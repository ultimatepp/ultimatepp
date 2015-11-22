class ViewDraw : public SystemDraw {
public:
	ViewDraw(Ctrl *ctrl);
	~ViewDraw();
};

Vector<WString>& coreCmdLine__();
Vector<WString> SplitCmdLine__(const char *cmd);

#ifdef PLATFORM_WIN32

#define GUI_APP_MAIN \
void GuiMainFn_();\
\
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) \
{ \
	UPP::coreCmdLine__() = UPP::SplitCmdLine__(UPP::FromSystemCharset(lpCmdLine)); \
	UPP::AppInitEnvironment__(); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()

#endif

#ifdef PLATFORM_POSIX
#define GUI_APP_MAIN \
void GuiMainFn_(); \
\
int main(int argc, const char **argv, const char **envptr) { \
	UPP::AppInit__(argc, argv, envptr); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::AppExit__(); \
	return UPP::GetExitCode(); \
} \
 \
void GuiMainFn_()

#endif

class DHCtrl : Ctrl {};
