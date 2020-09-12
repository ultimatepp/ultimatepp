//$ class Upp::Ctrl {
private:
	friend struct MMCtrl;
	friend struct MMImp;

	static int                 WndCaretTime;
	static bool                WndCaretVisible;
	static bool                local_dnd_copy;
	static void AnimateCaret();
	static Ptr<Ctrl>           lastActive;
	
	friend void CocoInit(int argc, const char **argv, const char **envptr);

protected:
	virtual void MMClose() {}

	void   Create(Ctrl *owner, dword style, bool active);
	static void SetNSAppImage(const Image& img);
	static void SyncAppIcon();
	static void ResetCocoaMouse();

public:
	static void      EndSession()              {}
	static bool      IsEndSession()            { return false; }
	
	void  *GetNSWindow() const;
	void  *GetNSView() const;
	bool   IsCocoActive() const;
	
	void   RegisterCocoaDropFormats();

	static Rect GetScreenArea(Point pt);
//$ };
