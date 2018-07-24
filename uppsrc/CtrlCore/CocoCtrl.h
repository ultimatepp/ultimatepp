//$ class Ctrl {
private:
	friend struct MMCtrl;
	friend struct MMImp;

	static int                 WndCaretTime;
	static bool                WndCaretVisible;
	static void AnimateCaret();

protected:
	virtual void MMClose() { DLOG("MMClose"); }

	void Create(dword style);

public:
	static void      EndSession()              {}
	static bool      IsEndSession()            { return false; }
//$ };
