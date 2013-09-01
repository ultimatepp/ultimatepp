class ViewDraw : public SystemDraw {
public:
	ViewDraw(Ctrl *ctrl);
	~ViewDraw();
};


/*
class ViewDraw : public SystemDraw {
	Vector<Rect> dummy;
public:
	ViewDraw(Ctrl *) : SystemDraw(Ctrl::framebuffer, dummy) { dummy.Add(Rect(10, 10, 100, 100)); }
};
*/
class DHCtrl : Ctrl {};

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
