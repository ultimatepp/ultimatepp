//$ class Ctrl {
public:
	static void InitGl();
	static void DoMouseGl(int event, Point p, int zdelta = 0);
	static bool DoKeyGl(dword key, int cnt);
	static void RemoveCursor();
	static void RemoveCaret();
	static void CursorSync();
	
	static Image GetBak(Rect& tr);
	void  SetOpen(bool b)               { isopen = b; }
	void  SetTop()                      { top = new Top; }
//$ };
