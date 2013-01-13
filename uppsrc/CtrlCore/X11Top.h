//$ class TopWindow {
protected:
	virtual void    EventProc(XWindow& w, XEvent *event);
private:
	XSizeHints *size_hints;
	XWMHints   *wm_hints;
	XClassHint *class_hint;
	Size        xminsize, xmaxsize;
	bool        topmost;

	void          CenterRect(Ctrl *owner);
	void          DefSyncTitle();
	void          EndIgnoreTakeFocus();

	void          SyncState();
	void          SyncState0();

	void          Open0(Ctrl *owner);

	Image         invert;
	WString       title2;

	static  Rect      windowFrameMargin;
//$ };
