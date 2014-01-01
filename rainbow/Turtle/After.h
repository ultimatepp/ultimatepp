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

#ifdef PLATFORM_WIN32
#define GUI_APP_MAIN \
void GuiMainFn_(); \
\
extern "C" int main(int argc, const char **argv, const char **envptr) { \
	UPP::AppInitEnvironment__(); \
	UPP::Ctrl::InitTelpp(); \
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
extern "C" int main(int argc, const char **argv, const char **envptr) { \
	UPP::AppInit__(argc, argv, envptr); \
	UPP::Ctrl::InitTelpp(); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	return UPP::GetExitCode(); \
} \
\
void GuiMainFn_()
#endif
