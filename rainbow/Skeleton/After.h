class ViewDraw : public SystemDraw {
public:
	ViewDraw(Ctrl *ctrl);
	~ViewDraw();
};

Vector<WString>& coreCmdLine__();
Vector<WString> SplitCmdLine__(const char *cmd);

#define GUI_APP_MAIN \
void GuiMainFn_();\
\
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) \
{ \
	UPP::coreCmdLine__() = UPP::SplitCmdLine__(UPP::FromSystemCharset(lpCmdLine)); \
	UPP::AppInitEnvironment__(); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::UsrLog("---------- About to delete this log..."); \
	UPP::DeleteUsrLog(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()


class DHCtrl : Ctrl {};
