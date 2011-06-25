//$ class Ctrl {
public:
	static void InitFB();
	static void DoMouseFB(int event, Point p, int zdelta = 0);
	
	void SetOpen(bool b)               { isopen = b; }
	void SetTop()                      { top = new Top; }
//$ };
