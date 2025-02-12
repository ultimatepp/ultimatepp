class TopFrameDraw : public SystemDraw {
#if GTK_CHECK_VERSION(3, 22, 0)
	GdkDrawingContext *ctx;
#endif
public:
	TopFrameDraw(Ctrl *ctrl, const Rect& r);
	~TopFrameDraw();
};

class DHCtrl : Ctrl {};

bool InitGtkApp(int argc, char **argv, const char **envptr);
void ExitGtkApp();

#define GUI_APP_MAIN \
void GuiMainFn_(); \
\
int main(int argc, char **argv, const char **envptr) { \
	UPP::AppInit__(argc, (const char **)argv, envptr); \
	GUI_APP_MAIN_HOOK \
	if (!UPP::InitGtkApp(argc, argv, envptr)) \
		return -1; \
	UPP::AppExecute__(GuiMainFn_); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::ExitGtkApp(); \
	UPP::Ctrl::ShutdownThreads(); \
	UPP::AppExit__(); \
	return UPP::GetExitCode(); \
} \
 \
void GuiMainFn_()
