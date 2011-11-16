#ifdef PLATFORM_POSIX

void FBInit(const String& fbdevice);
void FBDeInit();

#define GUI_APP_MAIN \
void GuiMainFn_();\
\
int main(int argc, const char **argv, const char **envptr) { \
	UPP::AppInit__(argc, argv, envptr); \
	FBInit("/dev/fb0"); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	FBDeInit(); \
	UPP::UsrLog("---------- About to delete this log of LinuxFB..."); \
	UPP::DeleteUsrLog(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()

#endif

