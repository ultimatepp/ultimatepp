#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

LRESULT CALLBACK fbWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Vector<WString>& coreCmdLine__();
Vector<WString> SplitCmdLine__(const char *cmd);

#define GUI_APP_MAIN \
void GuiMainFn_();\
\
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) \
{ \
	UPP::coreCmdLine__() = UPP::SplitCmdLine__(UPP::FromSystemCharset(lpCmdLine)); \
	UPP::AppInitEnvironment__(); \
	FBInit(hInstance); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::UsrLog("---------- About to delete this log..."); \
	UPP::DeleteUsrLog(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()

END_UPP_NAMESPACE


