class ViewDraw : public SystemDraw {
public:
	ViewDraw(Ctrl *ctrl);
	~ViewDraw();
};

Vector<WString>& coreCmdLine__();
Vector<WString> SplitCmdLine__(const char *cmd);

void CocoInit(int argc, const char **argv, const char **envptr);
void CocoExit();

#ifdef PLATFORM_POSIX
#define GUI_APP_MAIN \
void GuiMainFn_(); \
\
int main(int argc, const char **argv, const char **envptr) { \
	UPP::AppInit__(argc, (const char **)argv, envptr); \
	GUI_APP_MAIN_HOOK \
	UPP::CocoInit(argc, argv, envptr); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::CocoExit(); \
	return UPP::GetExitCode(); \
} \
 \
void GuiMainFn_()

#endif

class DHCtrl : Ctrl {};

enum {
	SF_NSDocumentDirectory,
	SF_NSUserDirectory,
	SF_NSDesktopDirectory,
	SF_NSDownloadsDirectory,
	SF_NSMoviesDirectory,
	SF_NSMusicDirectory,
	SF_NSPicturesDirectory,
};

String GetSpecialDirectory(int i);