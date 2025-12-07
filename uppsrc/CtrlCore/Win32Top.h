public:
	virtual LRESULT  WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	dword       style;
	dword       exstyle;
	HICON       ico, lico;

	void        DeleteIco();
	void        SetIco();
	void        CenterRect(HWND owner, int center);

	One<FrameTop<Ctrl>>   custom_bar_frame;
	One<Ctrl>             custom_bar;
	Color                 custom_titlebar_bk = Null;
	int                   custom_titlebar_cy = 0;
	int                   active_titlebar_button = -1;
	bool                  active_titlebar_active = false;
	
	bool        IsCustomTitleBar__() const;
	Ctrl       *MakeCustomTitleBar__(Color bk, int mincy);
	void        SyncCustomBar();

public:
	void       Open(HWND ownerhwnd);
	TopWindow& Style(dword _style);
	dword      GetStyle() const                       { return style; }
	TopWindow& ExStyle(dword _exstyle);
	dword      GetExStyle() const                     { return exstyle; }
