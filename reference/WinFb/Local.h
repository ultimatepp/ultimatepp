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
	AppExecute__(GuiMainFn_); \
	Ctrl::ExitFB(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()
