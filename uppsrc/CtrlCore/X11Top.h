protected:
	virtual void    EventProc(XWindow& w, XEvent *event);
private:
	XSizeHints *size_hints;
	XWMHints   *wm_hints;
	XClassHint *class_hint;
	Size        xminsize, xmaxsize;
	bool        topmost;
	
	Buffer<unsigned long>  PreperIcon(const Image& icon, int& len);
	
	void                   CenterRect(Ctrl *owner);
	void                   DefSyncTitle();
	void                   EndIgnoreTakeFocus();

	void                   SyncState();

	Image                  invert;
	WString                title2;

	static  Rect           windowFrameMargin;

