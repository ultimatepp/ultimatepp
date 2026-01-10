private:
	friend struct MMCtrl;
	friend struct MMImp;

	static bool      local_dnd_copy;
	static Ptr<Ctrl> lastActive;
	
	static bool      always_use_bundled_icon;
	
	friend void CocoInit(int argc, const char **argv, const char **envptr);
	
protected:
	virtual void MMClose() {}

	void   Create(Ctrl *owner, dword style, bool active);
	static void SetNSAppImage(const Image& img);
	static void SyncAppIcon();
	static void ResetCocoaMouse();
	static void DoCancelPreedit();
	static int  GetCaretBlinkTime()       { return 500; }

public:
	static void EndSession()              {}
	static bool IsEndSession()            { return false; }
	
	static void SetAlwaysUseBundledIcon(bool enable = true) { always_use_bundled_icon = enable; }
	static bool IsAlwaysUseBundledIcon()                    { return always_use_bundled_icon; }
	
	void  *GetNSWindow() const;
	void  *GetNSView() const;
	bool   IsCocoActive() const;
	
	void   RegisterCocoaDropFormats();

	static Rect GetScreenArea(Point pt);
