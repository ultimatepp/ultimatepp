#ifndef _LinuxGl_After_h_
#define _LinuxGl_After_h_

class ViewDraw : public SystemDraw {
public:
	ViewDraw(Ctrl *ctrl) {}
	~ViewDraw() {}
};

class DHCtrl : Ctrl {};

int AppMain(int argc, const char **argv, const char **envptr);

#define GUI_APP_MAIN \
int main(int argc, const char **argv, const char **envptr) \
{ return UPP::AppMain(argc, argv, envptr); } \
\
void GuiMainFn_()

#endif
