//$ class Ctrl {
private:
	friend struct MMCtrl;
	friend struct MMImp;

	static int                 WndCaretTime;
	static bool                WndCaretVisible;
	static void AnimateCaret();

protected:
	virtual void MMClose() {}

	void   Create(Ctrl *owner, dword style, bool active);

public:
	static void      EndSession()              {}
	static bool      IsEndSession()            { return false; }
//$ };
