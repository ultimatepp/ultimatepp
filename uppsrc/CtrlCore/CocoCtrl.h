//$ class Ctrl {
private:
	friend struct MMCtrl;
	friend struct MMImp;

	friend void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                             Color color, int type, int animation);
	friend void FinishDragRect(Ctrl& q);

	static int                 scale;
	static double              scalei;
	static int                 WndCaretTime;
	static bool                WndCaretVisible;
	static void AnimateCaret();
	
	friend void CocoInit(int argc, const char **argv, const char **envptr);

protected:
	virtual void MMClose() {}

	void   Create(Ctrl *owner, dword style, bool active);

public:
	static void      EndSession()              {}
	static bool      IsEndSession()            { return false; }
	static int       Scale()                   { return scale; }
	static double    InvScale()                { return scalei; }
	
	void  *GetNSWindow() const;
	void  *GetNSView() const;
//$ };
