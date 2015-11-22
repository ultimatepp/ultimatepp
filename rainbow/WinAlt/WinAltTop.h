//$ class TopWindow {
public:
	virtual LRESULT  WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	dword       style;
	dword       exstyle;
	HICON       ico, lico;

	void        DeleteIco0();
	void        DeleteIco();
	void        CenterRect(HWND owner, int center);

public:
	void       Open(HWND ownerhwnd);
	TopWindow& Style(dword _style);
	dword      GetStyle() const                       { return style; }
	TopWindow& ExStyle(dword _exstyle);
	dword      GetExStyle() const                     { return exstyle; }
//$ };