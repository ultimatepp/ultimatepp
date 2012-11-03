#ifndef _SDLFb_Local_h_
#define _SDLFb_Local_h_

void FBInit();
void FBDeInit();

#ifdef PLATFORM_WIN32
#define GUI_APP_MAIN \
void GuiMainFn_(); \
\
extern "C" int main(int argc, char *argv[]) { \
	UPP::AppInit__(argc, (const char **)argv); \
	FBInit(); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	FBDeInit(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()

#endif

#ifdef PLATFORM_POSIX
#define GUI_APP_MAIN \
void GuiMainFn_(); \
\
extern "C" int main(int argc, const char **argv, const char **envptr) { \
	UPP::AppInit__(argc, argv, envptr); \
	FBInit(); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	FBDeInit(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()

#endif

#endif
