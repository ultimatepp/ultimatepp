class ViewDraw : public SystemDraw {
#if GTK_CHECK_VERSION(3, 22, 0)
	GdkDrawingContext *ctx;
#endif
public:
	ViewDraw(Ctrl *ctrl);
	~ViewDraw();
};

void DrawDragRect(Ctrl& q, const DrawDragRectInfo& f);

class DHCtrl : Ctrl {};

void InitGtkApp(int argc, char **argv, const char **envptr);
void ExitGtkApp();

#define GUI_APP_MAIN \
void GuiMainFn_(); \
\
int main(int argc, char **argv, const char **envptr) { \
	UPP::AppInit__(argc, (const char **)argv, envptr); \
	GUI_APP_MAIN_HOOK \
	UPP::InitGtkApp(argc, argv, envptr); \
	UPP::AppExecute__(GuiMainFn_); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::ExitGtkApp(); \
	UPP::AppExit__(); \
	return UPP::GetExitCode(); \
} \
 \
void GuiMainFn_()
