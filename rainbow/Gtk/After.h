class ViewDraw : public SystemDraw {
public:
	ViewDraw(Ctrl *ctrl);
	~ViewDraw();
};

void DrawDragRect(Ctrl& q, const DrawDragRectInfo& f);

class DHCtrl : Ctrl {};

void InitGtkApp(int argc, char **argv, const char **envptr);

#define GUI_APP_MAIN \
void GuiMainFn_(); \
\
int main(int argc, char **argv, const char **envptr) { \
	UPP::AppInit__(argc, (const char **)argv, envptr); \
	UPP::InitGtkApp(argc, argv, envptr); \
	GuiMainFn_(); \
	UPP::Ctrl::CloseTopCtrls(); \
	UPP::AppExit__(); \
	return UPP::GetExitCode(); \
} \
 \
void GuiMainFn_()
