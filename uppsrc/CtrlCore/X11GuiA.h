class TopFrameDraw : public SystemDraw {
public:
	TopFrameDraw(Ctrl *ctrl, const Rect& r);
	~TopFrameDraw();

protected:
	bool   caret;
};


#define GUI_APP_MAIN \
void GuiMainFn_(); \
\
int main(int argc, const char **argv, const char **envptr) { \
	UPP::AppInit__(argc, argv, envptr); \
	GUI_APP_MAIN_HOOK \
	UPP::Ctrl::InitX11(NULL); \
	UPP::AppExecute__(GuiMainFn_); \
	UPP::Ctrl::ExitX11(); \
	UPP::Ctrl::ShutdownThreads(); \
	UPP::AppExit__(); \
	return UPP::GetExitCode(); \
} \
 \
void GuiMainFn_()

class DHCtrl : public Ctrl {
	int isError;
	bool isMapped;
	Size CurrentSize;
	XVisualInfo* UserVisualInfo;
	String ErrorMessage;

	void MapWindow(bool map);
	bool Init(void);
	void Terminate(void);

protected:
	Visual     *GetVisual(void);
	XVisualInfo GetVisualInfo(void);

	virtual XVisualInfo *CreateVisual(void) {return 0;}
	virtual void SetAttributes(unsigned long &ValueMask, XSetWindowAttributes &attr) {}
	virtual void Paint(Draw &draw) {}
	virtual void BeforeInit(void) {}
	virtual void AfterInit(bool Error) {}
	virtual void BeforeTerminate(void) {}
	virtual void AfterTerminate(void) {}
	virtual void Resize(int w, int h) {}

	void SetError(bool err) { isError = err; }
	void SetErrorMessage(String const &msg) { ErrorMessage = msg; }

	virtual void State(int reason);
	
	Window   hwnd;	
	bool isInitialized;
	
public:
	typedef DHCtrl CLASSNAME;

	bool   IsInitialized(void) { return isInitialized; }

	bool   GetError(void) { return isError; }
	String GetErrorMessage(void) { return ErrorMessage; }

	DHCtrl();
	~DHCtrl();
};
